
#include "SphereManager.h"

void SphereManager::setup(){
    
    displacement.setup();
    dispResolution = 96;
    
    faceShader.setGeometryInputType(GL_TRIANGLE_STRIP);
    faceShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
    faceShader.setGeometryOutputCount(3);
    faceShader.load("entropica/shaders/faces/vert.glsl", "entropica/shaders/faces/frag.glsl", "entropica/shaders/faces/geom.glsl");
    
    phongShader.load("entropica/shaders/phong/phong.vert","entropica/shaders/phong/phong.frag" );
    
    //--------------
    _mainColor = ofColor::white;
    material.setShininess(110.0);
    material.setDiffuseColor(_mainColor);
    
    setupGui();
    

}
//----------------------------------
void SphereManager::update(){
    
    //---------------------------------
    float x, y, vol, rad, res, vel;
    
    float centroid = 0.5;
    float specComp = 0.5;
    
    x   = centroid*.1;
    y   = .001 + specComp*.049;
    vol = 0.5+centroid*0.1;
    rad = 30+specComp*100;
    res = dispResolution;
    vel  = 1;
    
    dispNzAmnt =strengthGui;
    
    displacement.update(xGui, yGui, volumeGui, radiusGui, resolGui, ofGetFrameNum()*velGui);


}
//----------------------------------
void SphereManager::drawScene(){
    
    material.begin();
    
    if (bDoFaceSh) {
        faceShader.begin();
        faceShader.setUniform1f("timeVal", ofGetFrameNum());
        faceShader.setUniform1f("noiseAmnt", dispNzAmnt);
    }else{
        phongShader.begin();
    }
    
    ofPushMatrix();
    ofTranslate(_pos.x, _pos.y, zPos);
    ofRotateX(180.0);
    
    
    bDoFaces ? sphereDistor.draw() : displacement.mainMesh.draw();
    
    
    ofPopMatrix();
    
    
    if (bDoFaceSh){
        faceShader.end();
    }else{
        phongShader.end();
    }
    
    material.end();

    
}
//----------------------------------
void SphereManager::setupGui(){
    
    //cubeSphere------------
    gui.setup("sphere");
    gui.setPosition(200,300);
    gui.add(velGui.setup( "Velocity", 1, 0, 5 ));
    gui.add(volumeGui.setup( "Volume", 0.185, 0, 1 ));
    gui.add(xGui.setup( "X", 0.0815, 0, 0.1 ));
    gui.add(yGui.setup( "Y", 0.06, 0, 0.1 ));
    gui.add(radiusGui.setup( "Radius", 97, 0, 500 ));
    gui.add(resolGui.setup( "Resolution", 96, 1, 100 ));
    gui.add(strengthGui.setup( "Strength", -25, -200, 200 ));
    gui.add(faceNoiseGui.setup( "FaceNoise", 20., 0., 20.));
    gui.add(zPos.setup( "Z pos", 0.0, -300.0, 300.0));
}
//----------------------------------
void SphereManager::drawGui(){
    gui.draw();
}
//----------------------------------
void SphereManager::setColor(ofColor col){
    _mainColor = col;
    material.setDiffuseColor(_mainColor);//??? es diffuse el que va?
}
