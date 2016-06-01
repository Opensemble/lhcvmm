#pragma once

#include "ofMain.h"
#include "ofxPostProcessing.h"
#include "ofxGui.h"

class PostProcessingManager{

public:
    
    void setup(int w, int h);
    
    void updateValues();
    
    void drawGui(int x, int y);
    void setupGui();
    
    void begin();
    void begin(ofCamera& cam);
    void begin(ofCamera& cam, ofRectangle viewport);
    
    
    void end();
    

    
    
private:
    
    ofxPostProcessing post;
    DofPass::Ptr dof;
    GodRaysPass::Ptr godRays;
    LimbDarkeningPass::Ptr limbDarkening;
    SSAOPass::Ptr ssao;
    
    //gui------------------------------------------
    
    ofxPanel gui;
    
    ofxToggle gDoFxaa;
    ofxToggle gDoBloom;
    ofxToggle gDoDof;
    ofxToggle gDoKaleidoscope;
    ofxToggle gDoNoiseWarp;
    ofxToggle gDoPixelate;
    ofxToggle gDoEdgePass;
    ofxToggle gDoVerticalTiltShift;
    ofxToggle gDoGodRays;
    ofxToggle gDoLimbDarkening;
    ofxToggle gDoSsao;
    
    ofxFloatSlider gDofAperture, gDofMaxBlur, gDofFocus;
    ofxFloatSlider gGodRaysLightDotView;
    ofxFloatSlider gSsaoAoClamp, gSsaoLumInfluence;
};