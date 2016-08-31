//
//  OscMultiClient.h
//  rootOscServer
//
//  Created by Diego Dorado on 8/30/16.
//
//

#ifndef __rootOscServer__OscMultiClient__
#define __rootOscServer__OscMultiClient__

#include "ofxOsc.h"

#define NUM_MSG_STRINGS 20

struct Client{
    string hostname;
    int port;
};

class OscMultiClient {
    ofxOscReceiver receiver;
    vector<ofxOscSender> senders;
    
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    
    void processIncomingMessages();


    
public:
   
    void setup();
    void update();
    void sendMessage(string);
    int subscribers;
    string remoteIp;
    int remotePort;
    vector<Client> clients;
    static const int RECEIVE_PORT;

};


#endif /* defined(__rootOscServer__OscMultiClient__) */
