
#pragma once

#include "ofMain.h"
#include "BosqueConstants.h"



class ParticleGroup{

 public:
 
    ParticleGroup(int partsNum, int x, int y, int w,  int d, float radiusInit, float angleInit);
    ~ParticleGroup();
    
    void update(std::map<string, float>& data);
    void draw();
    
    int getPartsNum(){return _partsNum;}
    bool isOutOfBounds(int x_min, int x_max);
    ofVec3f getAnchorPos(){return _anchor;}
    
    //Vars---
     vector<ofVec3f> positions;
    
    
    
 private:
    //int _Xpos;
    int _x, _y, _z;
    int _w;
    int _dir;//-1 o 1
    int _partsNum;
    int _size;
    float _angle;
    
    float _rotAngleInit;
    float _rotRadiusInit;
    
    ofVec3f _anchor;
    vector<ofVec3f> velocities;
    
    int frameCounter;
};
