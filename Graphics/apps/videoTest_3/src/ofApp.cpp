#include "ofApp.h"

//FIXME: esta dado vuelta con el post-processing? no me importa mucho...
//TODO: make all animations frameNum dependants

#pragma mark - Core Funcs
//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetBackgroundColor(0);
    ///FRAME RATE: 30
    ofSetFrameRate(FRAME_RATE);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofEnableAlphaBlending();
    ofEnableDepthTest();
    
    //Setup settings-----------------
    data.load("analysisData.xml");
    
    int frameRate = data.getValue("FILE-INFO:frameRate",0);
    int totalFramesNum = data.getValue("FILE-INFO:totalFramesNum", 0);
    cout<<"frameRate - "<<frameRate<<endl;
    cout<<"total frames - "<<totalFramesNum<<endl;
    
    //renderer setup-------------------
    
    framesMaxNumber = totalFramesNum;
    frameCounter = -1;
    isAnimating = false;
    ofLogVerbose()<<"ANIMATION INFO ---- ";
    
    
    fisheye.setup(tVariableFisheye);
    fisheyeAmount = 0.0;
    
    //renderer.setup(FRAME_RATE, PNG_SEQUENCE, r256);///init resolution and mode
    //renderer.setup(FRAME_RATE, PNG_SEQUENCE, r512);///init resolution and mode
    //renderer.setup(FRAME_RATE, PNG_SEQUENCE, r1024);///init resolution and mode
    //renderer.setup(FRAME_RATE, PNG_SEQUENCE, r2048);///init resolution and mode
    renderer.setup(FRAME_RATE, PNG_SEQUENCE, r4096);///init resolution and mode
    
   
    verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.04, true, true);
    
    //
    
    drawFbo.allocate(renderer.getFboWidth(), renderer.getFboHeight());
    fw = drawFbo.getWidth();
    fh = drawFbo.getHeight();
    
    fboPost.allocate(fw, fh);
    fboPost.begin();
    ofClear(0,0,0,1);
    fboPost.end();
    
    fboInstanced.allocate(fw, fh, GL_RGBA);
    fboInstanced.begin();
        ofClear(255,255,255, 0);
    fboInstanced.end();
    
    fboParticles.allocate(fw, fh, GL_RGBA);
    fboParticles.begin();
    ofClear(255,255,255, 0);
    fboParticles.end();
    
    fboSphere.allocate(fw, fh, GL_RGBA);
    fboSphere.begin();
    ofClear(255,255,255, 0);
    fboSphere.end();
    
    
    //gui-------------
    setupGui();
    bShowGuiCubeSphere =false;
    bShowGuiPair = bShowGuiInstanced = true;
    
    
    //----------------------------
    _center.set(fw*0.5, fh*0.5, 0.0);
    cam.disableMouseInput();
    cam.setPosition(ofVec3f(_center.x, _center.y, fw*0.7));
    cam.lookAt(_center);
    cam.setTarget(_center);
    
    light.setPosition(fw*0.2, fw*.5, fh*0.3);
    light.setPointLight();
    
    //Instanced---------------------------
    instanced.setup(fw);
    instanced.setLimits(ofVec3f(fw, fh, 100));
    instanced.setOrientation(ofVec3f(1,1,1));
    instanced.setColor(COLOR_INSTANCED);
    
    //Particles------------------
    pair.setup(0, fh*0.5, fw, fh, OUT);
    pair.setColor(COLOR_PARTICLES);
    
    //Sphere------------------------
    sphere.setup();
    sphere.setColor(COLOR_SPHERE);
    sphere.setPosition(_center);
    
    //Post-Procesing-------------------------------
    postManager.setup(fw, fh);
    
    //OSC------------------------------------------
    receiver.setup(PORT);
    oscPower = 0.0;
    oscFreq = 0.0;
    oscConfidence = 0.0;
    oscSalience = 0.0;
    oscHfc = 0.0;
    oscCentroid = 0.0;
    oscSpecComp = 0.0;
    oscInharm = 0.0;
    oscOnset = false;
    oscTLtrack = 0.0;
    
   
    lastFrameWithOnset = 0;
    isReallyOnset = false;
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    //display frame rate as window title
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    //animation frameNumUpdate
    if(isAnimating){
        frameCounter++;
        //end recording and animation at 15"
        if (frameCounter>=framesMaxNumber){
            stopAnimation();
            renderer.stopRecording();
        }
    }
    
    
    //receiveOsc----------------
    if(gReceiveOSC){
        //receiveOsc();
        updateOscFromDataFile(frameCounter);
    }else{
        
    }
    
    //check trigger onset---------------------
    if(oscOnset && isAnimating){
        if(frameCounter-lastFrameWithOnset > 10){
            isReallyOnset = true;
            triggerOnset();
            lastFrameWithOnset = frameCounter;
        }
    }else{
        isReallyOnset=false;
    }

    
    //update graphics--------------------------------
    updateInstanced();
    updatePair();
    
    
    //update post-processing------------------------
    postManager.updateValues();
    
    //light pos
    light.setPosition(gLightPos->x * fw,
                      gLightPos->y * 0.8*fw,
                      0.05 * 0.8*fw );
    
    

    ///-------------------------------
    
   
    //-----------------------------------
    
    int rw = renderer.getFboWidth();
    int rh =  renderer.getFboHeight();
    
    ///draw openGL scene in drawFbo------------------
    drawFboInstanced();
    drawFboParticles();
    drawFboSphere();
    
    drawFboMain();
    
    drawFboPost();
    ///-------------------------------
    
   
    fisheyeAmount = 0.6;
    renderer.getFbo()->begin();
    ofClear(0);
    fisheye.begin(fboPost.getTexture(), rw, rh, fisheyeAmount);///which fbo to render
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(rw, 0); glVertex3f(rw, 0, 0);
    glTexCoord2f(rw, rh); glVertex3f(rw, rh, 0);
    glTexCoord2f(0,rh);  glVertex3f(0, rh, 0);
    glEnd();
    fisheye.end();
    renderer.getFbo()->end();
    
    //Record Renderer's FBO into a .mov file or png sequence
    renderer.update();


   
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    
    //fboPost.draw(ofGetWidth()-768, 0, fw, fh);
    renderer.draw(ofGetWidth()-768, 0, 768, 768);

    
    //drawGuis------------------
    if (bShowGuiInstanced)
        instanced.drawGui();
    if(bShowGuiPair)
        guiPair.draw();
    if(bShowGuiCubeSphere)
        sphere.drawGui();
    
    guiMain.draw();
    
    postManager.drawGui(200,500);
    
    
    //Recording indicator-------------------
    if(renderer.getIsRecording()){
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(ofGetWidth() - 60, 60);
        ofSetColor(255, 0, 0);
        ofDrawCircle(0,0, 40);
        ofSetColor(ofColor::white);
        ofDrawBitmapString("REC", -10, 0);
        ofPopMatrix();
        ofPopStyle();
    }
    
    //Display Key commands-----------------
    ofPushStyle();
    string keys = "KEY COMMANDS:";
    keys += "\nSpacebar: START/STOP Animation"
    "\nr: START/STOP Recording & Animation"
    "\np: Secuencia Png"
    "\nm: Archivo MOV-H264"
    "\n1: 256x256"
    "\n2: 512x512"
    "\n3: 1024x1024"
    "\n4: 2048x2048"
    "\n5: 4096x4096";
    ofSetColor(ofColor::white);
    ofDrawBitmapString(keys, ofGetWidth()-200, 20);
    ofPopStyle();
    
    //Display Info-----------------
    ofPushStyle();
    string info = "INFO: ";
    info += "\nfps: "+ofToString(ofGetFrameRate())
    + "\nFBO output res: " + renderer.getResolutionAsString()
    + "\nREC mode: " + renderer.getRecordingModeAsString() +
    + "\nframeCounter: " + ofToString(frameCounter);
    if(renderer.getIsRecording()){
        info  += "\nRECORDING FRAME NUM: " + ofToString(ofGetFrameNum() - renderer.getLastFrameMarker());
    }
    ofSetColor(ofColor::yellow);
    ofDrawBitmapString(info, ofGetWidth()-200, ofGetHeight()-100);
    ofPopStyle();

    
    
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key){
        case 'a':
            pair.addPartGroup(1);
            break;
        case 'k':
            pair.addPartGroup(2);
            break;
            
        
        
            //start-stop Animation--------------------
        case ' ':
            if(!isAnimating)startAnimation();
            else stopAnimation();
            break;
            //start-stop Animation & Recording---------------
        case 'r':
            if(!renderer.getIsRecording())renderer.startRecording();
            else renderer.stopRecording();
            
            if(!isAnimating)startAnimation();
            else stopAnimation();
            break;
            //FIXME: change resolutions arent working, setups needed
            //change resolution-----------------------------
        case '1':
            if(renderer.getOutputResolution()!= r256){
                renderer.setOutputResolution(r256);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.04, true, true);
                drawFbo.clear();
                drawFbo.allocate(renderer.getFboWidth(), renderer.getFboHeight());
                instanced.setLimits(ofVec3f(renderer.getFboWidth(), renderer.getFboHeight(), 100));
                
                fw = renderer.getFboWidth();
                fh = renderer.getFboHeight();
                
                fboPost.clear();
                fboPost.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
                
                fboInstanced.clear();
                fboInstanced.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
                
                fboParticles.clear();
                fboParticles.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
                
                fboSphere.clear();
                fboSphere.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
            
            }
            break;
        case '2':
            if(renderer.getOutputResolution()!= r512){
                renderer.setOutputResolution(r512);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.04, true, true);
                drawFbo.clear();
                drawFbo.allocate(renderer.getFboWidth(), renderer.getFboHeight());
                instanced.setLimits(ofVec3f(renderer.getFboWidth(), renderer.getFboHeight(), 100));
                
                fw = renderer.getFboWidth();
                fh = renderer.getFboHeight();
                
                fboPost.clear();
                fboPost.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
                
                fboInstanced.clear();
                fboInstanced.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
                
                fboParticles.clear();
                fboParticles.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
                
                fboSphere.clear();
                fboSphere.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
            }
            break;
        case '3':
            if(renderer.getOutputResolution()!= r1024){
                renderer.setOutputResolution(r1024);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.04, true, true);
                drawFbo.clear();
                drawFbo.allocate(renderer.getFboWidth(), renderer.getFboHeight());
                instanced.setLimits(ofVec3f(renderer.getFboWidth(), renderer.getFboHeight(), 100));
                
                fw = renderer.getFboWidth();
                fh = renderer.getFboHeight();
                
                fboPost.clear();
                fboPost.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
                
                
                fboInstanced.clear();
                fboInstanced.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
                
                fboParticles.clear();
                fboParticles.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
                
                fboSphere.clear();
                fboSphere.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
            }
            break;
        case '4':
            if(renderer.getOutputResolution()!= r2048){
                renderer.setOutputResolution(r2048);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.04, true, true);
                drawFbo.clear();
                drawFbo.allocate(renderer.getFboWidth(), renderer.getFboHeight());
                instanced.setLimits(ofVec3f(renderer.getFboWidth(), renderer.getFboHeight(), 100));
                
                fw = renderer.getFboWidth();
                fh = renderer.getFboHeight();
                
                fboPost.clear();
                fboPost.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
                
                fboInstanced.clear();
                fboInstanced.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
                
                fboParticles.clear();
                fboParticles.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
                
                fboSphere.clear();
                fboSphere.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
            }
            break;
        case '5':
            if(renderer.getOutputResolution()!= r4096){
                renderer.setOutputResolution(r4096);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.04, true, true);
                drawFbo.clear();
                drawFbo.allocate(renderer.getFboWidth(), renderer.getFboHeight());
                instanced.setLimits(ofVec3f(renderer.getFboWidth(), renderer.getFboHeight(), 100));
                
                fw = renderer.getFboWidth();
                fh = renderer.getFboHeight();
                
                fboPost.clear();
                fboPost.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
                
                fboInstanced.clear();
                fboInstanced.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
                
                fboParticles.clear();
                fboParticles.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);
                
                fboSphere.clear();
                fboSphere.allocate(renderer.getFboWidth(), renderer.getFboHeight(), GL_RGBA);

            }
            break;
            
            //change recording mode---------------
        case 'p':
            if(renderer.getRecordingMode()!=PNG_SEQUENCE) renderer.setRecordingMode(PNG_SEQUENCE);
            break;
        case 'm':
            if(renderer.getRecordingMode()!=MOV_FILE) renderer.setRecordingMode(MOV_FILE);
            break;
            
        default:
            break;
    }
    
}
#pragma mark - Fbo draw funcs
//--------------------------------------------------------------
void ofApp::drawDomeLimits(int w, int h){
    
    
    //ofBackground(255);
    int x = w*0.5;
    int y = h*0.5;
    int rad = w*0.5;
    
    ofPushStyle();
    ofSetCircleResolution(30);
    ofNoFill();
    //full
    ofSetColor(ofColor::red);
    ofDrawCircle(x,y,rad);
//    //mid
//    ofSetColor(ofColor::orange);
//    ofDrawCircle(x,y,rad*0.66);
//    //small
//    ofSetColor(ofColor::yellow);
//    ofDrawCircle(x,y,rad*0.33);
    
    
    ofPopStyle();
}
//--------------------------------------------------------------
void ofApp::drawFboPost(){

    fboPost.begin();
    ofClear(0,0,0,1);
    
    ofDisableAlphaBlending();
    
    postManager.begin();
    drawFbo.draw(0,0);
    postManager.end();
    
    fboPost.end();
    
    ofEnableAlphaBlending();
}
//--------------------------------------------------------------
void ofApp::drawFboMain(){
    
    drawFbo.begin();
    

    ofClear(0, 0, 0, 0);

    if(gDoDrawParts) fboParticles.draw(0,0);
   
    if(gDoDrawInstanced)fboInstanced.draw(0,0);
    
    if(gDoDrawSphere) fboSphere.draw(0,0);
    
    if(gDoDrawDomeLimits) drawDomeLimits(fw, fh);
    
    drawFbo.end();
}
//--------------------------------------------------------------
void ofApp::drawFboInstanced(){
    
    
    ofEnableAlphaBlending();
    
    fboInstanced.begin();
    ofClear(255,255,255, 0);
    //----------------------------
    if(gUseLight){
        ofEnableLighting();
        light.enable();
    }
    
    cam.begin();
    
    //??? hacen falta estos guachines?
    ofEnableDepthTest();
    ofDisableAlphaBlending();
    //-----------------------
    
    instanced.drawScene();
    
    
    ofDisableDepthTest();//??? hace falta?
    
    cam.end();
    
    if(gUseLight){
        light.disable();
        ofDisableLighting();
    }
    //----------------------------
    fboInstanced.end();
    
   
    
}
//--------------------------------------------------------------
void ofApp::drawFboParticles(){

    fboParticles.begin();
    ofClear(255,255,255, 0);
    //----------------------------
    if(gUseLight){
        ofEnableLighting();
        light.enable();
    }

    if(gUseCam)cam.begin();

    
    ofEnableDepthTest();
    ofEnableAlphaBlending();
   
    ///DRAW pair
    pair.drawScene();
   
    
    if(gUseCam)cam.end();
    
    if(gUseLight){
        light.disable();
        ofDisableLighting();
    }
    
    ofDisableDepthTest();
    //----------------------------
    fboParticles.end();
}
//--------------------------------------------------------------
void ofApp::drawFboSphere(){
    
    fboSphere.begin();
    ofClear(255,255,255, 0);
    //----------------------------
    
    
    ofEnableDepthTest();
   
    
    if(gUseLight){
        ofEnableLighting();
        light.enable();
    }

    cam.begin();
  

    ///drawSphere---------
    sphere.drawScene();
    ///---------
    
    
    
    if(gUseLight){
        light.disable();
        ofDisableLighting();
    }
    
    cam.end();
    
    ofDisableDepthTest();
    
    //----------------------------
    fboSphere.end();

}
#pragma mark - Updates
//--------------------------------------------------------------
void ofApp::updatePair(){
    
    int w = fw;
    
    guiPair.update();
    
    std::map<string, float> oscData;
    
    
    oscData[KEY_DIST_TRESHOLD] = guiPair.gDistTreshold;///osc
    oscData[KEY_PARTS_NUM]     = guiPair.gPartsNum;
    oscData[KEY_X_VELOCITY]    = guiPair.gXvelocity;///osc
    //oscData[KEY_RADIUS_INIT]   = guiPair.gRadiusInit;
    oscData[KEY_RADIUS_INIT]   = 0.009 * w;
    oscData[KEY_RADIUS_VAR]    = guiPair.gRadiusVar;///osc
    oscData[KEY_ANGLE_INIT]    = guiPair.gAngleInit;
    oscData[KEY_ANGLE_VAR]     = guiPair.gAngleVar;///osc
    //nz
    oscData[KEY_ANGLE_NZ_AMP]  = guiPair.gNzAngleAmp;///osc
    oscData[KEY_ANGLE_NZ_FREQ] = guiPair.gNzAngleFreq;
//    oscData[KEY_RADIUS_NZ_AMP] = guiPair.gNzRadAmp;///osc
    oscData[KEY_RADIUS_NZ_AMP] = 0.002 * w;///osc
    oscData[KEY_RADIUS_NZ_FREQ]= guiPair.gNzRadFreq;
    oscData[KEY_X_NZ_AMP]      = guiPair.gNzXposAmp;///osc
    oscData[KEY_X_NZ_FREQ]     = guiPair.gNzXposFreq;
    oscData[KEY_PART_SIZE]     = 10; ///osc
    
    if(gReceiveOSC){
        
        //oscData[KEY_DIST_TRESHOLD] = guiPair.gDistTreshold * oscCentroid * oscCentroid;
        //oscData[KEY_DIST_TRESHOLD] = 460 * oscCentroid * oscCentroid;
        if(oscHfc>0.5){
            oscData[KEY_DIST_TRESHOLD] = w * oscHfc;
        }
        
//        oscData[KEY_RADIUS_VAR]    = guiPair.gRadiusVar   * oscCentroid;
        oscData[KEY_RADIUS_VAR]    = 0.2*w   * oscCentroid;
        
        oscData[KEY_ANGLE_VAR]     = 0.025  *  oscSpecComp * oscSpecComp ;
        //oscData[KEY_ANGLE_VAR]     = 0.132 *  oscSpecComp * oscSpecComp ;
        
//        oscData[KEY_X_NZ_AMP]      =  guiPair.gNzXposAmp;
        oscData[KEY_X_NZ_AMP]      =  0.38 * w;
        
//        oscData[KEY_X_VELOCITY]    = guiPair.gXvelocity*0.5 + guiPair.gXvelocity*oscCentroid;
        oscData[KEY_X_VELOCITY]    = 0.8*w *0.5 + 0.8*w *oscCentroid;
        
        oscData[KEY_PART_SIZE]     = 0.004*w + oscPower*0.004*w;
    }

    std::map<string, float> pairData_A = oscData;
    std::map<string, float> pairData_B = oscData;
    

//    pair.setDistanceTreshold(guiPair.gDistTreshol/d * oscPower * oscPower * 5);///fixme
    pair.setDistanceTreshold(0.07*w * oscPower * oscPower * 5);
    
    pair.update(pairData_A, pairData_B);
    
}

