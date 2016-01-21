#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    sphere.setScale(.5);
    sphere.setResolution(50);
    sphere.setOrientation(ofVec3f(90,0,0));

    box.setResolution(50);

    domemaster.setup();
    domemaster.resize(512,512);

    //ofSetFrameRate(FRAME_RATE);

    frameDuration = 1.0 / FRAME_RATE;
    framesMaxNumber = DURATION * FRAME_RATE;
    frameCounter = 0;
    isAnimating = true;
    isPlayingForward = true;

}

//--------------------------------------------------------------
void ofApp::update(){
  //box.setPosition(0,0,ofMap(x, 0, 1024, -250, 250));


  //display frame rate as window title
  ofSetWindowTitle(ofToString(ofGetFrameRate()));

  //animation data update
  if(isAnimating){
    if(isPlayingForward){
      frameCounter++;
      if (frameCounter>=framesMaxNumber)
          isPlayingForward = false;
    }else{
      frameCounter--;
      if (frameCounter<=0)
          isPlayingForward = true;
    }
    animationTime = frameCounter * frameDuration;
    animValue = animationTime/DURATION;
  }
  //-----------------------------------



}

//--------------------------------------------------------------
void ofApp::draw(){

    for (int i=0; i<domemaster.renderCount; i++){
        domemaster.begin(i);
        drawScene(i,domemaster.width, domemaster.height);
        domemaster.end(i);
    }

    domemaster.draw();
    domemaster.drawMask();

    ofPushStyle();
    ofSetColor(ofColor::red);
    ofNoFill();
    ofSetLineWidth( 512*0.004);
    ofDrawCircle(512*.5, 512*.5, 512*.5);
    ofPopStyle();


}

//--------------------------------------------------------------
void ofApp::drawScene(int i , int width, int height){
  switch (i) {
    case 0:
      //bottom
      ofBackground(ofColor::cyan);
      break;
    case 1:
      //front
      ofBackground(ofColor::blue);
      break;
    case 2:
      //left
      ofBackground(ofColor::magenta);
      break;
    case 3:
      //right
      ofBackground(ofColor::green);
      break;
    case 4:
      //top
      ofBackground(ofColor::orange);
      break;
  }


  ofSetColor(ofColor::white);
  //box.drawWireframe();

  ofSetColor(ofColor::yellow);
  //sphere.drawWireframe();

  ofSetColor(ofColor::orange);
  ofSpherePrimitive s3;
  s3.setResolution(10);
  s3.setPosition(ofMap(animValue, 0, 1, -200, 200)/sqrt(2), ofMap(animValue, 0, 1, -200, 200)/sqrt(2), ofMap(animValue, 0, 1, -200, 200)/sqrt(2));
  s3.drawWireframe();




  s3.setPosition(ofMap(animValue, 0, 1, -200, 200), 0, -200);
  s3.drawWireframe();


  float angle = ofDegToRad(animValue * 360);
  float radius = 200;
  float x = radius * cos(angle);
  float y = radius * sin(angle);
  float z = -x;

  s3.setPosition(x,y,z);
  s3.drawWireframe();




}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
  sphere.setPosition(0,0,ofMap(x, 0, 1024, -250, 250));
  //domemaster.setCameraPosition(0,ofMap(x, 0, 1024, -250, 250), ofMap(y, 0, 1024, -250, 250));

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
