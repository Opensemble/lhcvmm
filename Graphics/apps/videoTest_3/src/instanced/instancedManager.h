//
//  instancedManager.h
//  borradorFriccion
//
//  Created by Leo on 16/03/15.
//
//

#pragma once

#include "ofMain.h"

enum InstancedMode {
    LINEAL,
    RAD_CONCENTRIC,
    RAD_CENTRIFUGE
};

class InstancedManager {

public:
    
    void setup();
    void draw();
    
    void setMode(InstancedMode m){_mode = m;}
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
    void setCubeSize(ofVec3f size);
    void setXpos(float val){xPos = val;}
    void setYpos(float val){yPos=val;}
    void setRadDeform(float val){radDeform = val;}
    void setOrientation(ofVec3f vec){Orient=vec;}
    void setQuilombo(bool b){bDoQuilombo=b;}
    void setMaskRadius(float radius){maskRadius=radius;}
    void setColor(ofColor col){
        mainColor=col;
        material.setDiffuseColor(mainColor);
    }
    
    float getCubeSize(){return cubeSize;}
    
    
private:
    
    InstancedMode _mode;
    
    ofVec3f Lim, Orient;
    
    ofVboMesh	vboMesh;
    ofShader shaderInst;
    
    ofColor mainColor;
    
    float maskRadius;
    
    float _width, _height;
    int _hRes, _vRes;
    
    float xPos, yPos;
    
    float radDeform = 0.0;
    
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
    
    int velCounter;
    float YnzCounter;
    
    ofMaterial material;
    


};


