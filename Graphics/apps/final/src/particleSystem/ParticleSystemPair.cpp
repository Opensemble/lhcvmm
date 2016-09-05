
#include "ParticleSystemPair.h"
//----------------------------------------------------------

void ParticleSystemPair::setup(int x, int y, int w, int h, PairType type){
    
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    
    _type = type;
    
    
    system_1.setup(_x, _y, _w, _h, 1);
    system_2.setup(_w, _y, _w, _h, -1);
    
    _distanceTreshold = 100.0;
    _groupPartsNum = 3;

    ofLogVerbose()<<"ParticleSystemPair setup: Loading lineShader:";
    lineShader.setGeometryInputType(GL_LINES);
    lineShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
    lineShader.setGeometryOutputCount(24);
    lineShader.load("bosque/shaders/cubeLines.vert", "bosque/shaders/cubeLines.frag", "bosque/shaders/cubeLines.geom");
    //-------------
    ofLogVerbose()<<"ParticleSystemPair setup: Loading phongShader:";
    phongShader.load("bosque/shaders/phong.vert","bosque/shaders/phong.frag" );
    //-------------
   
    
    continuumTimer = 0.0;
    lastTimer = 0.0;
    _continuum_rate_ms = 500.0;
    
    material.setShininess(100);
   
 
}
//----------------------------------------------------------
void ParticleSystemPair::update(std::map<string, float>& data1, std::map<string, float>& data2){
    
    system_1.update(data1);
    system_2.update(data2);
    
    _groupPartsNum = data1.at(KEY_PARTS_NUM);
    
    //automatic add parts
    int rate = data1.at(KEY_PARTS_RATE);
    int frame = ofGetFrameNum();
    
    if (frame%rate == 0){
        
        system_1.addParticlesGroup(_groupPartsNum);
        system_2.addParticlesGroup(_groupPartsNum);
        
    }
    
    
}
//----------------------------------------------------------
void ParticleSystemPair::drawScene(){

    material.begin();
    
    checkAndDrawGroupUnions();
    
    system_1.drawParticles();
    system_2.drawParticles();
    
    material.end();
    
}

