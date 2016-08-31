#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    oscMultiClient.setup();
    
    ofBackground(0,0,0);
    // Fixed framerate, enough for this app
    ofSetFrameRate(20);
    
    string path = "data_files/";
    ofDirectory dir(path);
    //only show png files
    dir.allowExt("root");
    //populate the directory object
    dir.listDir();

    //break if no datafiles
    if(dir.size()==0)
        return;
    
    
    //ofLog(OF_LOG_NOTICE) << dir.size();

    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        dataFiles.push_back({dir.getName(i),dir.getAbsolutePath() + "/" + dir.getName(i)});
    }




    gui.setup();

    //adds gui
    for(int i=0;i< eventStreamsCount;i++){
        eventStreams[i].setup(&dataFiles, &oscMultiClient, i+1);
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
    oscMultiClient.update();
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    //warns if no dataset were found
    if(dataFiles.size()==0){
        string msg = "No dataset found on ./data_files folder.\n\n";
        msg +="Download it from:\n\n";
        msg +="https://tripiana.web.cern.ch/tripiana/openensemble/ATLAS_data/data_Egamma_A.root\n\n";
        msg +="And restart program.\n";
        ofDrawBitmapString(msg, 100, 100);
        
        //dont render anything else
        return;
    }
  
    //draws gui
    gui.draw();
    
    //draws connections visual feedback
    if(oscMultiClient.clients.size()==0){
        
        ofSetColor(200,0,0);
        string msg = "No clients connected.\n\n";
        msg +="Send a \"/subscribe\" OSC message to ";;
        msg +=ofToString(OscMultiClient::RECEIVE_PORT) + " port.\n\n";
        msg +="You will be receiving then \"/raw_data\" OSC messages.\n";
        ofDrawBitmapString(msg, 440, 50);
    }else{
        ofSetColor(0,200,0);
        string msg = "Clients connected:\n\n";
        for (int i= 0;i<oscMultiClient.clients.size();i++){
            msg += oscMultiClient.clients[i].hostname;
            msg += " on port "  + ofToString(oscMultiClient.clients[i].port) + "\n";
        }
        ofDrawBitmapString(msg, 440, 50);
    }
    
    //draws streams visual feedback
    for(int i=0;i< eventStreamsCount;i++){
        
        ofSetColor(200);
        ofDrawBitmapString("Stream #"+ofToString(eventStreams[i].channel), 250, 15+20 +i*30);
        if(eventStreams[i].enabled){
            ofDrawBitmapString(ofToString(eventStreams[i].currentEventIndex), 380, 15+20 +i*30);
         
            float color = ofMap(ofGetElapsedTimef(), eventStreams[i].currentEventTime, eventStreams[i].nextEventTime, 20.0f, 220.0f);

            ofSetColor(20, color, 20);

        }else{
            ofSetColor(150,0,0);
        }
        
        ofDrawRectangle(340, 20 +i*30, 20, 20);
        
        
    }

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
