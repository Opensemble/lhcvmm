//
//  GuiManager.h
//  videoTest_2
//
//  Created by Leo on 4/13/16.
//
//

#ifndef GuiManager_h
#define GuiManager_h

#include "ofMain.h"
#include "ofxGui.h"

class GuiManager {

public:
    
    void setup();
    void draw();
    
    //gui
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
    
    //ofxButton b1;
    
    ofxPanel gui;


};


#endif /* GuiManager_hpp */
