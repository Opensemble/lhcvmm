#pragma once

#include "ofMain.h"

#include "ParticleGroup.h"


class ParticleGroupSystem{

 public:
 
    void setup(int x, int y, int w, int h, int dir);
    void update(std::map<string, float>& data);
    void drawParticles();
    void exit(){};
    
    void addParticlesGroup(int groupPartsNum);
    
    void addGpuParticles(ofVec3f pos);
    
    void setDoAddRandom(bool b){bDoAddRandom = b;}
    
    bool getDoAddRandom(){return bDoAddRandom;}
    
    int _w, _h;
    int _x, _y;
    
    int _dir;//-1, 1
    
    ofVec2f bounds_h;
    
    vector<ParticleGroup*> partGroups;
    
    float _minRadius, _maxRadius;
    
    float radiusInit;
    float angleInit;
    
    bool bDoAddRandom;
    
    
    
    


};
