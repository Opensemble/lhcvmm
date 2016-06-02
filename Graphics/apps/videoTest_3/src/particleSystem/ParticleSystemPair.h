
#pragma once

#include "ofMain.h"

#include "ParticleGroupSystem.h"


enum PairType{
    IN,
    OUT
};

class ParticleSystemPair{
    
public:
    
    void setup(int x, int y, int w, int h, PairType type);
    void update(std::map<string, float>& data1, std::map<string, float>& data2);
    void drawScene();
    
    void reset(int x, int y, int w, int h);
    
    void checkAndDrawGroupUnions();
    
    void drawSolidMesh(ParticleGroup *group1, ParticleGroup *group2);
    void drawLineStructure(ParticleGroup *group1, ParticleGroup *group2);
    
    void addPartGroup(int ch);
    
    
    
    void setDistanceTreshold(float tresh){_distanceTreshold = tresh;}
    void setGroupPartsNum(int num){_groupPartsNum = num;}
    void setIsContinuum(bool state, float rate_ms = 500.0);
    void setDoRandom(bool state){
        _bDoRandom = state;
        system_1.setDoAddRandom(state);
        system_2.setDoAddRandom(state);
    }
    
    void setColor(ofColor col);
    
    bool getIsContinuum(){return _bIsContinuum;}
    bool getIsDoingRandom(){return _bDoRandom;}
    int getPartsNum();
    
    ParticleGroupSystem system_1, system_2;
    
    int _x, _y;
    int _w, _h;

private:
    //parameters
    float _distanceTreshold;
    int _groupPartsNum;
    
    PairType _type;
    
    ofShader lineShader;
    ofShader phongShader;
    
    bool _bIsContinuum;
    float continuumTimer;
    float lastTimer;
    float _continuum_rate_ms;
    float _bDoRandom;
    
    ofMaterial material;
    
    ofColor _mainColor;
    
    
    
    
        
    
    
    
    

};