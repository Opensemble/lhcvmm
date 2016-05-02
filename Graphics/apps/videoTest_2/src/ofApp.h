#pragma once

#include "ofMain.h"
#include "GuiManager.h"

#include "instancedManager.h"
#include "TimelineApp.h"

#include "ofMain.h"
#include "ofxFisheye.h"
#include "VideoRenderer.h"


//----------------

#include "Constants.h"

//---------------
#define MAX_CUBESIZE 0.02
#define MAX_H_RES 200
#define MAX_V_RES 100
#define MAX_VELOCITY 10

#define MAX_NZ_TIME 50

#define MAX_NZ_AMP 0.2
#define MAX_NZ_FREQ 0.1
#define MAX_NZ_RUG 0.03

#define MAX_LIGHT_X 1024
#define MAX_LIGHT_Y 800
#define MAX_LIGHT_Z 800

enum ValuesMode {
    GUI,
    TIMELINE
};


class ofApp : public ofBaseApp{
	public:
		
	void setup();
	void update();
	void draw();
    void exit();
    
    void keyPressed(int key);
    
    void drawScene(int w, int h);
    
    void startAnimation();
    void stopAnimation();

    void setValuesMode(ValuesMode val){currentValuesMode = val;}
    void updateInstancedValues();
    
    //commo render vars****************************************
    
    //animation data variables----------------
    bool    isAnimating;
    int     frameCounter;//animation Frame Counter
    float   frameDuration;//Duration in seconds of each frame
    int     framesMaxNumber;//Number of frames of the entire animation
    float   animValue;//Current frame in relationship with the duration of the entire animation (0.0 - 1.0)
    float   animationTime;
    
    //----------------------------
    ofxFisheye fisheye;
    float fisheyeAmount;
    
    VideoRenderer renderer;
    
    ofFbo drawFbo; //FBO for drawing scene, wihtout fisheye
    
    ofTrueTypeFont	verdana;
    
    //this App vars**************************************************
    
    GuiManager gm;
    bool bShowGui;
    
    ofEasyCam cam;
    ofLight light;
    
    ValuesMode currentValuesMode;
    
    InstancedManager instanced;
    
    shared_ptr<TimelineApp> timelineApp;
    
    

};
