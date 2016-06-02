#include "ofApp.h"


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
    bShowGuiCubeSphere = bShowGuiPair = bShowGuiInstanced = true;
    
    
    //----------------------------
    _center.set(fw*0.5, fh*0.5, 0.0);
    
    cam.setPosition(ofVec3f(_center.x, _center.y, 500.0));
    cam.lookAt(_center);
    cam.setTarget(_center);
    
    light.setPosition(fw*0.2, fw*.5, fh*0.3);
    light.setPointLight();
    
    //Instanced---------------------------
    instanced.setup(fw);
    instanced.setLimits(ofVec3f(fw, fh, 100));
    instanced.setOrientation(ofVec3f(1,1,1));
    instanced.setColor(COLOR_INSTANCED);
    
    //Particles------------------
    pair.setup(0, fh*0.5, fw, fh, OUT);
    pair.setColor(COLOR_PARTICLES);
    
    //Sphere------------------------
    sphere.setup();
    sphere.setColor(COLOR_SPHERE);
    sphere.setPosition(_center);
    
    //Post-Procesing-------------------------------
    postManager.setup(fw, fh);
    
    //--------------------------------------------
}

//--------------------------------------------------------------
void ofApp::update(){
    //display frame rate as window title
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    instanced.update();
    updatePair();
    sphere.update();
    
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
        instanced.drawGui();
    if(bShowGuiPair)
        guiPair.draw();
    if(bShowGuiCubeSphere)
        sphere.drawGui();
    
    guiMain.draw();
    
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
void ofApp::drawDomeLimits(int w, int h){
    
    
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
    
//    ofClear(255, 255, 255, 0);
    ofClear(0, 0, 0, 0);

    
    if(gDoDrawInstanced)fboInstanced.draw(0,0);

    if(gDoDrawParts) fboParticles.draw(0,0);
    
    if(gDoDrawSphere) fboSphere.draw(0,0);
    
    if(gDoDrawDomeLimits) drawDomeLimits(fw, fh);
    
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
    
    //??? hacen falta estos guachines?
    ofEnableDepthTest();
    ofDisableAlphaBlending();
    //-----------------------
    
    instanced.drawScene();
    
    
    ofDisableDepthTest();//??? hace falta?
    
    cam.end();
    
    if(gUseLight){
        light.disable();
        ofDisableLighting();
    }
    //----------------------------
    fboInstanced.end();

   
    
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

    
    ofEnableDepthTest();
    ofEnableAlphaBlending();
   
    ///DRAW pair
    pair.drawScene();
   
    
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
  

    ///drawSphere---------
    sphere.drawScene();
    ///---------
    
    
    
    if(gUseLight){
        light.disable();
        ofDisableLighting();
    }
    
    cam.end();
    
    ofDisableDepthTest();
    
    //----------------------------
    fboSphere.end();

}
//--------------------------------------------------------------
void ofApp::setupGui(){
    
    //-----------------------------------
    guiMain.setup("Main Panel");
    guiMain.setPosition(0, 0);
    guiMain.add(gDoPostProcessing.setup("Post-Proc", true));
    guiMain.add(gReceiveOSC.setup("Receive Osc", true));
    guiMain.add(gDoDrawInstanced.setup("Draw Inst", true));
    guiMain.add(gDoDrawSphere.setup("Draw Sphere", true));
    guiMain.add(gDoDrawParts.setup("Draw Parts", true));
    guiMain.add(gDoDrawDomeLimits.setup("Draw Dome Lim", true));
    guiMain.add(gLightPos.setup("LighPos", ofVec3f(0.5), ofVec3f(0.0), ofVec3f(1.0)));
    guiMain.add(gUseCam.setup("useCam", true));
    guiMain.add(gAxis.setup("axis", true));
    guiMain.add(gUseLight.setup("useLight", true));

    
    //--------------------------
    //??? mandarlo al particleSystemPair?
    guiPair.setup();
    guiPair.gui.setPosition(200, 0);


}

//--------------------------------------------------------------
void ofApp::updatePair(){
   //??? mandarlo al particleSystemPair?
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
void ofApp::resetCamera(){
    
    cam.setPosition(ofVec3f(fw*0.5, fh*0.5, 500.0));
    cam.lookAt(ofVec3f( fw*0.5, fh*0.5, 0.0));
    cam.setTarget(ofVec3f( fw*0.5, fh*0.5, 0.0));

}
//--------------------------------------------------------------

