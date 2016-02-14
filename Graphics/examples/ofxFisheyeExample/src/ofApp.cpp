#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(34);
    
    w = ofGetWidth();
    h = ofGetHeight();

#ifdef TARGET_OPENGLES
	shader.load("shaders_gles/noise.vert","shaders_gles/noise.frag");
#else
	if(ofIsGLProgrammableRenderer()){
		shader.load("shaders_gl3/noise.vert", "shaders_gl3/noise.frag");
	}else{
		shader.load("shaders/noise.vert", "shaders/noise.frag");
	}
#endif
    
    fbo.allocate(w, h);

    fisheye.setup(tFixFisheye);

	doShader = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    static float lastElapsedTime=0;
    static float growingRadius=0;
    float cell = w/16.;
    float deltaTime = ofGetElapsedTimef() - lastElapsedTime;
    lastElapsedTime += deltaTime;

    fbo.begin();
    ofClear(255);
    ofSetColor(ofColor::white);
    ofPlanePrimitive plane;
    plane.set(w, h, 16+1, 16+1);
    plane.setPosition(w*.5, h*.5, 0);
    plane.drawWireframe();
    
    ofTranslate(w*0.5, h*0.5, 10);
    
    ofSetColor(ofColor::cyan);
    ofNoFill();
    ofSetLineWidth(3);

    growingRadius += deltaTime * cell * 2 * 0.5;
    if (growingRadius> cell*2)
        growingRadius = 0;
    for(int i=0;i<4;i++){
        float r = i*cell*2 + growingRadius;
        ofDrawCircle(0, 0, r);
        //ofDrawRectangle(-r,-r,r*2,r*2);
    }
    
    fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    float amount = 1;//ofClamp(ofGetMouseX() / (float)ofGetWidth(), 0.0, 1.0);//0.0-1.0
    
	if( doShader ){
        fisheye.begin(fbo.getTexture(), fbo.getWidth(), fbo.getHeight(), amount);
    }
	
    fbo.draw(0, 0, h, w);
		
	if( doShader ){
		fisheye.end();
	}
    
    //key commands----------
    string keys = "'s': toggles shader\n'1': Fisheye A shader\n'2': Fisheye B shader\n'3': Barrel Distortion shader\nMouse X: fx amount";
    ofDrawBitmapStringHighlight(keys, 10, 20);
    
    string info = "fx amount: " + ofToString(amount) +
    "\nCurrent FX type: " + fisheye.getFxTypeAsString();
    ofDrawBitmapStringHighlight(info, 10, ofGetHeight()-50);
    
    
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

