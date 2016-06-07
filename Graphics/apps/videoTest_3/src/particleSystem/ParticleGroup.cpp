

#include "ParticleGroup.h"


//-----------------------------------------

ParticleGroup::ParticleGroup(int partsNum,int x, int y, int w, int d, float radiusInit, float angleInit){
    
    _w = w;
    //_x = w/2;
    _x = x;
    _y = y;
    _z = 0;
    
    _anchor.set(_x, _y, _z);
    
    
    _dir = d;
    _partsNum = partsNum;
    
    _size = 10;
    
    if(_dir <0)
        _angle = 0.0;//starts from right
    else
        _angle = ofDegToRad(180.0);//starts from the left
    
    
    _rotRadiusInit = ofRandom(-radiusInit, radiusInit);
    _rotAngleInit = ofRandom(-angleInit, angleInit);
    
    for(int i=0; i<_partsNum ; i++){
        ofVec3f pos(_x, _y, _z);
        ofVec3f vel(0,0,0);
        positions.push_back(pos);
        velocities.push_back(vel);
    }

    
}
//-----------------------------------------
void ParticleGroup::update(std::map<string, float>& data){
    
    float time = (float)1/ofGetFrameRate();
    
    
    float velocityX = data.at(KEY_X_VELOCITY);
    float velocityZ = velocityX;
    float angleVar  = data.at(KEY_ANGLE_VAR);
    float radiusVar = data.at(KEY_RADIUS_VAR);
    //nz
    float amp_nzAngle   = data.at(KEY_ANGLE_NZ_AMP);//0-2;
    float freq_nzAngle  = data.at(KEY_ANGLE_NZ_FREQ); //0.001 - 0.01
    float amp_nzRad     = data.at(KEY_RADIUS_NZ_AMP);//0-2
    float freq_nzRad    = data.at(KEY_RADIUS_NZ_FREQ); //1.0- 10
    float amp_nzXpos    = data.at(KEY_X_NZ_AMP);//0-ofgetWidth
    float freq_nzXpos   = data.at(KEY_X_NZ_FREQ); //0.01 - 0.1
    
    _size = data.at(KEY_PART_SIZE);
    
    
    //anchor---------------------
    _angle += time * _dir * _rotAngleInit + angleVar;
    float radiusAnchor =  (_w/2) - 100 ;
    
    //_anchor.z += -1 * time * velocityZ;
    //_anchor.x = _x + radiusAnchor * cos(_angle);
    
    _anchor.x += _dir * time * velocityX;
    
    _anchor.y = _y + radiusAnchor * sin(_angle);
   // _anchor.z = _z + radiusAnchor * cos(_angle);

    
    float radiusParts =  _rotRadiusInit + radiusVar;
   
    
    
    //set x,y,z
    for(int i=0; i<_partsNum; i++){
    
        float thisAngle = _angle + ofDegToRad( (360.0/float(_partsNum)) * i);
        float thisRadius = radiusParts;
        //float thisZpos = _anchor.z;
        float thisZpos = _anchor.x;
    
        //nz-------------------------
        
        float nzAngle = ofSignedNoise(ofGetElapsedTimef() * freq_nzAngle, i*0.1) * thisAngle * amp_nzAngle;
        thisAngle += nzAngle;
        
        float nzRadius = ofNoise(thisAngle * freq_nzRad) * radiusParts * amp_nzRad;
        thisRadius += nzRadius;
        
        float nzZpos = ofSignedNoise(thisAngle * freq_nzXpos) * amp_nzXpos;
        thisZpos += nzZpos;
        
        
        //-------------------------------------------
        
        velocities[i].z = thisRadius * cos(thisAngle);
        velocities[i].y = thisRadius * sin(thisAngle);
       
        //positions[i].x = _anchor.x + velocities[i].x;
        positions[i].z = _anchor.z + velocities[i].z;
        
        positions[i].y = _anchor.y + velocities[i].y;
        //positions[i].z = thisZpos;
        positions[i].x = thisZpos;
        
    }


    
  
    
}
//-----------------------------------------
void ParticleGroup::draw(){

    ofPushStyle();
    
   
    
    for(int i=0; i<positions.size(); i++){
        //ofDrawBox(positions[i],_size);
        ofDrawSphere(positions[i], _size);
    }

    //for lines--------
    
//    int numVertex = positions.size();
//    
//    GLfloat vertexes[numVertex * 3];//array of floats (numVertex *3) 3->x,y,z
//    int i=0;
//    for(float n=0; n<positions.size(); n++) {
//        ofVec3f vec			= positions[n];
//        vertexes[i*3]		= vec.x;
//        vertexes[(i*3)+1]   = vec.y;
//        vertexes[(i*3)+2]   = vec.z;
//        i++;
//    }
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glVertexPointer(3, GL_FLOAT, 0, vertexes);
//    ///glDrawArrays(GL_LINE_STRIP, 0, numVertex);
//    glDrawArrays(GL_LINE_LOOP, 0, numVertex);
    
    
    ofPopStyle();
}
//-----------------------------------------
bool ParticleGroup::isOutOfBounds(int x_min, int x_max){
    
    bool b = false;
    
    if(_anchor.z<x_min|| _anchor.z>x_max){
        b = true;
    }
    
    return b;
}