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
    gui.setup(); // most of the time you don't need a name
    gui.add(tWireframe.setup("wireframe", false));
    gui.add(tRotate.setup("rotate", true));
    gui.add(sScale.setup("scale", 0.5, 0., 1.0));
    gui.add(sResolution.setup("resolution", 80, 20, 95));
    //-
    gui.add(sNzAmp.setup("nzAmp", 100.0, 0.0, 500.0));
    gui.add(sNzRug.setup("nzRug", 100.0, 0.01, 500.0));
    gui.add(sNzFreq.setup("nzFreq", 2.0, 0.0, 8.0));
    //-
    gui.add(sTrHres.setup("TrHres", 100.0, 1.0, 500.0));
    gui.add(sTrWidth.setup("TrWidth", 600.0, 1.0, 1000.0));
    gui.add(sTrVspacing.setup("TrVspacing", 5.0, 1.0, 10.0));
    
    
    gui.add(s2.setup("s2", 0.5, 0., 1.0));
    gui.add(s3.setup("s3", 0.5, 0., 1.0));
    
    gui.add(b1.setup("b1"));
    gui.add(b2.setup("b2"));
  
    
    ofSetBackgroundColor(ofColor::black);

    float planeScale = 1.0;
    int planeGridSize = 10;
    int planeWidth = ofGetWidth() * planeScale;
    int planeHeight = ofGetHeight() * planeScale;
    int planeColums = planeWidth / planeGridSize;
    int planeRows = planeHeight / planeGridSize;
    
    plane.set(planeWidth, planeHeight, planeColums, planeRows, OF_PRIMITIVE_TRIANGLES);
    

}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    float planeScale = sScale;
    int planeGridSize = 100-sResolution;
    int planeWidth = ofGetWidth() * planeScale;
    int planeHeight = ofGetHeight() * planeScale;
    int planeColums = planeWidth / planeGridSize;
    int planeRows = planeHeight / planeGridSize;
    
    plane.set(planeWidth, planeHeight, planeColums, planeRows);
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    
   
    
    float percentX = mouseX / (float)ofGetWidth();
    percentX = ofClamp(percentX, 0, 1);

    ofSetColor(ofColor::white);
    
    
    ofPushMatrix();
    
    // translate plane into center screen.
    float tx = ofGetWidth() / 2;
    float ty = ofGetHeight() / 2;
    ofTranslate(tx, ty);
    
    if(tRotate){
        float percentY = mouseY / (float)ofGetHeight();
        float rotation = ofMap(percentY, 0, 1, -60, 60, true) + 60;
        ofRotate(rotation, 1, 0, 0);
    }
    
    shader.begin();
    shader.setUniform1f("uTime", ofGetElapsedTimef());
    //nz
    shader.setUniform1f("uDispAmp", sNzAmp);
    shader.setUniform1f("uDispRug", sNzRug);
    shader.setUniform1f("uDispFreq", sNzFreq);
    //tr
    shader.setUniform1f("uHres", sTrHres);
    shader.setUniform1f("uWidth", sTrWidth);
    shader.setUniform1f("uVspacing", sTrVspacing);
  
    
    if(tWireframe)plane.drawWireframe();
    else plane.drawVertices();
    
    shader.end();
    
    ofDrawAxis(200);
    
    ofPopMatrix();
    
    gui.draw();
    
    
    string info = "Verts Num: " + ofToString(plane.getMeshPtr()->getNumVertices()) +
                    "\nPlane size: " + ofToString(plane.getWidth()) + "x" + ofToString(plane.getHeight()) +
                    "\nWindoe size: "+ ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight());
    ofDrawBitmapString(info, 10, ofGetHeight()-40);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
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