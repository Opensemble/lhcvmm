#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //openGl and GLSL info------
    ofLogVerbose()<<"GLVersionMajor: "<< ofGetGLRenderer()->getGLVersionMajor();
    ofLogVerbose()<<"GLVersionMinor: "<< ofGetGLRenderer()->getGLVersionMinor();
    ofLogVerbose()<<"GLSL Version: "<< ofGLSLVersionFromGL(ofGetGLRenderer()->getGLVersionMajor(), ofGetGLRenderer()->getGLVersionMinor());
    
    //renderer setup-------------------
    ofSetFrameRate(FRAME_RATE);
    frameDuration = 1.0 / FRAME_RATE;
    framesMaxNumber = DURATION * FRAME_RATE;
    frameCounter = 0;
    isAnimating = false;
    ofLogVerbose()<<"ANIMATION INFO ---- ";
    ofLogVerbose()<<"Frame Rate: "<< FRAME_RATE;
    ofLogVerbose()<<"Frame Duration: "<< frameDuration;
    ofLogVerbose()<<"Frames Max Number: "<< framesMaxNumber <<"\n---------";
    
    
    fisheye.setup(tVariableFisheye);
    fisheyeAmount = 0.0;
    
    renderer.setup(FRAME_RATE, PNG_SEQUENCE, r1024);
    
    drawFbo.allocate(renderer.getFboWidth(), renderer.getFboHeight());
    verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.04, true, true);
    
    
    //shaders---------------------
#ifdef TARGET_OPENGLES
	shader.load("shadersES2/shader");
#else
	if(ofIsGLProgrammableRenderer()){
        ofLogVerbose()<<"Using ProgrammableRenderer";
	}else{
        ofLogVerbose()<<"NOT Using ProgrammableRenderer";
	}
#endif
    
    //gui-------------
    gm.setup();

    ofSetBackgroundColor(80);
    
    cam.lookAt(ofVec3f( ofGetWidth()*.5, ofGetHeight()*.5, 0.0));
    light.setPosition(ofGetWidth()*.5, ofGetHeight()*.5, 150.0);
    light.setPointLight();
    
    instanced.setup();
    instanced.setLimits(ofVec3f(renderer.getFboWidth(), renderer.getFboHeight(), 100));
    instanced.setOrientation(ofVec3f(1,1,1));
    instanced.setColor(ofColor::white);
    
    currentValuesMode = TIMELINE;
    bShowGui = false;

}

//--------------------------------------------------------------
void ofApp::update(){
    //display frame rate as window title
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    //update instancedManager values
    updateInstancedValues();
    
    //animation data update
    if(isAnimating){
        frameCounter++;
        timelineApp->timeline.setCurrentFrame(frameCounter);
        animationTime = frameCounter * frameDuration;
        animValue = animationTime/DURATION;
        
        //end recording and animation at 15"
        if (frameCounter>=framesMaxNumber){
            stopAnimation();
            renderer.stopRecording();
        }
    }
    //-----------------------------------
    
    int rw = renderer.getFboWidth();
    int rh =  renderer.getFboHeight();
    
    //draw openGL scene in drawFbo
    drawFbo.begin();
    ofClear(0);
    drawScene(rw, rh);
    drawFbo.end();
    
    //fisheye with timeline
    fisheyeAmount = timelineApp->timeline.getValue("fisheye");
    
    renderer.getFbo()->begin();
    ofClear(0);
    fisheye.begin(drawFbo.getTexture(), rw, rh, fisheyeAmount);
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
    
    //drawScene(ofGetHeight(), ofGetWidth());
    if (bShowGui) gm.gui.draw();
    
    //Scaled Renderer's FBO preview----------
    renderer.draw(250, 0, 512, 512);
    
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
    ofDrawBitmapString(keys, 10, 20);
    ofPopStyle();
    
    //Display Info-----------------
    ofPushStyle();
    string info = "INFO: ";
    info += "\nfps: "+ofToString(ofGetFrameRate())
    + "\nFBO output res: " + renderer.getResolutionAsString()
    + "\nREC mode: " + renderer.getRecordingModeAsString();
    if(renderer.getIsRecording()){
        info  += "\nRECORDING FRAME NUM: " + ofToString(ofGetFrameNum() - renderer.getLastFrameMarker());
    }
    ofSetColor(ofColor::yellow);
    ofDrawBitmapString(info, 10, ofGetHeight()-100);
    ofPopStyle();

}
//--------------------------------------------------------------
void ofApp::exit(){
    renderer.exit();
}
//--------------------------------------------------------------
void ofApp::drawScene(int w, int h){
    
    if(gm.gUseLight){
        ofEnableLighting();
        light.enable();
    }
    
    if(gm.gUseCam)cam.begin();
    
    if(gm.gAxis)ofDrawAxis(200);
    //light.draw();
    
    instanced.draw();
    
    if(gm.gUseCam)cam.end();
    
    if(gm.gUseLight){
        light.disable();
        ofDisableLighting();
    }
    
    //INFO DISPLAY--------------------------------
    ofPushStyle();
    ofSetColor(ofColor::white);
    string sceneInfo = "Time: " + ofToString(animationTime, 2)
    + "\nFisheye: " + ofToString(fisheyeAmount, 2);
    verdana.drawString(sceneInfo, w*.35, h-h*.15);
    ofPopStyle();

}

