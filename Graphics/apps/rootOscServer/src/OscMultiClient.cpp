//
//  OscMultiClient.cpp
//  rootOscServer
//
//  Created by Diego Dorado on 8/30/16.
//
//

#include "OscMultiClient.h"

const int OscMultiClient::RECEIVE_PORT = 50000;

void OscMultiClient::setup(){
    receiver.setup(OscMultiClient::RECEIVE_PORT);
}

void OscMultiClient::update(){
    processIncomingMessages();
}


void OscMultiClient::sendMessage(string jsonMessage){
    
    ofxOscMessage m;
    m.setAddress("/raw_data");
    m.addStringArg(jsonMessage);
    
    for(int i = 0; i< senders.size();i++)
        senders[i].sendMessage(m, false);
}


//--------------------------------------------------------------
void OscMultiClient::processIncomingMessages(){
    
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);
        
		// check for mouse moved message
		if(m.getAddress() == "/subscribe"){
            Client c;
            c.hostname = m.getRemoteIp();
            c.port = m.getRemotePort();
            clients.push_back(c);
            ofxOscSender sender;
            sender.setup(c.hostname, c.port);
            senders.push_back(sender);
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

		}
        
	}
}