//----------------------------------------------------------
void ParticleSystemPair::checkAndDrawGroupUnions(){
    
    
    
    for(int i=0; i<system_1.partGroups.size(); i++){
        
        auto group1 = system_1.partGroups[i];
        
        for(int j=0; j<system_2.partGroups.size(); j++){
            
            auto group2 = system_2.partGroups[j];
            
            float distance = group1->getAnchorPos().distance(group2->getAnchorPos());
            
            if(distance < _distanceTreshold){
                
                if (_type == IN){
                    drawSolidMesh(group1, group2);
                }else if(_type == OUT){
                    drawLineStructure(group1, group2);
                }
                
            }
        }
        
    }

}
//----------------------------------------------------------
void ParticleSystemPair::drawSolidMesh(ParticleGroup *group1, ParticleGroup *group2){
    
    vector<ofMeshFace> faces;
    
    ofMeshFace face;
    ofMesh mesh;
    
    //each group1 vert with all group2 vert
    for (int i=0; i<group1->positions.size(); i++){
        for(int j=0; j<group2->positions.size()-1; j++){
            
            
            ofVec3f normal;
            
            ofVec3f vec = group1->positions[i];
            face.setVertex(0, vec);
            normal = vec.cross(group1->getAnchorPos());
            normal.normalize();
            face.setNormal(0, normal);
            
            vec = group2->positions[j];
            face.setVertex(1, vec);
            normal = vec.cross(group2->getAnchorPos());
            normal.normalize();
            face.setNormal(1, normal);
            
            vec = group2->positions[j+1];
            face.setVertex(2, vec);
            normal = vec.cross(group2->getAnchorPos());
            normal.normalize();
            face.setNormal(2, normal);
    
            faces.push_back(face);

        }
    }
    //each group2 vert with all group1 vert
    for (int i=0; i<group2->positions.size(); i++){
        for(int j=0; j<group1->positions.size()-1; j++){
            
            ofVec3f normal;
            
            ofVec3f vec = group2->positions[i];
            face.setVertex(0, vec);
            normal = vec.cross(group2->getAnchorPos());
            normal.normalize();
            face.setNormal(0, normal);
            
            vec = group1->positions[j];
            face.setVertex(1, vec);
            normal = vec.cross(group1->getAnchorPos());
            normal.normalize();
            face.setNormal(1, normal);
            
            vec = group1->positions[j+1];
            face.setVertex(2, vec);
            normal = vec.cross(group1->getAnchorPos());
            normal.normalize();
            face.setNormal(2, normal);
            
            faces.push_back(face);
        }
        
    }
    //set mesh faces
    mesh.setFromTriangles(faces);
    //mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);

    //--------------------------------------
    
    ofPushStyle();

    ofFill();
    
    
   
    
    phongShader.begin();
    
    mesh.draw();
    
    phongShader.end();

    ofPopStyle();

}
//----------------------------------------------------------
void ParticleSystemPair::drawLineStructure(ParticleGroup *group1, ParticleGroup *group2){
    
    vector<ofMeshFace> faces;
    
    ofMeshFace face;
    ofMesh mesh;
    
    //each group1 vert with all group2 vert
    for (int i=0; i<group1->positions.size(); i++){
        for(int j=0; j<group2->positions.size()-1; j++){
            
            face.setVertex(0, group1->positions[i]);
            face.setVertex(1, group2->positions[j]);
            face.setVertex(2, group2->positions[j+1]);
            
            faces.push_back(face);
            
        }
    }
    //each group2 vert with all group1 vert
    for (int i=0; i<group2->positions.size(); i++){
        for(int j=0; j<group1->positions.size()-1; j++){
            
            face.setVertex(0, group2->positions[i]);
            face.setVertex(1, group1->positions[j]);
            face.setVertex(2, group1->positions[j+1]);
            
            faces.push_back(face);
        }
        
    }
    //set mesh faces
    mesh.setFromTriangles(faces);
    
    //--------------------------------------
    
    ofPushStyle();
    
    ofFill();
    
    lineShader.begin();
    lineShader.setUniform1f("thickness", 0.002*_w);
    
        //mesh.drawWireframe();
        int numVertex = mesh.getNumVertices();
    
        GLfloat vertexes[numVertex * 3];//array of floats (numVertex *3) 3->x,y,z
        int i=0;
        for(float n=0; n<mesh.getNumVertices(); n++) {
            ofVec3f vec			= mesh.getVertices()[n];
            vertexes[i*3]		= vec.x;
            vertexes[(i*3)+1]   = vec.y;
            vertexes[(i*3)+2]   = vec.z;
            i++;
        }
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertexes);
        glDrawArrays(GL_LINE_STRIP, 0, numVertex);
    
    lineShader.end();
    
    ofPopStyle();
    
}

//----------------------------------------------------------
void ParticleSystemPair::addPartGroup(int ch){
    
    if(ch==1){
        system_1.addParticlesGroup(_groupPartsNum);
    }else if (ch==2){
        system_2.addParticlesGroup(_groupPartsNum);
    }
    
}

//----------------------------------------------------------
void ParticleSystemPair::setIsContinuum(bool state, float rate_ms){
    
    _bIsContinuum = state;
    _continuum_rate_ms = rate_ms;
    
}
//----------------------------------------------------------
void ParticleSystemPair::reset(int x, int y, int w, int h){
    
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    
    system_1.setup(_x, _y, _w, _h, 1);
    system_2.setup(_w, _y, _w, _h, -1);

}
//----------------------------------------------------------
int ParticleSystemPair::getPartsNum(){
    
    return system_1.partGroups.size() + system_2.partGroups.size();
}
//----------------------------------
void ParticleSystemPair::setColor(ofColor col){
    _mainColor = col;
    material.setDiffuseColor(_mainColor);//??? es diffuse el que va?
}

