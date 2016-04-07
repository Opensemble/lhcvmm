#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "instancedManager.h"

#define MAX_CUBESIZE 10
#define MAX_H_RES 200
#define MAX_V_RES 100
#define MAX_VELOCITY 10

#define MAX_NZ_TIME 50

#define MAX_NZ_AMP 500
#define MAX_NZ_FREQ 0.1
#define MAX_NZ_RUG 30

#define MAX_LIGHT_X 1024
#define MAX_LIGHT_Y 800
#define MAX_LIGHT_Z 800


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

    
    //gui
    ofxToggle tMode, tRadMode;
    ofxFloatSlider sRadDeform;
    
    ofxFloatSlider sWidth, sHeight, sCubeSize, sHres,
                    sVres, sVelocity, sYpos, sXpos;
    
    ofxFloatSlider sNzTime;
    ofxFloatSlider sNzXAmp, sNzXRug, sNzXFreq;
    ofxFloatSlider sNzYAmp, sNzYRug, sNzYFreq;
    ofxFloatSlider sNzZAmp, sNzZRug, sNzZFreq;
    
    ofxToggle tUseCam;
    ofxToggle tAxis;
    ofxToggle tUseLight;
    ofxVec3Slider sLightPos;
    //ofxButton b1;
    
    ofxPanel gui;
    bool bShowGui = true;
    
    ofEasyCam cam;
    ofLight light;
    //----
    InstancedManager instanced;
    
    
    
};
