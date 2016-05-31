#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "BosqueConstants.h"

class GuiManager{

 public:
    
    void setup();
    void update();
    void draw();
    
    std::map<string, float>& getData(){return guiData;}
    
    //gui
    ofxPanel gui;
    ofxFloatSlider gDistTreshold;
    ofxIntSlider gPartsNum;
    ofxFloatSlider gXvelocity;
    ofxFloatSlider gRadiusInit, gRadiusVar;
    ofxFloatSlider gAngleInit, gAngleVar;
    ofxFloatSlider gNzAngleAmp, gNzAngleFreq;
    ofxFloatSlider gNzRadAmp, gNzRadFreq;
    ofxFloatSlider gNzXposAmp, gNzXposFreq;
    
    std::map<string, float> guiData;
};
