#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetBackgroundColor(50);
    ofSetFrameRate(FRAME_RATE);

    sphere.setScale(.15);
    sphere.setResolution(50);
    sphere.setOrientation(ofVec3f(90,0,0));

    box.setResolution(50);

    domemaster.setup();
    domemaster.resize(512,512);
    domemaster.setMeshScale(0.66f);
    sceneFbo.allocate(512,512);

    //ofSetFrameRate(FRAME_RATE);

    frameDuration = 1.0 / FRAME_RATE;
    framesMaxNumber = DURATION * FRAME_RATE;
    frameCounter = 0;
    isAnimating = true;
    isPlayingForward = true;

}

//--------------------------------------------------------------
void ofApp::update(){


  //display frame rate as window title
  //ofSetWindowTitle(ofToString(ofGetFrameRate()));

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

  sceneFbo.begin();
  ofClear(0);
  for (int i=0; i<domemaster.renderCount; i++){
      domemaster.begin(i);
      drawScene(i,domemaster.width, domemaster.height);
      domemaster.end(i);
  }
  domemaster.draw();
  if(showMask){
    domemaster.drawMask();
  }
  sceneFbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){

    sceneFbo.draw(250,0,512,512);


    //Display Key commands-----------------
    ofPushStyle();
    string keys = "KEY COMMANDS:";
    keys += "\nS: increase mesh scale"
    "\ns: decrease mesh scale"
    "\nm: toggle mask"
    "\nc: toggle cubemap";
    ofSetColor(ofColor::white);
    ofDrawBitmapString(keys, 10, 20);
    ofPopStyle();

    //Display Info-----------------
    ofPushStyle();
    string info = "INFO: ";
    info += "\nfps: "+ofToString(ofGetFrameRate())
    + "\nmesh scale: " + ofToString(meshScale);
    ofSetColor(ofColor::yellow);
    ofDrawBitmapString(info, 10, ofGetHeight()-100);
    ofPopStyle();

}

//--------------------------------------------------------------
void ofApp::drawScene(int i , int width, int height){

  if(showCubemap){
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
  }

  ofSetColor(ofColor::orange);
  ofSpherePrimitive s3;
  s3.setResolution(10);
  s3.setPosition(ofMap(animValue, 0, 1, -180, 180), ofMap(animValue, 0, 1, -180, 180), -animValue*180);
  s3.drawWireframe();

  s3.setPosition(ofMap(animValue, 0, 1, -180, 180), 0, 0);
  s3.drawWireframe();

  float angle = ofDegToRad(animValue * 360);
  float radius = 1000;
  float x = radius * cos(angle);
  float y = radius * sin(angle);
  float z = -animValue * 2000;

  s3.setPosition(x,y,z);
  s3.draw();

  s3.setPosition(x,y,0);
  s3.drawWireframe();



}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch (key) {

        case 's':
            meshScale -= 0.01;
            domemaster.setMeshScale(meshScale);
            break;
        case 'S':
            meshScale += 0.01;
            domemaster.setMeshScale(meshScale);
            break;
        case 'm':
            showMask = !showMask;
            break;
        case 'c':
            showCubemap = !showCubemap;
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
