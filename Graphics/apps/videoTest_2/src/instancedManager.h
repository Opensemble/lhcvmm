//
//  instancedManager.h
//  borradorFriccion
//
//  Created by Leo on 16/03/15.
//
//

#ifndef __borradorFriccion__instancedManager__
#define __borradorFriccion__instancedManager__

#include <iostream>
#include "ofMain.h"


class InstancedManager {

public:
    void setup();
    void update();
    void draw();
    void exit();
    
    
    
    void setWidth(float w){_width =  w*Lim.x;}
    void setHeight(float h){_height= h*Lim.y*2;}
    void setHres(int res){_hRes=res;}
    void setVres(int res){_vRes=res;}
    void setVelocity(float vel){velX = vel;}
    void setNzTime(float t){noiseTime = t;}
    void setXnzFreq(float f){xNoiseFreq = f;}
    void setXnzAmp(float a) {xNoiseAmp = a;}
    void setXnzRug(float r){xNoiseRug = r;}
    void setYnzFreq(float f){yNoiseFreq = f;}
    void setYnzAmp(float a) {yNoiseAmp = a;}
    void setYnzRug(float r){yNoiseRug = r;}
    void setZnzFreq(float f){zNoiseFreq = f;}
    void setZnzAmp(float a) {zNoiseAmp =a;}
    void setZnzRug(float r){zNoiseRug = r;}
    void setLimits(ofVec3f vec){Lim = vec;}
    void setCubeSize(float val);
    void setYPos(float val){yPos=val;}
    void setOrientation(ofVec3f vec){Orient=vec;}
    void setQuilombo(bool b){bDoQuilombo=b;}
    void setColor(ofColor col){
        mainColor=col;
        material.setDiffuseColor(mainColor);
    
    }
    
    float getCubeSize(){return cubeSize;}
    
    
private:
    
    ofVec3f Lim, Orient;
    
    ofVboMesh	vboMesh;
    ofShader shaderInst;
    
    float _width, _height;
    int _hRes, _vRes;
    
    float yPos;
    
    float velX = 5.0;
    float vSpacing=.5;
    
    float noiseTime = 1.0;
    
    float xNoiseFreq = 0.01;
    float xNoiseAmp  = 2.0;
    float xNoiseRug = 2.0;
    
    float yNoiseFreq = 0.01;
    float yNoiseAmp  = 2.0;
    float yNoiseRug = 2.0;
    
    float zNoiseFreq = 0.01;
    float zNoiseAmp  = 2.0;
    float zNoiseRug = 2.0;
    
    float cubeSize;
    
    bool bDoQuilombo;
    
    ofColor mainColor;
    
    int velCounter;
    float YnzCounter;
    
    ofMaterial material;
    
    




};



#endif /* defined(__borradorFriccion__instancedManager__) */
