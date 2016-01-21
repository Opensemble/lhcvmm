#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetBackgroundColor(50);
    ofSetFrameRate(FRAME_RATE);

    frameDuration = 1.0 / FRAME_RATE;
    framesMaxNumber = DURATION * FRAME_RATE;
    frameCounter = 0;
    isAnimating = false;

    fisheye.setup(tVariableFisheye);
    fisheyeAmount = 0.0;

    renderer.setup(FRAME_RATE, PNG_SEQUENCE, r1024);

    drawFbo.allocate(renderer.getFboWidth(), renderer.getFboHeight());

    verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.04, true, true);

    ofSetCircleResolution(60);

}

//--------------------------------------------------------------
void ofApp::update(){

    //display frame rate as window title
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    //animation data update
    if(isAnimating){
        frameCounter++;
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

    //draw drawFbo into Renderer's FBO with Fisheye FX
    fisheyeAmount = animValue * 0.35 + 0.5; //animated from 0.5 to 0.85
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
void ofApp::keyPressed(int key){

    switch (key) {
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
            }
            break;
        case '2':
            if(renderer.getOutputResolution()!= r512){
                renderer.setOutputResolution(r512);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.04, true, true);
                drawFbo.clear();
                drawFbo.allocate(renderer.getFboWidth(), renderer.getFboHeight());
            }
            break;
        case '3':
            if(renderer.getOutputResolution()!= r1024){
                renderer.setOutputResolution(r1024);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.04, true, true);
                drawFbo.clear();
                drawFbo.allocate(renderer.getFboWidth(), renderer.getFboHeight());
            }
            break;
        case '4':
            if(renderer.getOutputResolution()!= r2048){
                renderer.setOutputResolution(r2048);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.04, true, true);
                drawFbo.clear();
                drawFbo.allocate(renderer.getFboWidth(), renderer.getFboHeight());
            }
            break;
        case '5':
            if(renderer.getOutputResolution()!= r4096){
                renderer.setOutputResolution(r4096);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.04, true, true);
                drawFbo.clear();
                drawFbo.allocate(renderer.getFboWidth(), renderer.getFboHeight());
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
//--------------------------------------------------------------
void ofApp::drawScene(int w, int h){

    //LINEAR MOTION------------
    ofPushStyle();
    ofFill();
    //left to right moving circle
    ofSetColor(ofColor::red);
    ofDrawCircle(animValue*w, h*.5, w*0.05);
    //top to bottom moving circle
    ofSetColor(ofColor::violet);
    ofDrawCircle(w*.5, animValue*h, w*0.05);
    ofPopStyle();

    //CONCENTRIC MOTION------------------
    int circlesNum = 5;
    float speed = 2;
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(w*.5, h*.5);
    ofNoFill();
    ofSetColor(ofColor::white);
    ofSetLineWidth(w*0.002);
    for (int i=0; i<circlesNum; i++) {
        float v = animValue + (1/(float)circlesNum) * i ;
        if(v>1.0) v-= 1.0;
        int rad = (int)(v * speed * (h*.5)) % (int)(w*.5) ;
        //ofDrawCircle(0, 0,  rad);
        ofDrawRectangle(0, 0, rad*2, rad*2);
    }
    ofPopMatrix();
    ofPopStyle();

    //ROTATION----------------------
    int linesNum = 8;
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(w*.5, h*.5);
    ofRotate(animValue*360);
    ofSetColor(ofColor::green);
    ofSetLineWidth( w*0.003);
    for (int i=0; i<linesNum; i++) {
        float angle = ofDegToRad((360/(float)linesNum) * i);
        float radius = w*.5;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        ofDrawLine(0, 0, x, y);
    }
    ofPopMatrix();
    ofPopStyle();

    //DOME CIRCLE PROJECTION LIMIT DISPLAY-----------------
    ofPushStyle();
    ofSetColor(ofColor::red);
    ofNoFill();
    ofSetLineWidth( w*0.004);
    ofDrawCircle(w*.5, h*.5, w*.5);
    ofPopStyle();

    //INFO DISPLAY--------------------------------
    ofPushStyle();
    ofSetColor(ofColor::yellow);
    string sceneInfo = "Time: " + ofToString(animationTime, 2)
    + "\nFisheye: " + ofToString(fisheyeAmount, 2);
    verdana.drawString(sceneInfo, w*.35, h*.4);
    ofPopStyle();
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
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
