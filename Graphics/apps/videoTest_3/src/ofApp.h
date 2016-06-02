#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "PostProcessingManager.h"

#include "instancedManager.h"
#include "SphereManager.h"

#include "ParticleSystemPair.h"
#include "GuiManager.h"

//--------------------------------
//COLORS
#define COLOR_SPHERE    ofColor::red
#define COLOR_INSTANCED ofColor::yellow
#define COLOR_PARTICLES ofColor::green

#define MAX_LIGHT_X 1024
#define MAX_LIGHT_Y 800
#define MAX_LIGHT_Z 800

//--------------------------------

class ofApp : public ofBaseApp{
	
public:
		
	void setup();
	void update();
	void draw();
   
   
    
    void drawFboInstanced();
    void drawFboParticles();
    void drawFboSphere();
    void drawFboMain();
    void drawFboPost();
    
    void drawDomeLimits(int w, int h);
    
    void keyPressed(int key);
 
    void setupGui();
    void resetCamera();
    
    
    void updatePair();
    
    //gui----
    bool bShowGuiInstanced;
    bool bShowGuiPair;
    bool bShowGuiCubeSphere;
    
    
    
    ofEasyCam cam;
    ofLight light;
    
    InstancedManager instanced;
    
    ofFbo drawFbo; //FBO for drawing scene, wihtout fisheye
   
    
    int fw, fh;
    
    //ofMaterial materialPair;
    
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
    
    
  
    
    
    //cubeSphere---------------------
    

    
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
    
    ofxToggle gUseCam;
    ofxToggle gAxis;
    ofxToggle gUseLight;
    
    
    SphereManager sphere;

};
