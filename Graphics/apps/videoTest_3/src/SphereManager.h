#pragma once

#include "ofMain.h"

#include "ofxGui.h"
#include "DisplacementSphereMesh.h"

class SphereManager{

public:
    
    void setup();
    void update();
    void drawScene();
    void exit(){};
    
    void setupGui();
    void drawGui();
    
    void setPosition(ofVec3f pos){_pos = pos;}
    ofVec3f getPosition(){return _pos;}
    
    void setColor(ofColor col);
    
    
    
private:
    
    ofColor _mainColor;
    
    ofMaterial material;
    ofVec3f _pos;
    
    //Cubes-sphere----------------
    ofSpherePrimitive sphereCubes;
    ofMesh cubeMesh;
    
    //Displacement-sphere-------
    DisplacementSphereMesh displacement;
    int dispResolution;
    vector<ofMeshFace> triangles;
    ofSpherePrimitive sphereDistor;
    float dispNzAmnt;
    bool bDoFaceSh, bDoFaces;
    ofShader faceShader;
    ofShader phongShader;
    
    //gui------------------
    ofxPanel gui;
    ofxFloatSlider velGui;
    ofxFloatSlider volumeGui;
    ofxFloatSlider xGui, yGui;
    ofxFloatSlider radiusGui;
    ofxIntSlider resolGui;
    ofxFloatSlider faceNoiseGui;
    ofxFloatSlider strengthGui;
    ofxFloatSlider zPos;

    
};