#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(34);
    
    w = ofGetWidth();
    h = ofGetHeight();

    fbo.allocate(w, h);
    fbo.begin();
        ofClear(255);
        ofSetColor(ofColor::white);
        ofPlanePrimitive plane;
        plane.set(ofGetWidth(), ofGetHeight(), 20, 20);
        plane.setPosition(ofGetWidth()*.5, ofGetHeight()*.5, 0);
        plane.drawWireframe();
        ofSetColor(ofColor::yellow);
        ofDrawCircle(ofGetWidth()*0.5, ofGetHeight()*0.5, 10);
    fbo.end();

    fisheye.setup(tVariableFisheye);

	doShader = false;
}

//--------------------------------------------------------------
void ofApp::update(){

    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    float amount = ofClamp(ofGetMouseX() / (float)ofGetWidth(), 0.0, 1.0);//0.0-1.0
    
	if( doShader ){
        fisheye.begin(fbo.getTexture(), fbo.getWidth(), fbo.getHeight(), amount);
    }
	
    fbo.draw(0, 0, h, w);
		
	if( doShader ){
		fisheye.end();
	}
    
    //key commands----------
    ofPushStyle();
    ofSetColor(ofColor::orange);
    string keys = "'s': toggles shader\n'1': Fisheye A shader\n'2': Fisheye B shader\n'3': Barrel Distortion shader";
    ofDrawBitmapStringHighlight(keys, 10, 20);
    
    string info = "fx amount: " + ofToString(amount);
    ofDrawBitmapStringHighlight(info, 10, ofGetHeight()-30);
    
    ofPopStyle();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 

    switch (key) {
        case 's':
            doShader = !doShader;
            break;
        case '1':
            fisheye.setup(tFixFisheye);
            break;
        case '2':
            fisheye.setup(tVariableFisheye);
            break;
        case '3':
            fisheye.setup(tBarrelDist);
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

