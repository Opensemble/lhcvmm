

#include "instancedManager.h"

void InstancedManager::setup(int fboWidth){
    
    _fboWidth = fboWidth;
    
    ofBoxPrimitive tmpBox;
	tmpBox.set(1.0);// set the size
	vboMesh = tmpBox.getMesh();
    
  	shaderInst.load("shadersGL2/instanced.vert", "shadersGL2/instanced.frag");
    
    _mode = LINEAL;
    _hRes = 40;
    _vRes = 20;
    _width = 100;
    _height = 30;
    
    Orient.set(1,1,1);
    yPos = 0.0;
    bDoQuilombo =false;
    velX=1.0;
    
    zPos = 0.0;
    
    velCounter = 1;
    
    maskRadius = 0.0;
    
    //material--
    material.setShininess( 100 );    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setSpecularColor(ofColor(255.f, 255.f, 255.f, 255.f));
    material.setDiffuseColor(mainColor);
    
    //-------------------
    setupGui();//mandatory
}


//--------------------------------------
void InstancedManager::update(){
    
    int w = _fboWidth;
    
    //update instancedManager values
    if(!gMode)setMode(LINEAL);
    else if(gMode && !gRadMode) setMode(RAD_CONCENTRIC);
    else if(gMode && gRadMode){
        setMode(RAD_CENTRIFUGE);
        setRadDeform(gRadDeform);
    }
    
    setWidth(gWidth);
    setHeight(gHeight);
    setCubeSize(gCubesizeUnified * MAX_CUBESIZE*w);
    
    setMaskRadius(gMaskRadius);
    setHres(gHres * MAX_H_RES);
    setVres(gVres * MAX_V_RES);
    setVelocity(gVelocity * MAX_VELOCITY);
    setXpos(gXpos);
    setYpos(gYpos);
    setZpos(gZpos);
    //nz
    setNzTime(gNzTime * MAX_NZ_TIME);
    
    setXnzAmp(gNzXAmp * MAX_NZ_AMP*w);
    setXnzFreq(gNzXFreq * MAX_NZ_FREQ);
    setXnzRug(gNzXRug * MAX_NZ_RUG*w);
    
    setYnzAmp(gNzYAmp * MAX_NZ_AMP*w);
    setYnzFreq(gNzYFreq * MAX_NZ_FREQ);
    setYnzRug(gNzYRug * MAX_NZ_RUG*w);
    
    setZnzAmp(gNzZAmp * MAX_NZ_AMP*w);
    setZnzFreq(gNzZFreq * MAX_NZ_FREQ);
    setZnzRug(gNzZRug * MAX_NZ_RUG*w);

}
//--------------------------------------
void InstancedManager::drawScene(){
    


    material.begin();
    
    ofPushStyle();
	ofSetColor(mainColor);
    
    ofPushMatrix();
    
    //set XYZ-----------------------
    
    //lineal
    if(_mode==LINEAL && _vRes>1)
        ofTranslate(xPos*Lim.x, _height*(-.5) + (yPos*Lim.y), zPos);
    //radial
    else if(_mode!=LINEAL && _vRes>1)
        ofTranslate(xPos*Lim.x, Lim.y*(.5) + (yPos*Lim.y),zPos);
    
    
    if(Orient.x<0){
        ofRotateY(180);
        ofTranslate(Lim.x*Orient.x, 0);
    }
    //-----------------------------------
    //Set Shader uniforms
    //---------------------------------------
	shaderInst.begin();
    if (_mode == LINEAL)
        shaderInst.setUniform1i("uMode", 0);
    else if(_mode == RAD_CONCENTRIC)
        shaderInst.setUniform1i("uMode", 1);
    else if(_mode == RAD_CENTRIFUGE)
        shaderInst.setUniform1i("uMode", 2);
    
    shaderInst.setUniform1f("uDeformRad", radDeform);
        
	shaderInst.setUniform1i("uHres", _hRes);
    shaderInst.setUniform1i("uWidth", _width);
    shaderInst.setUniform1i("uVres", _vRes);
    shaderInst.setUniform1i("uHeight", _height);
    shaderInst.setUniform1f("uMaskRadius", maskRadius);
    
	shaderInst.setUniform1f("timeValue", (velCounter% 3000) / 3000.0f);
    //shaderInst.setUniform1f("timeValue_b", ofGetElapsedTimeMillis()); //time dependant
    shaderInst.setUniform1f("timeValue_b", velCounter*10.0); //frame dependant
    
    shaderInst.setUniform1f("uTimeNoise", noiseTime);
    
    shaderInst.setUniform1f("uXnoiseFreq", xNoiseFreq);
    shaderInst.setUniform1f("uXnoiseAmp",  xNoiseAmp*Orient.x);
    shaderInst.setUniform1f("uXnoiseRug",  xNoiseRug);
    
    shaderInst.setUniform1f("uYnoiseFreq", yNoiseFreq);
    shaderInst.setUniform1f("uYnoiseAmp", yNoiseAmp*Orient.x);
    shaderInst.setUniform1f("uYnoiseRug", yNoiseRug);
    
    shaderInst.setUniform1f("uZnoiseFreq", zNoiseFreq);
    shaderInst.setUniform1f("uZnoiseAmp", zNoiseAmp*Orient.x);
    shaderInst.setUniform1f("uZnoiseRug", zNoiseRug);
   
    vboMesh.drawInstanced(OF_MESH_FILL, _hRes * _vRes);
    
	shaderInst.end();//------------------------------------
    material.end();
    
	
    //----
    velCounter+=velX;
    
    ofPopMatrix();
  
    ofPopStyle();
    
   
}


