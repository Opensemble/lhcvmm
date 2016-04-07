#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //openGl and GLSL info------
    ofLogVerbose()<<"GLVersionMajor: "<< ofGetGLRenderer()->getGLVersionMajor();
    ofLogVerbose()<<"GLVersionMinor: "<< ofGetGLRenderer()->getGLVersionMinor();
    ofLogVerbose()<<"GLSL Version: "<< ofGLSLVersionFromGL(ofGetGLRenderer()->getGLVersionMajor(), ofGetGLRenderer()->getGLVersionMinor());
    
    
    
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
    gui.setup();
    gui.add(tMode.setup("LINEAL/RADIAL", false));
    gui.add(tRadMode.setup("Concentric/Centrifuge", false));
    gui.add(sRadDeform.setup("Radial Mix", 0.0, 0.0, 1.0));
    
    
    gui.add(sWidth.setup("width", 1.0, 0., 1.0));
    gui.add(sHeight.setup("height/radius", 0.25, 0., 1.0));
    gui.add(sCubeSize.setup("cubesize", 0.5, 0., 1.0));
    gui.add(sHres.setup("Hres", 0.3, 0., 1.0));
    gui.add(sVres.setup("Vres", 0.3, 0., 1.0));
    gui.add(sXpos.setup("Xpos", 0.5, 0., 1.0));
    gui.add(sYpos.setup("Ypos", 0.5, 0., 1.0));
    gui.add(sVelocity.setup("velocity", 0.0, 0., 1.0));
    //nz
    gui.add(sNzTime.setup("nzTime", 0.1, 0.0, 1.0));
    
    gui.add(sNzXAmp.setup("nzXAmp", 0.0, 0.0, 1.0));
    gui.add(sNzXFreq.setup("nzXFreq", 0.5, 0.0, 1.0));
    gui.add(sNzXRug.setup("nzXRug", 0.05, 0.01, 1.0));
    
    gui.add(sNzYAmp.setup("nzYAmp", 0.0, 0.0, 1.0));
    gui.add(sNzYFreq.setup("nzYFreq", 0.5, 0.0, 1.0));
    gui.add(sNzYRug.setup("nzYRug", 2.0, 0.01, 30.0));
    
    gui.add(sNzZAmp.setup("nzZAmp", 0.0, 0.0, 1.0));
    gui.add(sNzZFreq.setup("nzZFreq", 0.5, 0.0, 1.0));
    gui.add(sNzZRug.setup("nzZRug", 0.05, 0.01, 1.0));
    //
    gui.add(tUseCam.setup("useCam", false));
    gui.add(tAxis.setup("axis", false));
    gui.add(tUseLight.setup("useLight", false));
    gui.add(sLightPos.setup("LighPos", ofVec3f(0.5), ofVec3f(0.0), ofVec3f(1.0)));
    
    //gui.add(b1.setup("b1"));
  
    ofSetBackgroundColor(ofColor::black);
    
    cam.lookAt(ofVec3f( ofGetWidth()*.5, ofGetHeight()*.5, 0.0));
    light.setPosition(ofGetWidth()*.5, ofGetHeight()*.5, 150.0);
    light.setPointLight();
   
   
    ofVec3f sceneLimits;
    sceneLimits.set(ofGetWidth(), ofGetHeight(), 100);
    
    instanced.setup();
    instanced.setLimits(sceneLimits);
    instanced.setOrientation(ofVec3f(1,1,1));
    instanced.setColor(ofColor::white);

}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    if(!tMode)instanced.setMode(LINEAL);
    else if(tMode && !tRadMode) instanced.setMode(RAD_CONCENTRIC);
    else if(tMode && tRadMode){
        instanced.setMode(RAD_CENTRIFUGE);
        instanced.setRadDeform(sRadDeform);
    }
    
    instanced.setWidth(sWidth);
    instanced.setHeight(sHeight);
    instanced.setCubeSize(sCubeSize * MAX_CUBESIZE);
    instanced.setHres(sHres * MAX_H_RES);
    instanced.setVres(sVres * MAX_V_RES);
    instanced.setVelocity(sVelocity * MAX_VELOCITY);
    instanced.setXpos(sXpos);
    instanced.setYPos(sYpos);
    //nz
    instanced.setNzTime(sNzTime * MAX_NZ_TIME);
    
    instanced.setXnzAmp(sNzXAmp * MAX_NZ_AMP);
    instanced.setXnzFreq(sNzXFreq * MAX_NZ_FREQ);
    instanced.setXnzRug(sNzXRug * MAX_NZ_RUG);
    
    instanced.setYnzAmp(sNzYAmp * MAX_NZ_AMP);
    instanced.setYnzFreq(sNzYFreq * MAX_NZ_FREQ);
    instanced.setYnzRug(sNzYRug * MAX_NZ_RUG);
    
    instanced.setZnzAmp(sNzZAmp * MAX_NZ_AMP);
    instanced.setZnzFreq(sNzZFreq * MAX_NZ_FREQ);
    instanced.setZnzRug(sNzZRug * MAX_NZ_RUG);
    
    //light pos
    
    light.setPosition(sLightPos->x * MAX_LIGHT_X,
                      sLightPos->y * MAX_LIGHT_Y,
                      sLightPos->z * MAX_LIGHT_Z );
  

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(tUseLight){
        ofEnableLighting();
        light.enable();
    }
    
    if(tUseCam)cam.begin();
    
    if(tAxis)ofDrawAxis(200);
    light.draw();
    
    instanced.draw();
    
    if(tUseCam)cam.end();
    
    if(tUseLight){
        light.disable();
        ofDisableLighting();
    }
    
    if (bShowGui) gui.draw();
    
    
    string info = "Window size: "+ ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight());
    ofDrawBitmapString(info, ofGetWidth()-60, ofGetHeight()-40);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key){
        case 'g':
            bShowGui = !bShowGui;
            break;
        default:
            break;
    
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}