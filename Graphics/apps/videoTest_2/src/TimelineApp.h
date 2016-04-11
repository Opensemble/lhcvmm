#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "ofxTimeline.h"

class TimelineApp: public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	//ofParameterGroup parameters;
	//ofParameter<float> radius;
	//ofParameter<ofColor> color;
	//ofxPanel gui;
    
    ofxTimeline timeline;
};

