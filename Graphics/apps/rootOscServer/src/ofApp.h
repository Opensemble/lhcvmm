#pragma once

#include "ofMain.h"
#include <TFile.h>
#include <TTree.h>
#include "ofxGui.h"
#include "ofxOsc.h"
#include "EventStream.h"
#include "OscMultiClient.h"


class ofApp : public ofBaseApp{
	private:
		static const int eventStreamsCount = 8;

		ofxPanel gui;
		EventStream eventStreams[eventStreamsCount];
        OscMultiClient oscMultiClient;

		// data files
		vector<Datafile> dataFiles;

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
