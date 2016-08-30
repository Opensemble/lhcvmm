#include "ofApp.h"

static ofxOscSender sender;


//--------------------------------------------------------------
void ofApp::setup(){
  ofBackground(0,0,0);

  string path = "data_files/";
  ofDirectory dir(path);
  //only show png files
  dir.allowExt("root");
  //populate the directory object
  dir.listDir();

  //go through and print out all the paths
  for(int i = 0; i < dir.size(); i++){
    dataFiles.push_back({dir.getName(i),dir.getAbsolutePath() + "/" + dir.getName(i)});
  }


  //receiver.setup(PORT);
  sender.setup(HOST, PORT);


  // Fixed framerate
  ofSetFrameRate(10);

  gui.setup();

  //adds gui
  for(int i=0;i< eventStreamsCount;i++){
    eventStreams[i].setup(dataFiles, &ofApp::sendMessage, i+1);
    gui.add(eventStreams[i].parameters);
  }

  for(int i=0;i< gui.getControlNames().size();i++){

    ofxGuiGroup* group = dynamic_cast<ofxGuiGroup*>(gui.getControl(i));
    if (group){
      bool enabled = group->getToggle("enabled");
      //ofLog(OF_LOG_NOTICE) << enabled;
      group->minimize();
    }

  }



}

//--------------------------------------------------------------
void ofApp::update(){

}




//--------------------------------------------------------------
void ofApp::processIncomingMessages(){

	// hide old messages
	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		if(timers[i] < ofGetElapsedTimef()){
			msg_strings[i] = "";
		}
	}

	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);

		// check for mouse moved message
		if(m.getAddress() == "/mouse/position"){
			// both the arguments are int32's
			mouseX = m.getArgAsInt32(0);
			mouseY = m.getArgAsInt32(1);
		}
		else{
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if(m.getArgType(i) == OFXOSC_TYPE_INT32){
					msg_string += ofToString(m.getArgAsInt32(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
					msg_string += ofToString(m.getArgAsFloat(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
					msg_string += m.getArgAsString(i);
				}
				else{
					msg_string += "unknown";
				}
			}
			// add to the list of strings to display
			msg_strings[current_msg_string] = msg_string;
			timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
			current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
			// clear the next line
			msg_strings[current_msg_string] = "";
		}

	}
}

void ofApp::sendMessage(string jsonMessage){

  ofxOscMessage m;
  m.setAddress("/raw_data");
  m.addStringArg(jsonMessage);
  sender.sendMessage(m, false);
}

//--------------------------------------------------------------
void ofApp::draw(){
  //ofDrawBitmapString("helllo you sexy bitch " + std::to_string(eventStreamsCount), 300, 300);
  gui.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
