
#include "PostProcessingManager.h"

//TODO: add more efects and parameters
//TODO: add gui listener for update, not update everyframe


//---------------------------------------
void PostProcessingManager::setup(int w, int h){
    
    post.init(w, h);
    
    //0-Fxaa
    post.createPass<FxaaPass>()->setEnabled(false);
    //1-Bloom
    post.createPass<BloomPass>()->setEnabled(false);
    //2-dof
    dof = post.createPass<DofPass>();
    dof->setAperture(0.2);
    dof->setMaxBlur(0.05);
    dof->setEnabled(false);
    //3-Kaleidoscopes
    post.createPass<KaleidoscopePass>()->setEnabled(false);
    //4-Noisewarp
    post.createPass<NoiseWarpPass>()->setEnabled(false);
    //5-Pixelate
    post.createPass<PixelatePass>()->setEnabled(false);
    //6-EdgePass
    post.createPass<EdgePass>()->setEnabled(false);
    //7-VerticalTilt
    post.createPass<VerticalTiltShifPass>()->setEnabled(false);
    //8-GodRays
    godRays = post.createPass<GodRaysPass>();
    godRays->setLightDirDOTviewDir(0.3);
    godRays->setEnabled(false);
    //9-LimbDarkening
    limbDarkening=post.createPass<LimbDarkeningPass>();
    limbDarkening->setEnabled(false);
    //10-Ssao
    ssao=post.createPass<SSAOPass>();
    ssao->setAoClamp(0.65);
    ssao->setLumInfluence(0.25);
    ssao->setEnabled(false);
    
    //---------
    setupGui();
  

}
//---------------------------------------
void PostProcessingManager::updateValues(){
    
    //TODO: check pointers are not null
    
    post[0]->setEnabled(gDoFxaa);
    post[1]->setEnabled(gDoBloom);
    post[2]->setEnabled(gDoDof);
    post[3]->setEnabled(gDoKaleidoscope);
    post[4]->setEnabled(gDoNoiseWarp);
    post[5]->setEnabled(gDoPixelate);
    post[6]->setEnabled(gDoEdgePass);
    post[7]->setEnabled(gDoVerticalTiltShift);
    post[8]->setEnabled(gDoGodRays);
    post[9]->setEnabled(gDoLimbDarkening);
    post[10]->setEnabled(gDoSsao);
    
    
    dof->setAperture(gDofAperture);
    dof->setMaxBlur(gDofMaxBlur);
    dof->setFocus(gDofFocus);
    
    godRays->setLightDirDOTviewDir(gGodRaysLightDotView);
    
    ssao->setAoClamp(gSsaoAoClamp);
    ssao->setLumInfluence(gSsaoLumInfluence);
    
    
    
}
//---------------------------------------
void PostProcessingManager::setupGui(){
    gui.setup("postProcessing");
    
    gui.add(gDoFxaa.setup("Fxaa", false));
    
    gui.add(gDoBloom.setup("Bloom", false));
    
    gui.add(gDoDof.setup("Dof", false));
    gui.add(gDofFocus.setup("Dof-Focus", 0.9, 0.0, 1.0));
    gui.add(gDofAperture.setup("Dof-Aperture", 0.2, 0.0, 1.0));
    gui.add(gDofMaxBlur.setup("Dof-MaxBlur", 0.05, 0.0, 1.0));
    
    gui.add(gDoKaleidoscope.setup("Kaliedoscope", false));
    gui.add(gDoNoiseWarp.setup("NoiseWarp", false));
    gui.add(gDoPixelate.setup("Pixelate", false));
    gui.add(gDoEdgePass.setup("EdgePass", false));
    gui.add(gDoVerticalTiltShift.setup("VerticalTiltShift", false));
    
    gui.add(gDoGodRays.setup("GodRays", false));
    gui.add(gGodRaysLightDotView.setup("GodRays-Light", 0.3, 0.0, 1.0));
    
    gui.add(gDoLimbDarkening.setup("LimbDarkening", false));
    
    gui.add(gDoSsao.setup("Ssao", false));
    gui.add(gSsaoAoClamp.setup("Ssao-AoClamp", 0.65, 0.0, 1.0));
    gui.add(gSsaoLumInfluence.setup("Ssao-LumInfluence", 0.25, 0.0, 1.0));
}
//---------------------------------------
void PostProcessingManager::drawGui(int x, int y){
    gui.setPosition(x, y);
    gui.draw();
}
//---------------------------------------
void PostProcessingManager::begin(){
    post.begin();
}
//---------------------------------------
void PostProcessingManager::begin(ofCamera& cam){
    post.begin(cam);
}
//---------------------------------------
void PostProcessingManager::begin(ofCamera &cam, ofRectangle viewport){
    post.begin(cam, viewport);
}
//---------------------------------------
void PostProcessingManager::end(){
    post.end();
}