//--------------------------------------
void InstancedManager::setCubeSize(float val){

    cubeSize = val;

    ofBoxPrimitive tmpBox;
    tmpBox.set(cubeSize);// set the size
    vboMesh = tmpBox.getMesh();
    
//    ofSpherePrimitive tmpSph;
//    tmpSph.set(cubeSize, 10);
//    vboMesh = tmpSph.getMesh();

  
}
//--------------------------------------
void InstancedManager::setCubeSize(ofVec3f size){
    ofBoxPrimitive tmpBox;
    tmpBox.set(size.x, size.y, size.z);
    vboMesh = tmpBox.getMesh();
}
//--------------------------------------
void InstancedManager::setupGui(){

    guiInstanced.setup("instanced");
    guiInstanced.setPosition(0,300);
    guiInstanced.add(gMode.setup("LINEAL/RADIAL", true));
    guiInstanced.add(gRadMode.setup("Concentric/Centrifuge", false));
    guiInstanced.add(gRadDeform.setup("Radial Mix", 0.0, 0.0, 1.0));
    
    
    guiInstanced.add(gWidth.setup("width", 1.0, 0., 1.0));
    guiInstanced.add(gHeight.setup("height/radius", 0.36, 0., 3.0));
    guiInstanced.add(gCubesizeUnified.setup("cubesize", 0.15, 0., 1.0));
    guiInstanced.add(gMaskRadius.setup("maskRadius", 0.16, 0.0, 1.0));
    guiInstanced.add(gHres.setup("Hres", 1.0, 0., 1.0));
    guiInstanced.add(gVres.setup("Vres", 1.0, 0., 1.0));
    guiInstanced.add(gXpos.setup("Xpos", 0.0, 0., 1.0));
    guiInstanced.add(gYpos.setup("Ypos", 0.0, 0., 1.0));
    guiInstanced.add(gZpos.setup("Zpos", 0.0, -3000.0, 0.0));
    guiInstanced.add(gVelocity.setup("velocity", 0.1, 0., 1.0));
    //nz
    guiInstanced.add(gNzTime.setup("nzTime", 0.1, 0.0, 1.0));
    
    guiInstanced.add(gNzXAmp.setup("nzXAmp", 0.0, 0.0, 1.0));
    guiInstanced.add(gNzXFreq.setup("nzXFreq", 0.395, 0.0, 1.0));
    guiInstanced.add(gNzXRug.setup("nzXRug", 0.1, 0.01, 1.0));
    
    guiInstanced.add(gNzYAmp.setup("nzYAmp", 0.0, 0.0, 1.0));
    guiInstanced.add(gNzYFreq.setup("nzYFreq", 0.5, 0.0, 1.0));
    guiInstanced.add(gNzYRug.setup("nzYRug", 2.0, 0.01, 30.0));
    
    guiInstanced.add(gNzZAmp.setup("nzZAmp", 0.14, 0.0, 1.0));
    guiInstanced.add(gNzZFreq.setup("nzZFreq", 0.2, 0.0, 1.0));
    guiInstanced.add(gNzZRug.setup("nzZRug", 0.05, 0.01, 1.0));
}
//--------------------------------------
void InstancedManager::drawGui(){
    guiInstanced.draw();
}



