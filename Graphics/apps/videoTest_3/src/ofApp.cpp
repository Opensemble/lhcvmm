#include "ofApp.h"

//TODO: fix cubeSphere shader
//TODO: add post-processing with its gui
//TODO: add behaivour react to simulated parameters onset, power, etc - gui

//TODO: add OSC with Sonoscopio
//TODO: para el jueves: demo con wav viejo, con OSC

//--------------------------------------------------------------
void ofApp::setup(){
    
 
    ofSetBackgroundColor(0);
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    ofEnableDepthTest();
    
    //
    
    drawFbo.allocate(768,768);
    fw = drawFbo.getWidth();
    fh = drawFbo.getHeight();
    
    //gui-------------
    setupGui();
    
    
    cam.setPosition(ofVec3f(fw*0.5, fh*0.5, 500.0));
    cam.lookAt(ofVec3f( fw*0.5, fh*0.5, 0.0));
    cam.setTarget(ofVec3f( fw*0.5, fh*0.5, 0.0));
    
    light.setPosition(fw*0.2, fw*.5, fh*0.3);
    light.setPointLight();
    
    instanced.setup();
    instanced.setLimits(ofVec3f(fw, fh, 100));
    instanced.setOrientation(ofVec3f(1,1,1));
    instanced.setColor(ofColor::white);
    
    
    pair.setup(0, fh*0.5, fw, fh, OUT);
    materialPair.setShininess(100);
    bShowGuiCubeSphere = bShowGuiPair = bShowGuiInstanced = true;
    
    //cubeSphere
    cubeShader.setGeometryInputType(GL_TRIANGLE_STRIP);
    cubeShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
    cubeShader.setGeometryOutputCount(24);
    cubeShader.load("entropica/shaders/cubes/vert.glsl", "entropica/shaders/cubes/frag.glsl", "entropica/shaders/cubes/geom.glsl");
    
    sphereCubes.setResolution(48);///
    sphereCubes.setRadius(100);
    cubeMesh = sphereCubes.getMesh();
    cubeMesh.setMode(OF_PRIMITIVE_TRIANGLES);

}

