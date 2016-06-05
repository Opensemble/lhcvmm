#include "ofApp.h"

//TODO: add OSC with Sonoscopio
//TODO: add behaivour react to simulated parameters onset, power, etc - gui

//TODO: para el jueves: demo con wav viejo, con OSC


//FIXME: esta dado vuelta con el post-processing?

//TODO: make all animations frameNum dependants

#pragma mark - Core Funcs
//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetBackgroundColor(0);
    ///FRAME RATE: 30
    ofSetFrameRate(30.0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofEnableAlphaBlending();
    ofEnableDepthTest();
    
    //
    
    drawFbo.allocate(768,768);
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
    bShowGuiCubeSphere = bShowGuiPair = bShowGuiInstanced = true;
    
    
    //----------------------------
    _center.set(fw*0.5, fh*0.5, 0.0);
    
    cam.setPosition(ofVec3f(_center.x, _center.y, 500.0));
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
    
    isOnset = false;
    elapsed = 0.0;
    lastElapsed = 0.0;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    //display frame rate as window title
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    //receive osc-----------------------------------
    bool prevFrameOnset = oscOnset;
    receiveOsc();
    float timemMillis = ofGetElapsedTimeMillis();
    
    elapsed = timemMillis - lastElapsed;
    
    float timeTreshold = 400.0;
    if(oscOnset && !prevFrameOnset && elapsed>timeTreshold){
        isOnset = true;
        lastElapsed = ofGetElapsedTimeMillis();
    }else{
        isOnset = false;
    }
    
    //onsets trigger---------------------------------
    if(isOnset) triggerOnset();
    
    //update graphics--------------------------------
    //instanced.update();
    updateInstanced();
    updatePair();
    updateSphere();
    //sphere.update();
    
    //update post-processing------------------------
    postManager.updateValues();
    
    //light pos
    light.setPosition(gLightPos->x * MAX_LIGHT_X,
                      gLightPos->y * MAX_LIGHT_Y,
                      gLightPos->z * MAX_LIGHT_Z );
    
    //draw openGL scene in drawFbo
    drawFboInstanced();
    drawFboParticles();
    drawFboSphere();
    
    drawFboMain();
    
    drawFboPost();



   
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    
    //drawFbo.draw(ofGetWidth()-768, 0, fw, fh);
    fboPost.draw(ofGetWidth()-768, 0, fw, fh);
    
    
    if (bShowGuiInstanced)
        instanced.drawGui();
    if(bShowGuiPair)
        guiPair.draw();
    if(bShowGuiCubeSphere)
        sphere.drawGui();
    
    guiMain.draw();
    
    postManager.drawGui(200,500);

    
    string info =
    "power: " + ofToString(oscPower) +
    "\nfreq: " + ofToString(oscFreq) +
    "\nhfc: " + ofToString(oscHfc) +
    "\nonset: " + ofToString(oscOnset);
    
    ofDrawBitmapString(info, ofGetWidth()-150, 10);
    
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
            
        //gui shows
            
        case '1':
            bShowGuiInstanced = !bShowGuiInstanced;
            break;
        case '2':
            bShowGuiPair = !bShowGuiPair;
            break;
        case '3':
            bShowGuiCubeSphere = !bShowGuiCubeSphere;
            break;
            
        case 'r':
            resetCamera();
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
    
//    ofClear(255, 255, 255, 0);
    ofClear(0, 0, 0, 0);

    
    if(gDoDrawInstanced)fboInstanced.draw(0,0);

    if(gDoDrawParts) fboParticles.draw(0,0);
    
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
    //??? mandarlo al particleSystemPair?
    bool onset1, onset2;
    float pow1, pow2;
    float pitch1, pitch2;
    float conf1, conf2;
    
    
    pow1 = pow2      = 0.8;
    onset1 = onset2  = false;
    pitch1 = pitch2  = 0.8;
    conf1 = conf2    = 0.8;
    
    guiPair.update();
    
    
   
    
    std::map<string, float> oscData;
    
    
    oscData[KEY_DIST_TRESHOLD] = guiPair.gDistTreshold * oscPower * oscPower;
    oscData[KEY_PARTS_NUM]     = guiPair.gPartsNum;
    oscData[KEY_X_VELOCITY]    = guiPair.gXvelocity;
    oscData[KEY_RADIUS_INIT]   = guiPair.gRadiusInit;
    oscData[KEY_RADIUS_VAR]    = guiPair.gRadiusVar * oscPower * oscPower;
    oscData[KEY_ANGLE_INIT]    = guiPair.gAngleInit;
    oscData[KEY_ANGLE_VAR]     = guiPair.gAngleVar *  oscPower * oscPower;
    //nz
    oscData[KEY_ANGLE_NZ_AMP]  = guiPair.gNzAngleAmp * oscCentroid;
    oscData[KEY_ANGLE_NZ_FREQ] = guiPair.gNzAngleFreq;
    oscData[KEY_RADIUS_NZ_AMP] = guiPair.gNzRadAmp * oscCentroid;
    oscData[KEY_RADIUS_NZ_FREQ]= guiPair.gNzRadFreq;
    oscData[KEY_X_NZ_AMP]      = guiPair.gNzXposAmp;
    oscData[KEY_X_NZ_FREQ]     = guiPair.gNzXposFreq;
    oscData[KEY_PART_SIZE]     = 15 * oscPower;

    
//    std::map<string, float> pairData_A = guiPair.getData();
//    std::map<string, float> pairData_B = guiPair.getData();

    std::map<string, float> pairData_A = oscData;
    std::map<string, float> pairData_B = oscData;

    
    //-----------------
//    //vars that change in continuum MODE
//    if(pair.getIsContinuum() ){
//
//        pairData_A[KEY_X_VELOCITY]  = minVelX + pow1  *  pitch1 * maxVelX;
//        pairData_B[KEY_X_VELOCITY]  = minVelX + pow2  *  pitch2 * maxVelX;
//
//        pairData_A[KEY_ANGLE_VAR]  =  pitch1  *  pitch1  * maxAngleVar;
//        pairData_B[KEY_ANGLE_VAR]  =  pitch2  *  pitch2  *maxAngleVar;
//
//    }else{
//
//        pairData_A[KEY_X_VELOCITY]  = minVelX + pow1 * maxVelX;
//        pairData_B[KEY_X_VELOCITY]  = minVelX + pow2 * maxVelX;
//
//        pairData_A[KEY_ANGLE_VAR]  =  pitch1 * maxAngleVar;
//        pairData_B[KEY_ANGLE_VAR]  =  pitch2 * maxAngleVar;
//
//    }
    
    pair.setDistanceTreshold(guiPair.gDistTreshold);
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
    
    ///instanced.setMaskRadius(instanced.gMaskRadius);
    instanced.setMaskRadius(1-ofClamp(oscHfc * 10.0 * instanced.gMaskRadius, 0.45, .85));
    
    instanced.setHres(instanced.gHres * MAX_H_RES);
    instanced.setVres(instanced.gVres * MAX_V_RES);
    
    ///instanced.setVelocity(instanced.gVelocity * MAX_VELOCITY);
    instanced.setVelocity(oscPower * instanced.gVelocity * MAX_VELOCITY);
    
    instanced.setXpos(instanced.gXpos);
    instanced.setYpos(instanced.gYpos);
    instanced.setZpos(instanced.gZpos);
    //nz
    instanced.setNzTime(instanced.gNzTime * MAX_NZ_TIME);
    
    ///instanced.setXnzAmp(instanced.gNzXAmp * MAX_NZ_AMP*w);
    instanced.setXnzAmp(oscSpecComp * oscSpecComp * instanced.gNzXAmp * MAX_NZ_AMP*w);
    instanced.setXnzFreq(instanced.gNzXFreq * MAX_NZ_FREQ);
    instanced.setXnzRug(instanced.gNzXRug * MAX_NZ_RUG*w);
    
    instanced.setYnzAmp(instanced.gNzYAmp * MAX_NZ_AMP*w);
    instanced.setYnzFreq(instanced.gNzYFreq * MAX_NZ_FREQ);
    instanced.setYnzRug(instanced.gNzYRug * MAX_NZ_RUG*w);
    
    ///instanced.setZnzAmp(instanced.gNzZAmp * MAX_NZ_AMP*w);
    instanced.setZnzAmp(oscCentroid*oscCentroid * instanced.gNzZAmp * MAX_NZ_AMP*w);
    
    instanced.setZnzFreq(instanced.gNzZFreq * MAX_NZ_FREQ);
    instanced.setZnzRug(instanced.gNzZRug * MAX_NZ_RUG*w);


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
    guiMain.add(gDoDrawSphere.setup("Draw Sphere", true));
    guiMain.add(gDoDrawParts.setup("Draw Parts", true));
    guiMain.add(gDoDrawDomeLimits.setup("Draw Dome Lim", true));
    guiMain.add(gLightPos.setup("LighPos", ofVec3f(0.5), ofVec3f(0.0), ofVec3f(1.0)));
    guiMain.add(gUseCam.setup("useCam", true));
    guiMain.add(gAxis.setup("axis", true));
    guiMain.add(gUseLight.setup("useLight", true));

    
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
//        string address  = m.getAddress();
//        
//        //substract channel from adress:
//        //   "/ch0/POWER" -> ch0
//        string channelStr = address.substr(1,3);
//        
//        //use only channel 0
//        if(channelStr!="ch0"){
//            return;
//        }
//        
//        //remove channel string from address to get the algorithm
//        string algorithmStr =  m.getAddress();
//        algorithmStr.erase(0, 5);//removes "/ch0/" (5 characters)
//        
//        
//        if(algorithmStr == MTR_NAME_POWER){
//            oscPower = m.getArgAsFloat(0);
//        }
//        else if(algorithmStr == MTR_NAME_PITCH_FREQ){
//            oscFreq = m.getArgAsFloat(0);
//        }
//        else if(algorithmStr == MTR_NAME_PITCH_CONF){
//            oscConfidence = m.getArgAsFloat(0);
//        }
//        else if(algorithmStr == MTR_NAME_PITCH_SALIENCE){
//            oscSalience = m.getArgAsFloat(0);
//        }
//        else if(algorithmStr == MTR_NAME_HFC){
//            oscHfc = m.getArgAsFloat(0);
//        }
//        else if(algorithmStr == MTR_NAME_CENTROID){
//            oscCentroid = m.getArgAsFloat(0);
//        }
//        else if(algorithmStr == MTR_NAME_INHARMONICTY){
//            oscInharm = m.getArgAsFloat(0);
//        }
//        else if(algorithmStr == MTR_NAME_SPEC_COMP){
//            oscSpecComp = m.getArgAsFloat(0);
//        }
//        else if(algorithmStr == MTR_NAME_ONSETS){
//            oscOnset = m.getArgAsInt32(0);
//        }
//        else if(m.getAddress() == "/ch1/pitch") {
//            ch1.pitch      = m.getArgAsFloat(0);
//            ch1.confidence = m.getArgAsFloat(1);
//            ch1.salience   = m.getArgAsFloat(2);
//        }
//        else if(m.getAddress() == "/ch1/onsets"){
//            ch1.onsets = m.getArgAsInt32(0);
//        }
//        else if(m.getAddress() == "/ch1/spectral"){
//            ch1.hfc      = m.getArgAsFloat(0);
//            ch1.centroid = m.getArgAsFloat(1);
//            ch1.complx   = m.getArgAsFloat(2);
//            ch1.inharmon = m.getArgAsFloat(3);
//        }
//        else if(m.getAddress() == "/ch2/intensity"){
//            ch2.rms    = m.getArgAsFloat(0);
//            ch2.energy = m.getArgAsFloat(1);
//            ch2.power  = m.getArgAsFloat(2);
//        }
//        else if(m.getAddress() == "/ch2/pitch") {
//            ch2.pitch      = m.getArgAsFloat(0);
//            ch2.confidence = m.getArgAsFloat(1);
//            ch2.salience   = m.getArgAsFloat(2);
//        }
//        else if(m.getAddress() == "/ch2/onsets"){
//            ch2.onsets = m.getArgAsInt32(0);
//        }
//        else if(m.getAddress() == "/ch2/spectral"){
//            ch2.hfc      = m.getArgAsFloat(0);
//            ch2.centroid = m.getArgAsFloat(1);
//            ch2.complx   = m.getArgAsFloat(2);
//            ch2.inharmon = m.getArgAsFloat(3);
//        }
        
        
//        // unrecognized message: display on the bottom of the screen
//        string msg_string;
//        msg_string = m.getAddress();
//        msg_string += ": ";
//        for(int i = 0; i < m.getNumArgs(); i++){
//            // get the argument type
//            msg_string += m.getArgTypeName(i);
//            msg_string += ":";
//            // display the argument - make sure we get the right type
//            if(m.getArgType(i) == OFXOSC_TYPE_INT32){
//                msg_string += ofToString(m.getArgAsInt32(i));
//            }
//            else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
//                msg_string += ofToString(m.getArgAsFloat(i));
//            }
//            else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
//                msg_string += m.getArgAsString(i);
//            }
//            else{
//                msg_string += "unknown";
//            }
//        }
//        ofLogVerbose()<<"osc: "<<msg_string;
        


    }


}

