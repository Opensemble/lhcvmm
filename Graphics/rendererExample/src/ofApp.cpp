#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(FRAME_RATE);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    ofBackground(50);
    
    renderer.setup(FRAME_RATE, PNG_SEQUENCE, r1024);
    
    verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.05, true, true);
    
    lastTimeMarker = 0.0;

}

//--------------------------------------------------------------
void ofApp::update(){
    
    //dibujar contexto openGL en el fbo del renderer
    renderer.getFbo()->begin();
        ofClear(255);
        drawTestScene(renderer.getFboWidth(),  renderer.getFboHeight());
    renderer.getFbo()->end();
    
    //grabar fbo en archivo png o frame del .mov
    renderer.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //preview del fbo del renderer escalado----
    renderer.draw(250, 0, 512, 512);

    //recording indicator-------------------
    if(renderer.getIsRecording()){
        ofPushStyle();
        ofSetColor(255, 0, 0);
        ofDrawCircle(ofGetWidth() - 60, 60, 40);
        ofPopStyle();
    }
   
    //draw key commands-----------------
    ofPushStyle();
    string keys = "KEY COMMANDS:";
    keys += "\nSpacebar: START/STOP Recording"
    "\np: Secuencia Png"
    "\nm: Archivo MOV-H264"
    "\n1: 256x256"
    "\n2: 512x512"
    "\n3: 1024x1024"
    "\n4: 2048x2048"
    "\n5: 4096x4096"
    ;
    ofSetColor(ofColor::white);
    ofDrawBitmapString(keys, 10, 20);
    ofPopStyle();
    
    //draw info-----------------
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
void ofApp::drawTestScene(int w, int h){
    
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(w * .5, h * .5);
    ofSetColor(ofColor::blue);
    float radius = w * 0.25 + w * 0.15 * sin(ofGetFrameNum()*0.1);//animacion dependiente del n¼ de frame
    ofFill();		// draw "filled shapes"
    ofDrawCircle(0,0,radius);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofRotate(int (ofGetFrameNum()*2)% 360);//animacion dependiente del n¼ de frame
    ofSetColor(ofColor::green, 150);
    ofRect(0,0, w*0.4, w*0.4);
    ofPopStyle();
    ofPopMatrix();
    
    if(renderer.getIsRecording()){
        ofSetColor(ofColor::red);
        verdana.drawString(ofToString(ofGetElapsedTimef()-lastTimeMarker),w*.4, h*.5);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
        //start-stop recording
        case ' ':
            if(!renderer.getIsRecording()){
                renderer.startRecording();
                lastTimeMarker = ofGetElapsedTimef();//para medir el tiempo transcurrido desde que se empieza a grabar
            }else{
                renderer.stopRecording();
            }
            break;
            
        //change resolution
        case '1':
            if(renderer.getOutputResolution()!= r256){
                renderer.setOutputResolution(r256);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.05, true, true);
            }
            break;
        case '2':
            if(renderer.getOutputResolution()!= r512){
                renderer.setOutputResolution(r512);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.05, true, true);
            }
            break;
        case '3':
            if(renderer.getOutputResolution()!= r1024){
                renderer.setOutputResolution(r1024);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.05, true, true);
            }
            break;
        case '4':
            if(renderer.getOutputResolution()!= r2048){
                renderer.setOutputResolution(r2048);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.05, true, true);
            }
            break;
        case '5':
            if(renderer.getOutputResolution()!= r4096){
                renderer.setOutputResolution(r4096);
                verdana.load("fonts/verdana.ttf", renderer.getFboWidth()*0.05, true, true);
            }
            break;
       
        //change recording mode
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
