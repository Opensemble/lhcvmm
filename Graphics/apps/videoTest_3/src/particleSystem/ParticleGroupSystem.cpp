
#include "ParticleGroupSystem.h"

void ParticleGroupSystem::setup(int x, int y, int w, int h, int dir){
    
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    _dir = dir;
    
    
    if(dir>0)
        bounds_h.set(_x, _w);
    else if(dir<0)
        bounds_h.set(_x-_w, _x);
    
    _minRadius = 30.0;
    _maxRadius = 90.0;
    
  
}
//----------------------------------
void ParticleGroupSystem::update(std::map<string, float>& data){
    
    radiusInit= data.at(KEY_RADIUS_INIT);
    angleInit = data.at(KEY_ANGLE_INIT);
    
     for(int i=0; i<partGroups.size(); i++){
        
        partGroups[i]->update(data);
         
         //!!!z-limit
        if(partGroups[i]->isOutOfBounds(-3000, 100)){
            partGroups.erase(partGroups.begin()+i);
        }
    }
    


}
//---------------------------------
void ParticleGroupSystem::drawParticles(){

    for (auto p : partGroups){
        p->draw();
    }
    

    
}
//--------------------------------
void ParticleGroupSystem::addParticlesGroup(int groupPartsNum){
   
    int this_y;
    
    if(bDoAddRandom) this_y = ofRandom(_h);
    else this_y = _y;
    
    
    auto p = new ParticleGroup(groupPartsNum, _x, this_y, _w, _dir, radiusInit, angleInit);
    partGroups.push_back(p);

    
}

