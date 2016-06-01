#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "PostProcessingManager.h"

#include "instancedManager.h"

#include "ParticleSystemPair.h"
#include "GuiManager.h"

#include "DisplacementSphereMesh.h"

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
    
    void drawFboInstanced();
    void drawFboParticles();
    void drawFboSphere();
    void drawFboMain();
    void drawFboPost();
    
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
    gVres, gVelocity, gYpos, gXpos, gZpos;
    
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
    
    
    ofxFloatSlider velGui;
    ofxFloatSlider volumeGui;
    ofxFloatSlider xGui, yGui;
    ofxFloatSlider radiusGui;
    ofxIntSlider resolGui;
    ofxFloatSlider faceNoiseGui;
    ofxFloatSlider strengthGui;
    ofxFloatSlider zPos;

    //Cubes-sphere----------------
    ofSpherePrimitive sphereCubes;
    ofMesh cubeMesh;
    //Displacement-sphere-------
    DisplacementSphereMesh displacement;
    int dispResolution;
    vector<ofMeshFace> triangles;
    ofSpherePrimitive sphereDistor;
    float dispNzAmnt;
    bool bDoFaceSh, bDoFaces;
    ofShader faceShader;
    ofShader phongShader;
    
    ofVec3f _center;
    
    //-----------------
    ofFbo fboInstanced;
    ofFbo fboParticles;
    ofFbo fboSphere;
    
    ofFbo fboPost;
    
    //-------------------
    PostProcessingManager postManager;

    

};
