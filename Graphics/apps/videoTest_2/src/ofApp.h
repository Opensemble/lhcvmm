#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "instancedManager.h"


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
    ofxFloatSlider sWidth, sHeight, sCubeSize, sHres,
                    sVres, sVelocity, sYpos;
    
    ofxFloatSlider sNzTime;
    ofxFloatSlider sNzXAmp, sNzXRug, sNzXFreq;
    ofxFloatSlider sNzYAmp, sNzYRug, sNzYFreq;
    ofxFloatSlider sNzZAmp, sNzZRug, sNzZFreq;
    
   

    ofxToggle tRotate;
    ofxButton b1, b2;
    
    ofxPanel gui;
    bool bShowGui = true;
    //----
    InstancedManager instanced;
    
};
