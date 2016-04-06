//
//  instancedManager.cpp
//  borradorFriccion
//
//  Created by Leo on 16/03/15.
//
//

#include "instancedManager.h"

void InstancedManager::setup(){
    
    ofBoxPrimitive tmpBox;
	tmpBox.set(1.0);// set the size
	vboMesh = tmpBox.getMesh();
    
  	shaderInst.load("shadersGL2/instancedRad.vert", "shadersGL2/instancedRad.frag");
    
    _hRes = 40;
    _vRes = 20;
    _width = 100;
    _height = 30;
    
    Orient.set(1,1,1);
    yPos = 0.0;
    bDoQuilombo =false;
    velX=1.0;
    
    
    //material--
    material.setShininess( 100 );    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setSpecularColor(ofColor(255.f, 255.f, 255.f, 255.f));
    material.setDiffuseColor(mainColor);
}

//---------------------------------------
void InstancedManager::update(){
    
}

//--------------------------------------
void InstancedManager::draw(){
    
    
    ofEnableDepthTest();
	ofDisableAlphaBlending();

    material.begin();
    
    ofPushStyle();
	ofSetColor(mainColor);

	shaderInst.begin();
	shaderInst.setUniform1i("uHres", _hRes);
    shaderInst.setUniform1i("uWidth", _width);
    shaderInst.setUniform1i("uVres", _vRes);
    shaderInst.setUniform1i("uHeight", _height);
	shaderInst.setUniform1f("timeValue", (velCounter% 3000) / 3000.0f);
    shaderInst.setUniform1f("timeValue_b", ofGetElapsedTimeMillis());
    //shaderInst.setUniform1f("timeValue_b", yNzCounter);
    
    float vs;
   _vRes>1 ? vs=_height/(_vRes-1) : vs= 0.0; ///horizontal mode
//    _vRes>1 ? vs= ofDegToRad(360.0) /(_vRes-1) : vs= 0.0; ///radial mode
    shaderInst.setUniform1f("uVspacing",vs);
    
    
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
   
        ofPushMatrix();
        if(_vRes>1)ofTranslate(0, _height*(-.5) + (yPos*Lim.y),0);
        if(Orient.x<0){
            ofRotateY(180);
            ofTranslate(Lim.x*Orient.x, 0);
        }
    
        vboMesh.drawInstanced(OF_MESH_FILL, _hRes * _vRes);
        ofPopMatrix();
    
	shaderInst.end();
    
    ofEnableAlphaBlending();
	ofDisableDepthTest();
    //----
    velCounter+=velX;
  
    ofPopStyle();
    
    material.end();
}


//--------------------------------------
void InstancedManager::exit(){

}
//--------------------------------------
void InstancedManager::setCubeSize(float val){
    cubeSize = val;
    ofBoxPrimitive tmpBox;
	tmpBox.set(cubeSize);// set the size
	vboMesh = tmpBox.getMesh();

}