//--------------------------------------------------------------
void ofApp::updateSphere(){
    
    //---------------------------------
    float x, y, vol, rad, res, vel;
    
    float val1 = oscCentroid;
    float val2 = oscSpecComp;
    
    x   = oscCentroid * sphere.xGui;
    y   = .001  + oscSpecComp * sphere.yGui;
    vol = 0.25  + oscPower * sphere.volumeGui;
    rad = 10    + oscSpecComp * sphere.radiusGui;
    res = sphere.dispResolution;
    vel  = 1;
    
    sphere.dispNzAmnt = sphere.strengthGui;
    
    //sphere.displacement.update(sphere.xGui, sphere.yGui, sphere.volumeGui, sphere.radiusGui, sphere.resolGui, ofGetFrameNum()*sphere.velGui);
    sphere.displacement.update(x, y, vol, rad, sphere.resolGui, ofGetFrameNum()*sphere.velGui);
    
}
//--------------------------------------------------------------
void ofApp::updateInstanced(){
    
    int w = fw;
    
    //update instancedManager values
    if(!instanced.gMode)instanced.setMode(LINEAL);
    else if(instanced.gMode && !instanced.gRadMode) instanced.setMode(RAD_CONCENTRIC);
    else if(instanced.gMode && instanced.gRadMode){
        instanced.setMode(RAD_CENTRIFUGE);
        instanced.setRadDeform(instanced.gRadDeform);
    }
    
    instanced.setWidth(instanced.gWidth);
    instanced.setHeight(instanced.gHeight);
    instanced.setCubeSize(instanced.gCubesizeUnified * MAX_CUBESIZE*w);
    
    instanced.setMaskRadius(instanced.gMaskRadius);///osc
    
    instanced.setHres(instanced.gHres * MAX_H_RES);
    instanced.setVres(instanced.gVres * MAX_V_RES);
    
    instanced.setVelocity(instanced.gVelocity * MAX_VELOCITY);///osc
    
    instanced.setXpos(instanced.gXpos);
    instanced.setYpos(instanced.gYpos);
    instanced.setZpos(instanced.gZpos);
    //nz
    instanced.setNzTime(instanced.gNzTime * MAX_NZ_TIME);
    
    instanced.setXnzAmp(instanced.gNzXAmp * MAX_NZ_AMP*w);///osc
    instanced.setXnzFreq(instanced.gNzXFreq * MAX_NZ_FREQ);
    instanced.setXnzRug(instanced.gNzXRug * MAX_NZ_RUG*w);
    
    instanced.setYnzAmp(instanced.gNzYAmp * MAX_NZ_AMP*w);
    instanced.setYnzFreq(instanced.gNzYFreq * MAX_NZ_FREQ);
    instanced.setYnzRug(instanced.gNzYRug * MAX_NZ_RUG*w);
    
    instanced.setZnzAmp(instanced.gNzZAmp * MAX_NZ_AMP*w);///osc
    instanced.setZnzFreq(instanced.gNzZFreq * MAX_NZ_FREQ);
    instanced.setZnzRug(instanced.gNzZRug * MAX_NZ_RUG*w);
    
    if(gReceiveOSC){
//        instanced.setVelocity(oscPower * oscPower * 0.7 * MAX_VELOCITY);//10
        instanced.setVelocity(oscPower * oscPower * 0.7 * MAX_VELOCITY);//10
       
        instanced.setZnzAmp(oscCentroid*oscCentroid * 6 * MAX_NZ_AMP*w);
        
//        instanced.setZnzAmp(oscCentroid * instanced.gNzZAmp*3 * MAX_NZ_AMP*w);
        
        instanced.setXnzAmp(oscSpecComp * oscSpecComp  * MAX_NZ_AMP*w);
        
        if(isReallyOnset)instanced.setMaskRadius(0.0);
        else instanced.setMaskRadius(0.5 - oscTLtrack*0.5);
        
        
    }


}

