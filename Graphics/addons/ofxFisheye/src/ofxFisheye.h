/*
ofxFisheye is an addon based on:
http://www.geeks3d.com/20140213/glsl-shader-library-fish-eye-and-dome-and-barrel-distortion-post-processing-filters/
 */
#pragma once

#include "ofMain.h"

#include "fxFixFisheye.h"
#include "fxBarrelDist.h"
#include "fxVariableFisheye.h"


enum fxType{
    tFixFisheye,
    tBarrelDist,
    tVariableFisheye
};

class ofxFisheye{
    
    public:
     
        void setup(fxType type);
        void draw();
        void update();
        void exit();
    
        void begin(ofTexture& fboTexture, int w, int h, float amount);
        void end();
    
        void setFxType(fxType type);
        fxType getFxType(){return currentFxType;}
        string getFxTypeAsString();
    
    private:
        
        fxType currentFxType;
        
        ofShader fixFishShader;
        ofShader barrelShader;
        ofShader varFishShader;
    
        ofShader* currentShader;
    
        fxFixFisheye        fixFisheye;
        fxBarrelDist        barrelDist;
        fxVariableFisheye   varFisheye;

};

