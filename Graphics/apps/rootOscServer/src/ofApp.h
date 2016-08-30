#pragma once

#include "ofMain.h"
#include <TFile.h>
#include <TTree.h>
#include "ofxGui.h"
#include "ofxOsc.h"
#include "EventStream.h"

#define HOST "localhost"
#define PORT 57120
#define NUM_MSG_STRINGS 20

class ofApp : public ofBaseApp{
	private:
		static const int eventStreamsCount = 8;

		ofxPanel gui;
		EventStream eventStreams[eventStreamsCount];
		ofxOscReceiver receiver;

		int current_msg_string;
		string msg_strings[NUM_MSG_STRINGS];
		float timers[NUM_MSG_STRINGS];

		// data files
		vector<Datafile> dataFiles;

		void processIncomingMessages();
		static void sendMessage(string);


	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

};
