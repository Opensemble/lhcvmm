//
//  GuiManager.h
//  videoTest_2
//
//  Created by Leo on 4/13/16.
//
//

#include "GuiManager.h"

void GuiManager::setup(){
    
    gui.setup();
    gui.add(gMode.setup("LINEAL/RADIAL", false));
    gui.add(gRadMode.setup("Concentric/Centrifuge", false));
    gui.add(gRadDeform.setup("Radial Mix", 0.0, 0.0, 1.0));
    
    
    gui.add(gWidth.setup("width", 1.0, 0., 1.0));
    gui.add(gHeight.setup("height/radius", 0.25, 0., 1.0));
    gui.add(gCubesizeUnified.setup("cubesize", 0.25, 0., 1.0));
    //gui.add(gCubesize.setup("cubesize", ofVec3f(0.2), ofVec3f(0.0), ofVec3f(1.0)));
    gui.add(gMaskRadius.setup("maskRadius", 0.0, 0.0, 1.0));
    gui.add(gHres.setup("Hres", 0.3, 0., 1.0));
    gui.add(gVres.setup("Vres", 0.3, 0., 1.0));
    gui.add(gXpos.setup("Xpos", 0.5, 0., 1.0));
    gui.add(gYpos.setup("Ypos", 0.5, 0., 1.0));
    gui.add(gVelocity.setup("velocity", 0.0, 0., 1.0));
    //nz
    gui.add(gNzTime.setup("nzTime", 0.1, 0.0, 1.0));
    
    gui.add(gNzXAmp.setup("nzXAmp", 0.0, 0.0, 1.0));
    gui.add(gNzXFreq.setup("nzXFreq", 0.5, 0.0, 1.0));
    gui.add(gNzXRug.setup("nzXRug", 0.05, 0.01, 1.0));
    
    gui.add(gNzYAmp.setup("nzYAmp", 0.0, 0.0, 1.0));
    gui.add(gNzYFreq.setup("nzYFreq", 0.5, 0.0, 1.0));
    gui.add(gNzYRug.setup("nzYRug", 2.0, 0.01, 30.0));
    
    gui.add(gNzZAmp.setup("nzZAmp", 0.0, 0.0, 1.0));
    gui.add(gNzZFreq.setup("nzZFreq", 0.5, 0.0, 1.0));
    gui.add(gNzZRug.setup("nzZRug", 0.05, 0.01, 1.0));
    //
    gui.add(gUseCam.setup("useCam", false));
    gui.add(gAxis.setup("axis", false));
    gui.add(gUseLight.setup("useLight", false));
    gui.add(gLightPos.setup("LighPos", ofVec3f(0.5), ofVec3f(0.0), ofVec3f(1.0)));
}
//-------------------------
void GuiManager::draw(){
    gui.draw();
}