#pragma once

#include "ofMain.h"
#include "ofxGui.h"


class ofApp : public ofBaseApp{
	public:
		
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

    ofShader shader;
    ofPlanePrimitive plane;
    
    //gui
    ofxFloatSlider sScale, s2, s3;
    ofxFloatSlider sNzAmp, sNzRug, sNzFreq;
    ofxFloatSlider sTrHres, sTrWidth, sTrVspacing;
    ofxIntSlider sResolution;
    ofxToggle tWireframe;
    ofxToggle tRotate;
    ofxButton b1, b2;
    
    ofxPanel gui;
};
