
#include "ofxFisheye.h"

void ofxFisheye::setup(fxType type = tVariableFisheye){
    
    
    //load shaders------------
    #ifdef TARGET_OPENGLES
        ofLogError("NOT AVAILABLE SHADERS FOR THIS RENDERER");
    #else
        if(ofIsGLProgrammableRenderer()){
            ofLogError("NOT AVAILABLE SHADERS FOR THIS RENDERER");
        }else{
           
            fixFishShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fixFisheye.gl2FragmentShader);
            fixFishShader.bindDefaults();
            fixFishShader.linkProgram();
            
            barrelShader.setupShaderFromSource(GL_FRAGMENT_SHADER, barrelDist.gl2FragmentShader);
            barrelShader.bindDefaults();
            barrelShader.linkProgram();
            
            varFishShader.setupShaderFromSource(GL_FRAGMENT_SHADER, varFisheye.gl2FragmentShader);
            varFishShader.bindDefaults();
            varFishShader.linkProgram();
            
            ofLogNotice("ofxFisheye: Shaders loaded.");
        }
    #endif
    //------------------------
    setFxType(type);
    
}
//---------------------------------
void ofxFisheye::update(){

}
//---------------------------------
void ofxFisheye::draw(){

}
//--------------------------------
void ofxFisheye::exit(){

}
//---------------------------------
void ofxFisheye::begin(ofTexture& fboTexture, int w, int h, float amount){
    
    float amnt = ofClamp(amount, 0.0, 1.0);;
    
    switch (currentFxType) {
        case tFixFisheye:
            amnt *= 180;//0 a 180
            break;
        case tBarrelDist:
            amnt *= 4.0;//0 a 2
            break;
        case tVariableFisheye:
            amnt = amnt*2 - 1.0;// (-1,1)
            amnt*=0.5;//(-0.5,0.5)
            break;
            
        default:
            break;
    }
    
    
    currentShader->begin();
    currentShader->setUniformTexture("tex", fboTexture, 0);
    currentShader->setUniform1f("width", w);
    currentShader->setUniform1f("height", h);
    currentShader->setUniform1f("amount", amnt);
}
//---------------------------------
void ofxFisheye::end(){
    currentShader->end();
}
//---------------------------------
void ofxFisheye::setFxType(fxType type){
    
    currentFxType = type;
    
    switch (currentFxType) {
            
        case tFixFisheye:
            currentShader = &fixFishShader;
            break;
        case tBarrelDist:
            currentShader = &barrelShader;
            break;
        case tVariableFisheye:
            currentShader = &varFishShader;
            break;
            
        default:
            break;
    }
    
}
//---------------------------------
string ofxFisheye::getFxTypeAsString(){
    
    string s;
    
    switch (currentFxType) {
        case tFixFisheye:
            s = "FIX Fisheye";
            break;
        case tVariableFisheye:
            s = "VARIABLE Fisheye";
            break;
        case tBarrelDist:
            s = "Barrel Distortion";
            break;
            
        default:
            break;
    }
    
    return s;
}


