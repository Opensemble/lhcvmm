#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "ofxTimeline.h"

#include "Constants.h"

class TimelineApp: public ofBaseApp {
public:
	void setup();
	void draw();

    ofxTimeline timeline;
};

