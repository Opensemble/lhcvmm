
#include "GuiManager.h"

void GuiManager::setup(){
    
    int _w = ofGetHeight();
    
    gui.setup("partSystem");
    gui.add(gDistTreshold.setup("dist-treshold", 300, 0, 1000));
    gui.add(gPartsNum.setup("groupPartsNum", 3, 2, 5));
    gui.add(gXvelocity.setup("x-velocity", 150.0, 0.0, 500.0));
    gui.add(gRadiusInit.setup("radius-init", 50, 1, 300));
    gui.add(gRadiusVar.setup("radius-var", 0, 0, 200));
    gui.add(gAngleInit.setup("angle-init", 1.0, 0.1, 5.0));
    gui.add(gAngleVar.setup("angle-var", 0.0, 0.0, 0.2));
    //nz
    gui.add(gNzAngleAmp.setup("nz-Angle-Amp", 1.0, 0.0, 2.0));
    gui.add(gNzAngleFreq.setup("nz-Angle-Freq", 0.001, 0.0, 0.01));
    gui.add(gNzRadAmp.setup("nz-Radius-Amp", 1.0, 0.0, 5.0));
    gui.add(gNzRadFreq.setup("nz-Radius-Freq", 1.0, 0.0, 10.0));
    gui.add(gNzXposAmp.setup("nz-Xpos-Amp", 200.0, 0.0, _w));
    gui.add(gNzXposFreq.setup("nz-Xpos-Freq", 0.05, 0.0, 0.1));

}
//-----------------------------------
void GuiManager::update(){

    guiData[KEY_DIST_TRESHOLD] = gDistTreshold;
    guiData[KEY_PARTS_NUM]     = gPartsNum;
    guiData[KEY_X_VELOCITY]    = gXvelocity;
    guiData[KEY_RADIUS_INIT]   = gRadiusInit;
    guiData[KEY_RADIUS_VAR]    = gRadiusVar;
    guiData[KEY_ANGLE_INIT]    = gAngleInit;
    guiData[KEY_ANGLE_VAR]     = gAngleVar;
    //nz
    guiData[KEY_ANGLE_NZ_AMP]  = gNzAngleAmp;
    guiData[KEY_ANGLE_NZ_FREQ] = gNzAngleFreq;
    guiData[KEY_RADIUS_NZ_AMP] = gNzRadAmp;
    guiData[KEY_RADIUS_NZ_FREQ]= gNzRadFreq;
    guiData[KEY_X_NZ_AMP]      = gNzXposAmp;
    guiData[KEY_X_NZ_FREQ]     = gNzXposFreq;
    guiData[KEY_PART_SIZE]     = 10;

}
//----------------------------------
void GuiManager::draw(){

    gui.draw();
    
}