//--------------------------------------------------------------
void ofApp::updateInstancedValues(){
    
    float w = renderer.getFboWidth();
    
    if(currentValuesMode==GUI){
        
        if(!gm.gMode)instanced.setMode(LINEAL);
        else if(gm.gMode && !gm.gRadMode) instanced.setMode(RAD_CONCENTRIC);
        else if(gm.gMode && gm.gRadMode){
            instanced.setMode(RAD_CENTRIFUGE);
            instanced.setRadDeform(gm.gRadDeform);
        }
        
        instanced.setWidth(gm.gWidth);
        instanced.setHeight(gm.gHeight);
        instanced.setCubeSize(gm.gCubesizeUnified * MAX_CUBESIZE*w);
        //    instanced.setCubeSize(ofVec3f(gCubesize->x * MAX_CUBESIZE,
        //                                  gCubesize->y * MAX_CUBESIZE,
        //                                  gCubesize->z * MAX_CUBESIZE));
        instanced.setMaskRadius(gm.gMaskRadius);
        instanced.setHres(gm.gHres * MAX_H_RES);
        instanced.setVres(gm.gVres * MAX_V_RES);
        instanced.setVelocity(gm.gVelocity * MAX_VELOCITY);
        instanced.setXpos(gm.gXpos);
        instanced.setYpos(gm.gYpos);
        //nz
        instanced.setNzTime(gm.gNzTime * MAX_NZ_TIME);
        
        instanced.setXnzAmp(gm.gNzXAmp * MAX_NZ_AMP*w);
        instanced.setXnzFreq(gm.gNzXFreq * MAX_NZ_FREQ);
        instanced.setXnzRug(gm.gNzXRug * MAX_NZ_RUG*w);
        
        instanced.setYnzAmp(gm.gNzYAmp * MAX_NZ_AMP*w);
        instanced.setYnzFreq(gm.gNzYFreq * MAX_NZ_FREQ);
        instanced.setYnzRug(gm.gNzYRug * MAX_NZ_RUG*w);
        
        instanced.setZnzAmp(gm.gNzZAmp * MAX_NZ_AMP*w);
        instanced.setZnzFreq(gm.gNzZFreq * MAX_NZ_FREQ);
        instanced.setZnzRug(gm.gNzZRug * MAX_NZ_RUG*w);
        
        //light pos
        
        light.setPosition(gm.gLightPos->x * MAX_LIGHT_X,
                          gm.gLightPos->y * MAX_LIGHT_Y,
                          gm.gLightPos->z * MAX_LIGHT_Z );
    }
    else if (currentValuesMode==TIMELINE){
        
        if(!timelineApp->timeline.isSwitchOn("mode"))instanced.setMode(LINEAL);
        else if(timelineApp->timeline.isSwitchOn("mode") && !timelineApp->timeline.isSwitchOn("radMode")) instanced.setMode(RAD_CONCENTRIC);
        else if(timelineApp->timeline.isSwitchOn("mode")  && timelineApp->timeline.isSwitchOn("radMode")){
            instanced.setMode(RAD_CENTRIFUGE);
            instanced.setRadDeform(timelineApp->timeline.getValue("radDeform"));
        }
        
        instanced.setWidth(timelineApp->timeline.getValue("width"));
        instanced.setHeight(timelineApp->timeline.getValue("height"));
        instanced.setCubeSize(timelineApp->timeline.getValue("cubesize") * MAX_CUBESIZE *w);
        instanced.setMaskRadius(timelineApp->timeline.getValue("maskRadius"));
        instanced.setHres(timelineApp->timeline.getValue("Hres") * MAX_H_RES);
        instanced.setVres(timelineApp->timeline.getValue("Vres") * MAX_V_RES);
        instanced.setXpos(timelineApp->timeline.getValue("Xpos"));
        instanced.setYpos(timelineApp->timeline.getValue("Ypos"));
        instanced.setVelocity(timelineApp->timeline.getValue("velocity") * MAX_VELOCITY);
        //nz
        instanced.setNzTime(timelineApp->timeline.getValue("nzTime") * MAX_NZ_TIME);
        
        instanced.setXnzAmp(timelineApp->timeline.getValue("nzXAmp") * MAX_NZ_AMP *w);
        instanced.setXnzFreq(timelineApp->timeline.getValue("nzXFreq") * MAX_NZ_FREQ);
        instanced.setXnzRug(timelineApp->timeline.getValue("nzXRug") * MAX_NZ_RUG *w);
        
        instanced.setYnzAmp(timelineApp->timeline.getValue("nzYAmp") * MAX_NZ_AMP *w);
        instanced.setYnzFreq(timelineApp->timeline.getValue("nzYFreq") * MAX_NZ_FREQ);
        instanced.setYnzRug(timelineApp->timeline.getValue("nzYRug") * MAX_NZ_RUG *w);
        
        instanced.setZnzAmp(timelineApp->timeline.getValue("nzZAmp") * MAX_NZ_AMP*w);
        instanced.setZnzFreq(timelineApp->timeline.getValue("nzZFreq") * MAX_NZ_FREQ);
        instanced.setZnzRug(timelineApp->timeline.getValue("nzZRug") * MAX_NZ_RUG *w);
        
        //light pos
        
        light.setPosition(gm.gLightPos->x * MAX_LIGHT_X,
                          gm.gLightPos->y * MAX_LIGHT_Y,
                          gm.gLightPos->z * MAX_LIGHT_Z );
        
        
    }

}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key){
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
            //change resolution-----------------------------
        case '1':
            if(renderer.getOutputResolution()!= r256){
                renderer.setOutputResolution(r256);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.04, true, true);
                drawFbo.clear();
                drawFbo.allocate(renderer.getFboWidth(), renderer.getFboHeight());
                instanced.setLimits(ofVec3f(renderer.getFboWidth(), renderer.getFboHeight(), 100));
            }
            break;
        case '2':
            if(renderer.getOutputResolution()!= r512){
                renderer.setOutputResolution(r512);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.04, true, true);
                drawFbo.clear();
                drawFbo.allocate(renderer.getFboWidth(), renderer.getFboHeight());
                instanced.setLimits(ofVec3f(renderer.getFboWidth(), renderer.getFboHeight(), 100));
            }
            break;
        case '3':
            if(renderer.getOutputResolution()!= r1024){
                renderer.setOutputResolution(r1024);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.04, true, true);
                drawFbo.clear();
                drawFbo.allocate(renderer.getFboWidth(), renderer.getFboHeight());
                instanced.setLimits(ofVec3f(renderer.getFboWidth(), renderer.getFboHeight(), 100));
            }
            break;
        case '4':
            if(renderer.getOutputResolution()!= r2048){
                renderer.setOutputResolution(r2048);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.04, true, true);
                drawFbo.clear();
                drawFbo.allocate(renderer.getFboWidth(), renderer.getFboHeight());
                instanced.setLimits(ofVec3f(renderer.getFboWidth(), renderer.getFboHeight(), 100));
            }
            break;
        case '5':
            if(renderer.getOutputResolution()!= r4096){
                renderer.setOutputResolution(r4096);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.04, true, true);
                drawFbo.clear();
                drawFbo.allocate(renderer.getFboWidth(), renderer.getFboHeight());
                instanced.setLimits(ofVec3f(renderer.getFboWidth(), renderer.getFboHeight(), 100));
            }
            break;
            
            //change recording mode---------------
        case 'p':
            if(renderer.getRecordingMode()!=PNG_SEQUENCE) renderer.setRecordingMode(PNG_SEQUENCE);
            break;
        case 'm':
            if(renderer.getRecordingMode()!=MOV_FILE) renderer.setRecordingMode(MOV_FILE);
            break;

        case 'g':
            bShowGui = !bShowGui;//show-hide gui
            break;
        case 't':
            if(currentValuesMode==GUI)currentValuesMode=TIMELINE;
            else if(currentValuesMode==TIMELINE)currentValuesMode=GUI;
            break;
        default:
            break;
    }
    
}
//--------------------------------------------------------------
void ofApp::startAnimation(){
    isAnimating=true;
    ofLogNotice("Animation STARTED");
}
//--------------------------------------------------------------
void ofApp::stopAnimation(){
    frameCounter = 0;
    isAnimating = false;
    ofLogNotice("Animation STOPED");
}

