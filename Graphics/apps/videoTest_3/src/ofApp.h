#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "instancedManager.h"

#include "ParticleSystemPair.h"
#include "GuiManager.h"

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




class ofApp : public ofBaseApp{
	
public:
		
	void setup();
	void update();
	void draw();
   
    void drawScene(int w, int h);
    void drawDomeMask(int w, int h);
    
    void keyPressed(int key);
 
    void setupGui();
    void resetCamera();
    
    void updateInstanced();
    void updatePair();
    void updateSphere();
    
    //gui----
    bool bShowGuiInstanced;
    bool bShowGuiPair;
    bool bShowGuiCubeSphere;
    
    
    
    ofEasyCam cam;
    ofLight light;
    
    InstancedManager instanced;
    
    ofFbo drawFbo; //FBO for drawing scene, wihtout fisheye
   
    
    int fw, fh;
    
    ofMaterial materialPair;
    
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
    //instanced gui----------------
    ofxPanel guiInstanced;
    ofxToggle gMode, gRadMode;
    ofxFloatSlider gRadDeform;
    
    ofxFloatSlider gWidth, gHeight, gHres,
    gVres, gVelocity, gYpos, gXpos;
    
    ofxVec3Slider gCubesize;
    ofxFloatSlider gCubesizeUnified;
    
    ofxFloatSlider gMaskRadius;
    
    ofxFloatSlider gNzTime;
    ofxFloatSlider gNzXAmp, gNzXRug, gNzXFreq;
    ofxFloatSlider gNzYAmp, gNzYRug, gNzYFreq;
    ofxFloatSlider gNzZAmp, gNzZRug, gNzZFreq;
    
    ofxToggle gUseCam;
    ofxToggle gAxis;
    ofxToggle gUseLight;
    ofxVec3Slider gLightPos;
    
    //cubeSphere---------------------
    ofShader cubeShader;
    
    ofxPanel guiSphere;
    ofxToggle bDoShaderGui;
    ofxFloatSlider xShGui, yShGui;
    ofxFloatSlider volumeShGui;
    ofxFloatSlider velShGui;
    ofxFloatSlider sizeShGui;
    ofxIntSlider radiusShGui;
    
    //for cubeMesh
    float cubeX, cubeY;
    float cubeVol;
    float cubeSize;
    int cubeRadius;
    float cubeVel;
    
    //Cubes-sphere----------------
    ofSpherePrimitive sphereCubes;
    ofMesh cubeMesh;
    
    

};
