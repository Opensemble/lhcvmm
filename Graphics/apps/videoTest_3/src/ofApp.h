#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "PostProcessingManager.h"

#include "instancedManager.h"
#include "SphereManager.h"

#include "ParticleSystemPair.h"
#include "GuiManager.h"

#include "ofxOsc.h"
#include "ofxXmlSettings.h"

#include "ofxFisheye.h"
#include "VideoRenderer.h"


// listen on port 12345
#define PORT 12345

#define FRAME_RATE 30

//--------------------------------
//COLORS
#define COLOR_SPHERE    ofColor::white
#define COLOR_INSTANCED ofColor::orange
#define COLOR_PARTICLES ofColor::orange

#define MAX_LIGHT_X 1024
#define MAX_LIGHT_Y 800
#define MAX_LIGHT_Z 800
//-------------------------------
#define MTR_NAME_POWER "POWER"
#define MTR_NAME_PITCH_FREQ "FREQ"
#define MTR_NAME_PITCH_CONF "CONFID"
#define MTR_NAME_PITCH_SALIENCE "SALIENCE"
#define MTR_NAME_HFC "HFC"
#define MTR_NAME_CENTROID "CENTROID"
#define MTR_NAME_SPEC_COMP "SPEC-COMP"
#define MTR_NAME_INHARMONICTY "INHARM"
#define MTR_NAME_SPECTRUM "SPECTRUM"
#define MTR_NAME_MEL_BANDS "MEL-BANDS"
#define MTR_NAME_MFCC "MFCC"
#define MTR_NAME_HPCP "HPCP"
#define MTR_NAME_ONSETS "ONSETS"
#define MTR_SMOOTHING "SMOOTH"
#define MTR_ON_OFF "ON"
//--------------------------------

class ofApp : public ofBaseApp{
	
public:
		
	void setup();
	void update();
	void draw();
   
    void updatePair();
    void updateSphere();
    void updateInstanced();
    
    void drawFboInstanced();
    void drawFboParticles();
    void drawFboSphere();
    void drawFboMain();
    void drawFboPost();
    
    void drawDomeLimits(int w, int h);
    
    void keyPressed(int key);
 
    void setupGui();
    void resetCamera();
    
    void receiveOsc();
    void updateOscFromDataFile(int frameNum);
    
    void triggerOnset();
    
    //---------------------
    void startAnimation();
    void stopAnimation();

    
    //gui----
    bool bShowGuiInstanced;
    bool bShowGuiPair;
    bool bShowGuiCubeSphere;
    
    
    
    ofEasyCam cam;
    ofLight light;
    
    InstancedManager instanced;
    
    ofFbo drawFbo; //FBO for drawing scene, wihtout fisheye
   
    
    int fw, fh;
    
    
    //pair particles
    ParticleSystemPair pair;
    GuiManager guiPair;
    //parameters to change in scenes
    float minVelX, maxVelX;
    float minRadius;
    float maxRadius;//radiusInit, radiusVar
    float maxPartSize;
    float maxAngleVar;
    
private:
    
    ofxOscReceiver receiver;
    
    float oscPower;
    float oscFreq;
    float oscConfidence;
    float oscSalience;
    float oscHfc;
    float oscCentroid;
    float oscSpecComp;
    float oscInharm;
    bool  oscOnset; //onset from reseiver
    float oscTLtrack;
    
    bool isReallyOnset;
    //-----------------------
    
    ofVec3f _center;
    
    //-----------------
    ofFbo fboInstanced;
    ofFbo fboParticles;
    ofFbo fboSphere;
    
    ofFbo fboPost;
    
    //-------------------
    PostProcessingManager postManager;

    ofxPanel guiMain;
    ofxToggle gDoPostProcessing;
    ofxToggle gReceiveOSC;
    ofxToggle gDoDrawInstanced;
    ofxToggle gDoDrawSphere;
    ofxToggle gDoDrawParts;
    ofxToggle gDoDrawDomeLimits;
    ofxVec3Slider gLightPos;
    ofxFloatSlider gFisheye;
    
    ofxToggle gUseCam;
    ofxToggle gAxis;
    ofxToggle gUseLight;

    SphereManager sphere;

    //----------------------
    
    ofxXmlSettings data;
    
    //renderer-----------------
    //commo render vars****************************************
    
    //animation data variables----------------
    bool    isAnimating;
    int     frameCounter;//animation Frame Counter
    //float   frameDuration;//Duration in seconds of each frame
    int     framesMaxNumber;//Number of frames of the entire animation
    float   animValue;//Current frame in relationship with the duration of the entire animation (0.0 - 1.0)
    float   animationTime;
    
    int lastFrameWithOnset;
    //----------------------------
    ofxFisheye fisheye;
    float fisheyeAmount;
    
    VideoRenderer renderer;
    
    //ofFbo drawFbo; //FBO for drawing scene, wihtout fisheye
    
    ofTrueTypeFont	verdana;

};