//--------------------------------------------------------------
void ofApp::update(){
    //display frame rate as window title
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    updateInstanced();
    
    updatePair();
    
    updateSphere();
    
    //light pos
    light.setPosition(gLightPos->x * MAX_LIGHT_X,
                      gLightPos->y * MAX_LIGHT_Y,
                      gLightPos->z * MAX_LIGHT_Z );
    
    //draw openGL scene in drawFbo
    drawFbo.begin();
    ofClear(0, 0, 0, 255);
    drawScene(768, 768);
    drawFbo.end();


   
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(100);
    
    drawFbo.draw(ofGetWidth()-768, 0, fw, fh);
    
    if (bShowGuiInstanced)
        guiInstanced.draw();
    if(bShowGuiPair)
        guiPair.draw();
    if(bShowGuiCubeSphere)
        guiSphere.draw();

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key){
        case 'a':
            pair.addPartGroup(1);
            break;
        case 'k':
            pair.addPartGroup(2);
            break;
            
        //gui shows
            
        case '1':
            bShowGuiInstanced = !bShowGuiInstanced;
            break;
        case '2':
            bShowGuiPair = !bShowGuiPair;
            break;
        case '3':
            bShowGuiCubeSphere = !bShowGuiCubeSphere;
            break;
            
        case 'r':
            resetCamera();
            break;
            
        default:
            break;
    }
    
}
//--------------------------------------------------------------
void ofApp::drawScene(int w, int h){
    
    ofBackground(0);
    

    if(gUseLight){
        ofEnableLighting();
        light.enable();
    }
    
    if(gUseCam)cam.begin();
    
    if(gAxis)ofDrawAxis(1000);
    
    ///Draw INSTANCED
    instanced.draw();
    
    ///DRAW pair
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    materialPair.begin();
    pair.draw();
    materialPair.end();
    
    ///draw cubeSphere
    static int cubeTimeCnt;
    materialPair.begin();
    cubeShader.begin();
    cubeShader.setUniform1f("timeVal", cubeTimeCnt);
    cubeShader.setUniform1f("mouseX", cubeX);
    cubeShader.setUniform1f("mouseY", cubeY);
    cubeShader.setUniform1f("volume", cubeVol);
    cubeShader.setUniform1f("size", cubeSize);
    
    
    ofSetColor(ofColor::yellow);
    sphereCubes.setPosition(fw/2, fh/2, 0.0);
    sphereCubes.draw();
    
    cubeShader.end();
    
    cubeTimeCnt += (1+cubeVel);
 
    materialPair.end();
    
    if(gUseCam)cam.end();
    
    if(gUseLight){
        light.disable();
        ofDisableLighting();
    }
    
    ofDisableDepthTest();
    
    drawDomeMask(fw, fh);
}
//--------------------------------------------------------------
void ofApp::drawDomeMask(int w, int h){
    
    
    //ofBackground(255);
    
    ofPushStyle();
    ofNoFill();
    ofSetColor(ofColor::red);
    ofSetCircleResolution(30);
    ofDrawCircle(w*.5, h*.5, w*.5);
    
    
    ofPopStyle();
}
//--------------------------------------------------------------
void ofApp::setupGui(){
    
    guiInstanced.setup("instanced");
    guiInstanced.setPosition(0,0);
    guiInstanced.add(gMode.setup("LINEAL/RADIAL", true));
    guiInstanced.add(gRadMode.setup("Concentric/Centrifuge", false));
    guiInstanced.add(gRadDeform.setup("Radial Mix", 0.0, 0.0, 1.0));
    
    
    guiInstanced.add(gWidth.setup("width", 1.0, 0., 1.0));
    guiInstanced.add(gHeight.setup("height/radius", 0.36, 0., 1.0));
    guiInstanced.add(gCubesizeUnified.setup("cubesize", 0.15, 0., 1.0));
    //guiInstanced.add(gCubesize.setup("cubesize", ofVec3f(0.2), ofVec3f(0.0), ofVec3f(1.0)));
    guiInstanced.add(gMaskRadius.setup("maskRadius", 0.53, 0.0, 1.0));
    guiInstanced.add(gHres.setup("Hres", 0.7, 0., 1.0));
    guiInstanced.add(gVres.setup("Vres", 0.7, 0., 1.0));
    guiInstanced.add(gXpos.setup("Xpos", 0.0, 0., 1.0));
    guiInstanced.add(gYpos.setup("Ypos", 0.0, 0., 1.0));
    guiInstanced.add(gVelocity.setup("velocity", 0.1, 0., 1.0));
    //nz
    guiInstanced.add(gNzTime.setup("nzTime", 0.1, 0.0, 1.0));
    
    guiInstanced.add(gNzXAmp.setup("nzXAmp", 0.0, 0.0, 1.0));
    guiInstanced.add(gNzXFreq.setup("nzXFreq", 0.5, 0.0, 1.0));
    guiInstanced.add(gNzXRug.setup("nzXRug", 0.05, 0.01, 1.0));
    
    guiInstanced.add(gNzYAmp.setup("nzYAmp", 0.0, 0.0, 1.0));
    guiInstanced.add(gNzYFreq.setup("nzYFreq", 0.5, 0.0, 1.0));
    guiInstanced.add(gNzYRug.setup("nzYRug", 2.0, 0.01, 30.0));
    
    guiInstanced.add(gNzZAmp.setup("nzZAmp", 0.0, 0.0, 1.0));
    guiInstanced.add(gNzZFreq.setup("nzZFreq", 0.5, 0.0, 1.0));
    guiInstanced.add(gNzZRug.setup("nzZRug", 0.05, 0.01, 1.0));
    //
    guiInstanced.add(gUseCam.setup("useCam", true));
    guiInstanced.add(gAxis.setup("axis", true));
    guiInstanced.add(gUseLight.setup("useLight", true));
    guiInstanced.add(gLightPos.setup("LighPos", ofVec3f(0.5), ofVec3f(0.0), ofVec3f(1.0)));
    
    //--------------------------
    
    guiPair.setup();
    guiPair.gui.setPosition(200, 0);
    
    //cubeSphere------------
    guiSphere.setup("cube sphere");
    guiSphere.setPosition(200,400);
    guiSphere.add(bDoShaderGui.setup("Do Shader", false));
    guiSphere.add(xShGui.setup( "X", 0.02, 0, 2.0 ));
    guiSphere.add(yShGui.setup( "Y", 0.01, 0, 0.5 ));
    guiSphere.add(velShGui.setup( "Velocity", .1, 0, 20.0 ));
    guiSphere.add(volumeShGui.setup( "Volume", 0.5, 0, 1.0 ));
    guiSphere.add(sizeShGui.setup( "Size", 3, 0, 10.0 ));
    guiSphere.add(radiusShGui.setup( "Radius", 100, 0, 500 ));

}
//--------------------------------------------------------------
void ofApp::updateInstanced(){
    
    int w = fw;
    
    //update instancedManager values
    if(!gMode)instanced.setMode(LINEAL);
    else if(gMode && !gRadMode) instanced.setMode(RAD_CONCENTRIC);
    else if(gMode && gRadMode){
        instanced.setMode(RAD_CENTRIFUGE);
        instanced.setRadDeform(gRadDeform);
    }
    
    instanced.setWidth(gWidth);
    instanced.setHeight(gHeight);
    instanced.setCubeSize(gCubesizeUnified * MAX_CUBESIZE*w);
    //    instanced.setCubeSize(ofVec3f(gCubesize->x * MAX_CUBESIZE,
    //                                  gCubesize->y * MAX_CUBESIZE,
    //                                  gCubesize->z * MAX_CUBESIZE));
    instanced.setMaskRadius(gMaskRadius);
    instanced.setHres(gHres * MAX_H_RES);
    instanced.setVres(gVres * MAX_V_RES);
    instanced.setVelocity(gVelocity * MAX_VELOCITY);
    instanced.setXpos(gXpos);
    instanced.setYpos(gYpos);
    //nz
    instanced.setNzTime(gNzTime * MAX_NZ_TIME);
    
    instanced.setXnzAmp(gNzXAmp * MAX_NZ_AMP*w);
    instanced.setXnzFreq(gNzXFreq * MAX_NZ_FREQ);
    instanced.setXnzRug(gNzXRug * MAX_NZ_RUG*w);
    
    instanced.setYnzAmp(gNzYAmp * MAX_NZ_AMP*w);
    instanced.setYnzFreq(gNzYFreq * MAX_NZ_FREQ);
    instanced.setYnzRug(gNzYRug * MAX_NZ_RUG*w);
    
    instanced.setZnzAmp(gNzZAmp * MAX_NZ_AMP*w);
    instanced.setZnzFreq(gNzZFreq * MAX_NZ_FREQ);
    instanced.setZnzRug(gNzZRug * MAX_NZ_RUG*w);
    


}
//--------------------------------------------------------------
void ofApp::updatePair(){
    
 
   
    bool onset1, onset2;
    float pow1, pow2;
    float pitch1, pitch2;
    float conf1, conf2;
    
    
    pow1 = pow2      = 0.8;
    onset1 = onset2  = false;
    pitch1 = pitch2  = 0.8;
    conf1 = conf2    = 0.8;
    
    guiPair.update();
    

    std::map<string, float> pairData_A = guiPair.getData();
    std::map<string, float> pairData_B = guiPair.getData();
    
    
//    pairData_A[KEY_RADIUS_INIT] = pairData_B[KEY_RADIUS_INIT]  =  minRadius;
//
//    pairData_A[KEY_RADIUS_VAR]   = pow1 * maxRadius;
//    pairData_B[KEY_RADIUS_VAR]   = pow2 * maxRadius;
//    
//    
//    pairData_A[KEY_PART_SIZE]  =  pow1 * maxPartSize;
//    pairData_B[KEY_PART_SIZE]  =  pow2 * maxPartSize;
//
//    //vars that change in continuum MODE
//    if(pair.getIsContinuum() ){
//        
//        pairData_A[KEY_X_VELOCITY]  = minVelX + pow1  *  pitch1 * maxVelX;
//        pairData_B[KEY_X_VELOCITY]  = minVelX + pow2  *  pitch2 * maxVelX;
//        
//        pairData_A[KEY_ANGLE_VAR]  =  pitch1  *  pitch1  * maxAngleVar;
//        pairData_B[KEY_ANGLE_VAR]  =  pitch2  *  pitch2  *maxAngleVar;
//        
//    }else{
//        
//        pairData_A[KEY_X_VELOCITY]  = minVelX + pow1 * maxVelX;
//        pairData_B[KEY_X_VELOCITY]  = minVelX + pow2 * maxVelX;
//        
//        pairData_A[KEY_ANGLE_VAR]  =  pitch1 * maxAngleVar;
//        pairData_B[KEY_ANGLE_VAR]  =  pitch2 * maxAngleVar;
//        
//    }
    pair.setDistanceTreshold(guiPair.gDistTreshold);
    pair.update(pairData_A, pairData_B);

}
//--------------------------------------------------------------
void ofApp::updateSphere(){
    
    cubeX = xShGui;
    cubeY = yShGui;
    cubeVol = volumeShGui;
    cubeSize = sizeShGui;
    cubeRadius = radiusShGui;
    cubeVel = velShGui;
    
    sphereCubes.setRadius(cubeRadius);
}
//--------------------------------------------------------------
void ofApp::resetCamera(){
    cam.setPosition(ofVec3f(fw*0.5, fh*0.5, 500.0));
    cam.lookAt(ofVec3f( fw*0.5, fh*0.5, 0.0));
    cam.setTarget(ofVec3f( fw*0.5, fh*0.5, 0.0));

}
//--------------------------------------------------------------

