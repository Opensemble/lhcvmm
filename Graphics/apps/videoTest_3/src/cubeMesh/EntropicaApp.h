#pragma once

#include "ofMain.h"
//Addons

#include "ofxGui.h"
#include "ofxMarchingCubes.h"
#include "ofxTimeMeasurements.h"
#include "ofxAnimatableFloat.h"


//Common classes
#include "DisplacementSphereMesh.h"
#include "BaseMeiApp.h"

#include "Colors.h"
#include "EnumsAndDefines.h"



//*********************************


class EntropicaApp : public BaseMeiApp{
    
public:
    
    ~EntropicaApp(){exit();}
    
    //OF Core funcs
    void setup(ofFbo* iFbo, OscManager* iOsc);
    void update();
    void exit();
    
    void keyPressed(int key);
    
    //Scene funcs
    void drawScene();
    void drawGui();
    
    //Gui funcs
    void setControlGui();
    
    //String funcs
    
    string getInfoString();
    string getHelpString();
    string getMarkerString();
    string getInfoForAnalyzerString();
    
    void setFboPtr(ofFbo* iFbo){fbo = iFbo;}
    void setOscPtr(OscManager* iOsc){osc = iOsc;}
    void setPostProcessing(bool val){bPostProcessing = val;}

    //Flow control
    void setScene(Mark iMarker, int iSubmarker);
    void setScene(int gralMarker);

    
    //Common funcs
    void setupPostProcessing(int w, int h);

    float trans(float fromVal, float toVal, float trans){
        return (1-trans)*fromVal + trans*toVal;
    }
    


    
    ofMaterial material;
    ofLight light;
    ofColor mainColor, backgroundColor;
    
    ofEasyCam eCam;
    ofCamera fxdCam;
    bool bUseFixCam;
    float camCounter;
    
    Mark _marker;
    int _subMarker;
    int _gralMarker;
    float _animDuration, _animCamDuration;
    ofxAnimatableFloat animValue, animValueCam;
    
    
    //Booleans---------------
    bool bShowGui;
    
    bool bPostProcessing;

    //bool bKeyCommands;
    ofxToggle bReceiveOSCGui, bDoShaderGui;
    bool bDoDisplacement, bDoMetaball, bDoMarching, bDoFaces,bDoNoiseShader, bDoCubeMesh, bDoFaceSh;

    //Shaders----------------
    ofShader noiseShader;
    ofShader faceShader;
    ofShader cubeShader;
    ofShader phongSahder;
    //Displacement-sphere-------
    DisplacementSphereMesh displacement;
    int dispResolution;
    vector<ofMeshFace> triangles;
    ofSpherePrimitive sphereDistor;
    //Cubes-sphere----------------
    ofSpherePrimitive sphereCubes;
    ofMesh cubeMesh;
    //False-Metaballs--------------
    float metaTr_1, metaTr_2;
    int metaScale_1, metaScale_2;
    vector<ofVec3f> centers_1, centers_2;
    //MarchingCubes--------------
    ofxMarchingCubes mc;
  
    
    
    //----------------------------------------

    
//    DofPass::Ptr dof;
//    GodRaysPass::Ptr godRays;
//    LimbDarkeningPass::Ptr limbDarkening;
//    SSAOPass::Ptr ssao;
    
    //syphon


    
    //GUI-----------------
	//ofxPanel gui;
   
    ofxToggle bDoDisplacementGui, bDoMetaballGui, bDoMarchingGui, bDoFacesGui, bDoCubeMeshGui, bDrawAxisGui;
    ofxFloatSlider velGui;
    ofxFloatSlider volumeGui;
    ofxFloatSlider xGui, yGui;
    ofxFloatSlider radiusGui;
    ofxIntSlider resolGui;
    ofxToggle bWireGui;
    ofxToggle bLitGui;
    ofxFloatSlider strengthGui, angleGui;
    ofxIntSlider cubeScaleGui;
    ofxFloatSlider minRadiusGui, maxRadiusGui;
    ofxFloatSlider metaVelGui, metaDistOrbGui;
    ofxFloatSlider marchNoise1Gui, marchNoise2Gui, marchStepGui;
    ofxIntSlider   marchRadiusGui;
    ofxFloatSlider faceNoiseGui;
    ofxLabel lDisp, lMeta, lMarch, lCubeShader;
    ofxFloatSlider xShGui, yShGui;
    ofxFloatSlider volumeShGui;
    ofxFloatSlider velShGui;
    ofxFloatSlider sizeShGui;
    ofxIntSlider radiusShGui;

    
    //MappingParameters-----------
    //for one object
    float power;
    float centroid;
    float specComp;
    float specCompB;
    float pitchConf;
    float pitchFreq;
    float confTresh;
    float hfc;
    //for split objects
    float specComp1, specComp2;
    float centroid1, centroid2;
    float power1, power2;
    float pitchFreq1, pitchFreq2;
    float pitchConf1, pitchConf2;
    float hfc1, hfc2;
    //for cubeMesh
    float cubeX, cubeY;
    float cubeVol;
    float cubeSize;
    int cubeRadius;
    float cubeVel;
    //
    float marchShX, marchShY;
    float marchShVol;
    float marchShSize;
    int marchRadius;
    float marchShVel;
    //-
    float dispNzAmnt;
    //-Meta
    float metaShX_1, metaShX_2;
    float metaShY_1, metaShY_2;
    float metaShVel_1, metaShVel_2;
    float metaShVol_1, metaShVol_2;
    //-
    float metaVel_1, metaVel_2;
    float minRadius, metaMaxRadius_1, metaMaxRadius_2;
    
    //syphonControl
    ofxOscReceiver receiverSyph;
    ofxOscSender senderSyph;
    string serversList;
    string clientInfo;
    
    //-------------------
    string boolToString (bool b){
        string s;
        if(b)s = "TRUE";
        else s = "false";
        return s;
    }
    
    string markerToString (Mark iMarker){
        string s;
        switch (iMarker) {
            case A:
                s="A";
                break;
            case B:
                s="B";
                break;
            case C:
                s="C";
                break;
            case D:
                s="D";
                break;
            case Z:
                s="Z";
                break;
                
            default:
                break;
        }
        return s;
    }

};