#pragma mark - Other funcs
//--------------------------------------------------------------
void ofApp::setupGui(){
    
    //-----------------------------------
    guiMain.setup("Main Panel");
    guiMain.setPosition(0, 0);
    guiMain.add(gDoPostProcessing.setup("Post-Proc", true));
    guiMain.add(gReceiveOSC.setup("Receive Osc", true));
    guiMain.add(gDoDrawInstanced.setup("Draw Inst", true));
    guiMain.add(gDoDrawSphere.setup("Draw Sphere", false));
    guiMain.add(gDoDrawParts.setup("Draw Parts", true));
    guiMain.add(gDoDrawDomeLimits.setup("Draw Dome Lim", true));
    guiMain.add(gLightPos.setup("LighPos", ofVec3f(0.5), ofVec3f(0.0), ofVec3f(0.5)));
    guiMain.add(gUseCam.setup("useCam", true));
    guiMain.add(gAxis.setup("axis", true));
    guiMain.add(gUseLight.setup("useLight", true));
    guiMain.add(gFisheye.setup("fisheye", 0.6, 0.0, 1.0));

    
    //--------------------------
    //??? mandarlo al particleSystemPair?
    guiPair.setup();
    guiPair.gui.setPosition(200, 0);


}

//--------------------------------------------------------------
void ofApp::resetCamera(){
    
    cam.setPosition(ofVec3f(fw*0.5, fh*0.5, 500.0));
    cam.lookAt(ofVec3f( fw*0.5, fh*0.5, 0.0));
    cam.setTarget(ofVec3f( fw*0.5, fh*0.5, 0.0));

}
//--------------------------------------------------------------
void ofApp::triggerOnset(){
    
    pair.addPartGroup(1);
    pair.addPartGroup(2);

}
//--------------------------------------------------------------
void ofApp::updateOscFromDataFile(int frameNum){
    
    string frameNumStr = ofToString(frameNum);
    
    oscPower = data.getValue("ANALYSIS-DATA:FRAME-" + frameNumStr + ":ANALYZER:CHANNEL-0:POWER",0.0);
 
    oscFreq = data.getValue("ANALYSIS-DATA:FRAME-" + frameNumStr + ":ANALYZER:CHANNEL-0:PITCHFREQ", 0.0);
    
    oscConfidence = data.getValue("ANALYSIS-DATA:FRAME-" + frameNumStr + ":ANALYZER:CHANNEL-0:PITCHCONF", 0.0);
    
    oscSalience = data.getValue("ANALYSIS-DATA:FRAME-" + frameNumStr + ":ANALYZER:CHANNEL-0:SALIENCE", 0.0);
    
    oscHfc = data.getValue("ANALYSIS-DATA:FRAME-" + frameNumStr + ":ANALYZER:CHANNEL-0:HFC", 0.0);
    
    oscCentroid = data.getValue("ANALYSIS-DATA:FRAME-" + frameNumStr + ":ANALYZER:CHANNEL-0:CENTROID", 0.0);
    
    oscSpecComp = data.getValue("ANALYSIS-DATA:FRAME-" + frameNumStr + ":ANALYZER:CHANNEL-0:SPECCOMP", 0.0);
    
    oscInharm = data.getValue("ANALYSIS-DATA:FRAME-" + frameNumStr + ":ANALYZER:CHANNEL-0:INHARM", 0.0);
    
    oscOnset = data.getValue("ANALYSIS-DATA:FRAME-" + frameNumStr + ":ANALYZER:CHANNEL-0:ONSET", 0.0);
    
    oscTLtrack = data.getValue("ANALYSIS-DATA:FRAME-" + frameNumStr + ":TIMELINE:TL-default", 0.0);
    
}
//--------------------------------------------------------------
void ofApp::receiveOsc(){
    
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        if(m.getAddress()=="/ch0"){
            oscPower        = m.getArgAsFloat(0);
            oscFreq         = m.getArgAsFloat(1);
            oscConfidence   = m.getArgAsFloat(2);
            oscSalience     = m.getArgAsFloat(3);
            oscHfc          = m.getArgAsFloat(4);
            oscCentroid     = m.getArgAsFloat(5);
            oscSpecComp     = m.getArgAsFloat(6);
            oscInharm       = m.getArgAsFloat(7);
            oscOnset        = m.getArgAsInt32(8);
        }
        else if(m.getAddress()=="/TL-default"){
            oscTLtrack       = m.getArgAsFloat(0);
        }

    }


}

//--------------------------------------------------------------
void ofApp::startAnimation(){
    isAnimating=true;
    ofLogNotice("Animation STARTED");
}
//--------------------------------------------------------------
void ofApp::stopAnimation(){
    frameCounter = -1;
    lastFrameWithOnset = 0;
    isAnimating = false;
    ofLogNotice("Animation STOPED");
}

