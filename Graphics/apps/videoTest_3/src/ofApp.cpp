#include "ofApp.h"



//TODO: clean-up code
//TODO: add behaivour react to simulated parameters onset, power, etc - gui
//TODO: add OSC with Sonoscopio


//TODO: para el jueves: demo con wav viejo, con OSC

//FIXME: solve edges marking with post-procc.
//FIXME: esta dado vuelta con el post-processing?

//TODO: make all animations frameNum dependants

//--------------------------------------------------------------
void ofApp::setup(){
    
 
    ofSetBackgroundColor(0);
    ///FRAME RATE: 30
    ofSetFrameRate(30.0);
    ofEnableAlphaBlending();
    ofEnableDepthTest();
    
    //
    
    drawFbo.allocate(768,768);
    fw = drawFbo.getWidth();
    fh = drawFbo.getHeight();
    
    fboPost.allocate(fw, fh);
    fboPost.begin();
    ofClear(0,0,0,1);
    fboPost.end();
    
    fboInstanced.allocate(fw, fh, GL_RGBA);
    fboInstanced.begin();
        ofClear(255,255,255, 0);
    fboInstanced.end();
    
    fboParticles.allocate(fw, fh, GL_RGBA);
    fboParticles.begin();
    ofClear(255,255,255, 0);
    fboParticles.end();
    
    fboSphere.allocate(fw, fh, GL_RGBA);
    fboSphere.begin();
    ofClear(255,255,255, 0);
    fboSphere.end();
    
    
    //gui-------------
    setupGui();
    
    _center.set(fw*0.5, fh*0.5, 0.0);
    
    cam.setPosition(ofVec3f(_center.x, _center.y, 500.0));
    cam.lookAt(_center);
    cam.setTarget(_center);
    
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
    
    ///Displacement-------------
    displacement.setup();
    dispResolution = 48;
    
    faceShader.setGeometryInputType(GL_TRIANGLE_STRIP);
    faceShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
    faceShader.setGeometryOutputCount(3);
    faceShader.load("entropica/shaders/faces/vert.glsl", "entropica/shaders/faces/frag.glsl", "entropica/shaders/faces/geom.glsl");
    
    phongShader.load("entropica/shaders/phong/phong.vert","entropica/shaders/phong/phong.frag" );
    
    //-------------------------------
    
    postManager.setup(fw, fh);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    //display frame rate as window title
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    updateInstanced();
    updatePair();
    updateSphere();
    
    postManager.updateValues();
    
    //light pos
    light.setPosition(gLightPos->x * MAX_LIGHT_X,
                      gLightPos->y * MAX_LIGHT_Y,
                      gLightPos->z * MAX_LIGHT_Z );
    
    //draw openGL scene in drawFbo
    drawFboInstanced();
    drawFboParticles();
    drawFboSphere();
    
    drawFboMain();
    
    drawFboPost();



   
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    
    //drawFbo.draw(ofGetWidth()-768, 0, fw, fh);
    fboPost.draw(ofGetWidth()-768, 0, fw, fh);
    
    
    if (bShowGuiInstanced)
        guiInstanced.draw();
    if(bShowGuiPair)
        guiPair.draw();
    if(bShowGuiCubeSphere)
        guiSphere.draw();
    
    postManager.drawGui(200,500);

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

   
    
//    ofBackground(0);
//    
//
//    if(gUseLight){
//        ofEnableLighting();
//        light.enable();
//    }
//    
//    if(gUseCam)cam.begin();
//    
//    if(gAxis)ofDrawAxis(1000);
//    
//    ///Draw INSTANCED
//    instanced.draw();
//    
//    ///DRAW pair
//    ofEnableDepthTest();
//    ofEnableAlphaBlending();
//    materialPair.begin();
//    pair.draw();
//    materialPair.end();
//    
//    ///draw cubeSphere
//   
//    materialPair.begin();
//    
//    if (bDoFaceSh) {
//        faceShader.begin();
//        faceShader.setUniform1f("timeVal", ofGetFrameNum());
//        faceShader.setUniform1f("noiseAmnt", dispNzAmnt);
//    }else{
//        phongShader.begin();
//    }
//    
//    ofPushMatrix();
//    ofTranslate(_center.x, _center.y, zPos);
//    ofRotateX(180.0);
//    
//
//    bDoFaces ? sphereDistor.draw() : displacement.mainMesh.draw();
//    ofPopStyle();
//    
//    
//    ofPopMatrix();
//    
//    
//    if (bDoFaceSh){
//        faceShader.end();
//    }else{
//        phongShader.end();
//    }
//    
//    materialPair.end();
//    
//    ///---------
//    
//    if(gUseCam)cam.end();
//    
//    if(gUseLight){
//        light.disable();
//        ofDisableLighting();
//    }
//    
//    ofDisableDepthTest();
    
    drawDomeMask(fw, fh);
}
//--------------------------------------------------------------
void ofApp::drawDomeMask(int w, int h){
    
    
    //ofBackground(255);
    int x = w*0.5;
    int y = h*0.5;
    int rad = w*0.5;
    
    ofPushStyle();
    ofSetCircleResolution(30);
    ofNoFill();
    //full
    ofSetColor(ofColor::red);
    ofDrawCircle(x,y,rad);
//    //mid
//    ofSetColor(ofColor::orange);
//    ofDrawCircle(x,y,rad*0.66);
//    //small
//    ofSetColor(ofColor::yellow);
//    ofDrawCircle(x,y,rad*0.33);
    
    
    ofPopStyle();
}
//--------------------------------------------------------------
void ofApp::drawFboPost(){

    fboPost.begin();
    ofClear(0,0,0,1);
    
    ofDisableAlphaBlending();
    
    postManager.begin();
    drawFbo.draw(0,0);
    postManager.end();
    
    fboPost.end();
    
    ofEnableAlphaBlending();
}
//--------------------------------------------------------------
void ofApp::drawFboMain(){
    
    drawFbo.begin();
    ofClear(255, 255, 255, 0);
//    ofClear(0,0, 0, 1);
    
    fboInstanced.draw(0,0);
    fboParticles.draw(0,0);
    fboSphere.draw(0,0);
    
    drawDomeMask(fw, fh);
    
    drawFbo.end();
}
//--------------------------------------------------------------
void ofApp::drawFboInstanced(){
    
    
    ofEnableAlphaBlending();
    
    fboInstanced.begin();
    ofClear(255,255,255, 0);
    //----------------------------
    if(gUseLight){
        ofEnableLighting();
        light.enable();
    }
    
    cam.begin();
    
    if(gAxis)ofDrawAxis(1000);
    
    instanced.draw();
    
    cam.end();
    
    if(gUseLight){
        light.disable();
        ofDisableLighting();
    }
    //----------------------------
    fboInstanced.end();

    //ofDisableAlphaBlending();
    
}
//--------------------------------------------------------------
void ofApp::drawFboParticles(){

    fboParticles.begin();
    ofClear(255,255,255, 0);
    //----------------------------
    if(gUseLight){
        ofEnableLighting();
        light.enable();
    }

    if(gUseCam)cam.begin();

    ///DRAW pair
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    materialPair.begin();
    pair.draw();
    materialPair.end();
    
    if(gUseCam)cam.end();
    
    if(gUseLight){
        light.disable();
        ofDisableLighting();
    }
    
    ofDisableDepthTest();
    //----------------------------
    fboParticles.end();
}
//--------------------------------------------------------------
void ofApp::drawFboSphere(){
    
    fboSphere.begin();
    ofClear(255,255,255, 0);
    //----------------------------
    
    
    ofEnableDepthTest();
   
    
    if(gUseLight){
        ofEnableLighting();
        light.enable();
    }

    cam.begin();
  

    ///draw cubeSphere

    materialPair.begin();

    if (bDoFaceSh) {
        faceShader.begin();
        faceShader.setUniform1f("timeVal", ofGetFrameNum());
        faceShader.setUniform1f("noiseAmnt", dispNzAmnt);
    }else{
        phongShader.begin();
    }

    ofPushMatrix();
    ofTranslate(_center.x, _center.y, zPos);
    ofRotateX(180.0);


    bDoFaces ? sphereDistor.draw() : displacement.mainMesh.draw();


    ofPopMatrix();


    if (bDoFaceSh){
        faceShader.end();
    }else{
        phongShader.end();
    }

    materialPair.end();
    
    ///---------
    
    
    
    if(gUseLight){
        light.disable();
        ofDisableLighting();
    }
    
    cam.end();
    postManager.end();
    
    ofDisableDepthTest();
    
    //----------------------------
    fboSphere.end();

}
//--------------------------------------------------------------
void ofApp::setupGui(){
    
    guiInstanced.setup("instanced");
    guiInstanced.setPosition(0,0);
    guiInstanced.add(gMode.setup("LINEAL/RADIAL", true));
    guiInstanced.add(gRadMode.setup("Concentric/Centrifuge", false));
    guiInstanced.add(gRadDeform.setup("Radial Mix", 0.0, 0.0, 1.0));
    
    
    guiInstanced.add(gWidth.setup("width", 1.0, 0., 1.0));
    guiInstanced.add(gHeight.setup("height/radius", 0.36, 0., 3.0));
    guiInstanced.add(gCubesizeUnified.setup("cubesize", 0.15, 0., 1.0));
    //guiInstanced.add(gCubesize.setup("cubesize", ofVec3f(0.2), ofVec3f(0.0), ofVec3f(1.0)));
    guiInstanced.add(gMaskRadius.setup("maskRadius", 0.53, 0.0, 1.0));
    guiInstanced.add(gHres.setup("Hres", 0.7, 0., 1.0));
    guiInstanced.add(gVres.setup("Vres", 0.7, 0., 1.0));
    guiInstanced.add(gXpos.setup("Xpos", 0.0, 0., 1.0));
    guiInstanced.add(gYpos.setup("Ypos", 0.0, 0., 1.0));
    guiInstanced.add(gZpos.setup("Zpos", 0.0, -3000.0, 0.0));
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
    guiSphere.setup("sphere");
    guiSphere.setPosition(200,300);
    guiSphere.add(velGui.setup( "Velocity", 1, 0, 5 ));
    guiSphere.add(volumeGui.setup( "Volume", 0.25, 0, 1 ));
    guiSphere.add(xGui.setup( "X", 0.02, 0, 0.1 ));
    guiSphere.add(yGui.setup( "Y", 0.01, 0, 0.1 ));
    guiSphere.add(radiusGui.setup( "Radius", 30, 0, 500 ));
    guiSphere.add(resolGui.setup( "Resolution", 48, 1, 100 ));
    guiSphere.add(strengthGui.setup( "Strength", -25, -200, 200 ));
    guiSphere.add(faceNoiseGui.setup( "FaceNoise", 20., 0., 20.));
    guiSphere.add(zPos.setup( "Z pos", 0.0, -300.0, 300.0));


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
    instanced.setZpos(gZpos);
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
 
    
    //---------------------------------
    float x, y, vol, rad, res, vel;
    
    float centroid = 0.5;
    float specComp = 0.5;
    
    x   = centroid*.1;
    y   = .001 + specComp*.049;
    vol = 0.5+centroid*0.1;
    rad = 30+specComp*100;
    res = dispResolution;
    vel  = 1;
    
    dispNzAmnt =strengthGui;
    
    displacement.update(xGui, yGui, volumeGui, radiusGui, resolGui, ofGetFrameNum()*velGui);
}
//--------------------------------------------------------------
void ofApp::resetCamera(){
    cam.setPosition(ofVec3f(fw*0.5, fh*0.5, 500.0));
    cam.lookAt(ofVec3f( fw*0.5, fh*0.5, 0.0));
    cam.setTarget(ofVec3f( fw*0.5, fh*0.5, 0.0));

}
//--------------------------------------------------------------

