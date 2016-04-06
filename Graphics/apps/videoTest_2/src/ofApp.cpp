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
		shader.load("shadersGL3/shader");
	}else{
        ofLogVerbose()<<"NOT Using ProgrammableRenderer";
		shader.load("shadersGL2/shader");
	}
#endif
    
    //gui-------------
    gui.setup();
    gui.add(sWidth.setup("width", 1.0, 0., 1.0));
    gui.add(sHeight.setup("height", 0.25, 0., 1.0));
    gui.add(sCubeSize.setup("cubesize", 0.5, 0., 1.0));
    gui.add(sHres.setup("Hres", 0.3, 0., 1.0));
    gui.add(sVres.setup("Vres", 0.3, 0., 1.0));
    gui.add(sVelocity.setup("velocity", 0.0, 0., 1.0));
    gui.add(sYpos.setup("Ypos", 0.5, 0., 1.0));
    //nz
    gui.add(tRotate.setup("rotate", false));
    //-
    gui.add(sNzTime.setup("nzTime", 1.0, 0.0, 50.0));
    
    gui.add(sNzXAmp.setup("nzXAmp", 0.0, 0.0, 500.0));
    gui.add(sNzXFreq.setup("nzXFreq", 0.05, 0.0, 0.1));
    gui.add(sNzXRug.setup("nzXRug", 2.0, 0.01, 30.0));
    
    gui.add(sNzYAmp.setup("nzYAmp", 0.0, 0.0, 500.0));
    gui.add(sNzYFreq.setup("nzYFreq", 0.05, 0.0, 0.1));
    gui.add(sNzYRug.setup("nzYRug", 2.0, 0.01, 30.0));
    
    gui.add(sNzZAmp.setup("nzZAmp", 0.0, 0.0, 500.0));
    gui.add(sNzZFreq.setup("nzZFreq", 0.05, 0.0, 0.1));
    gui.add(sNzZRug.setup("nzZRug", 2.0, 0.01, 30.0));
    
    //
    
    gui.add(b1.setup("b1"));
    gui.add(b2.setup("b2"));
  
    
    ofSetBackgroundColor(ofColor::black);

    //plane----------
    float planeScale = 1.0;
    int planeGridSize = 10;
    int planeWidth = ofGetWidth() * planeScale;
    int planeHeight = ofGetHeight() * planeScale;
    int planeColums = planeWidth / planeGridSize;
    int planeRows = planeHeight / planeGridSize;
    
    plane.set(planeWidth, planeHeight, planeColums, planeRows, OF_PRIMITIVE_TRIANGLES);
    
    //instanced
   
    
    ofVec3f sceneLimits;
    sceneLimits.set(ofGetWidth(), ofGetHeight(), 100);
    
    instanced.setup();
    instanced.setLimits(sceneLimits);
    instanced.setOrientation(ofVec3f(1,1,1));
    instanced.setColor(ofColor::white);
    
   
    //instanced.setYPos(ofGetHeight()*.5);
    

}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    /*
    float planeScale = sScale;
    int planeGridSize = 100-sResolution;
    int planeWidth = ofGetWidth() * planeScale;
    int planeHeight = ofGetHeight() * planeScale;
    int planeColums = planeWidth / planeGridSize;
    int planeRows = planeHeight / planeGridSize;
    
    plane.set(planeWidth, planeHeight, planeColums, planeRows);
    */
    
    instanced.setWidth(sWidth);
    instanced.setHeight(sHeight);
    instanced.setCubeSize(sCubeSize*10);
    instanced.setHres(sHres*200);
    instanced.setVres(sVres*100);
    instanced.setVelocity(sVelocity*10);
    instanced.setYPos(sYpos);
    //nz
    instanced.setNzTime(sNzTime);
    
    instanced.setXnzAmp(sNzXAmp);
    instanced.setXnzFreq(sNzXFreq);
    instanced.setXnzRug(sNzXRug);
    instanced.setYnzAmp(sNzYAmp);
    
    instanced.setYnzFreq(sNzYFreq);
    instanced.setYnzRug(sNzYRug);
    
    instanced.setZnzAmp(sNzZAmp);
    instanced.setZnzFreq(sNzZFreq);
    instanced.setZnzRug(sNzZRug);
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
   
    
    float percentX = mouseX / (float)ofGetWidth();
    percentX = ofClamp(percentX, 0, 1);

    ofSetColor(ofColor::white);
    
    
    ofPushMatrix();
    
    // translate plane into center screen.
//    float tx = ofGetWidth() / 2;
//    float ty = ofGetHeight() / 2;
//    ofTranslate(tx, ty);
    
    if(tRotate){
        float percentY = mouseY / (float)ofGetHeight();
        float rotation = ofMap(percentY, 0, 1, -60, 60, true) + 60;
        ofRotate(rotation, 1, 0, 0);
    }

    ///Plane
//    shader.begin();
//    shader.setUniform1f("uTime", ofGetElapsedTimef());
//    //nz
//    shader.setUniform1f("uDispAmp", sNzAmp);
//    shader.setUniform1f("uDispRug", sNzRug);
//    shader.setUniform1f("uDispFreq", sNzFreq);
//    //tr
//    shader.setUniform1f("uHres", sTrHres);
//    shader.setUniform1f("uWidth", sTrWidth);
//    shader.setUniform1f("uVspacing", sTrVspacing);
//  
//    
//    if(tWireframe)plane.drawWireframe();
//    else plane.drawVertices();
//    
//    shader.end();
    
    ///Instanced
   
    
   
    instanced.draw();
    
    ofDrawAxis(200);
    
    ofPopMatrix();
    
    if (bShowGui) gui.draw();
    
    
    string info = "Verts Num: " + ofToString(plane.getMeshPtr()->getNumVertices()) +
                    "\nPlane size: " + ofToString(plane.getWidth()) + "x" + ofToString(plane.getHeight()) +
                    "\nWindoe size: "+ ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight());
    ofDrawBitmapString(info, 10, ofGetHeight()-40);
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