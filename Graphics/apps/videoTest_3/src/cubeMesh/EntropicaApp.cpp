#include "EntropicaApp.h"


#pragma mark - OF Core funcs
//--------------------------------------------------------------
void EntropicaApp::setup(ofFbo* iFbo, OscManager* iOsc){
   
    fbo = iFbo;
    osc = iOsc;
    
    ofDisableArbTex();
    
    ///OSC-------------------
    osc->setOnsetTimeTreshold(400.0);
    osc->setUseOnsetMasker(false);
    
    ///postProcessing--------------------------------
    setupPostProcessing(fbo->getWidth(), fbo->getHeight());
    
    ///colors-----------------
    mainColor.set(COLOR_MAIN_ENTROPICA);
    backgroundColor.set(COLOR_BACKGROUND_ENTROPICA);
    //mainColor.set(47, 79, 79);//verde buena onda
    //mainColor.set(255, 0, 150);//verde buena onda
    ///-----------------
    setControlGui();
    _marker = A;
    _subMarker = 0;
    bShowGui = false;
    bPostProcessing = true;
    
    ///Displacement-------------
    displacement.setup();
    dispResolution = 48;
    //cout<<"--DISPLACEMENT SPHERE Setup Finished-----"<<displacement.mainMesh.getNumVertices()<<endl;
    
    ///Metaballs-----------------------
    int metaBallsNum = 75;
    for(int i = 0; i < metaBallsNum; i++) {
		centers_1.push_back(ofVec3f(ofRandomf(), ofRandomf(), ofRandomf()));
        centers_2.push_back(ofVec3f(ofRandomf(), ofRandomf(), ofRandomf()));
	}
    //cout<<"--METABALLS Setup Finished-----"<<endl;
    
    ///CubeMesh--------------------
    sphereCubes.setResolution(36);///
    sphereCubes.setRadius(100);
    cubeMesh = sphereCubes.getMesh();
    cubeMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    //cout<<"--CUBE MESH Setup Finished-----"<<endl;
    
    ///Marching cubes--------------
    mc.setup();
	mc.setResolution(18, 18 , 18);
	mc.scale.set( 500, 500, 500 );
	mc.setSmoothing( false );
    //cout<<"--MARCHING CUBES Setup Finished-----"<<endl;
    
    ///SHADERS------------------------
    //cout<<"--Loading NOISE SHADER...-----"<<endl;
    noiseShader.load("entropica/shaders/disp/disp");
    noiseShader.begin();
    noiseShader.setUniform1i("numLights",1);
    noiseShader.end();
    //-
    //cout<<"--Loading FACE SHADER...-----"<<endl;
    faceShader.setGeometryInputType(GL_TRIANGLE_STRIP);
	faceShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
	faceShader.setGeometryOutputCount(3);
	faceShader.load("entropica/shaders/faces/vert.glsl", "entropica/shaders/faces/frag.glsl", "entropica/shaders/faces/geom.glsl");
    //-
    //cout<<"--Loading CUBE SHADER...-----"<<endl;
    cubeShader.setGeometryInputType(GL_TRIANGLE_STRIP);
	cubeShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
	cubeShader.setGeometryOutputCount(24);
	cubeShader.load("entropica/shaders/cubes/vert.glsl", "entropica/shaders/cubes/frag.glsl", "entropica/shaders/cubes/geom.glsl");
    //-
    //cout<<"--Loading PHONG SHADER...-----"<<endl;
    phongSahder.load("entropica/shaders/phong/phong.vert","entropica/shaders/phong/phong.frag" );
    ///--------------------------
    
    animValue.setCurve(EASE_IN);
	animValue.setRepeatType(PLAY_ONCE);
	animValue.setDuration(1.00);
    animValueCam.setCurve(EASE_IN);
	animValueCam.setRepeatType(PLAY_ONCE);
	animValueCam.setDuration(1.00);
    
    bUseFixCam = true;
    
    ofSetSmoothLighting(true);
    light.setPosition(500, 500, 500);
    light.setDirectional();
    light.enable();
    
    material.setShininess( 100 );    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setSpecularColor(ofColor(255.f, 255.f, 255.f, 255.f));
   
    
    material.setDiffuseColor(mainColor);


    ///--------------------------------------
    cout<<"-----SETUP FINISHED-----"<<endl;
    
}

//--------------------------------------------------------------

void EntropicaApp::update(){
    
    float dt = 1.0f / ofGetFrameRate();
    animValue.update(dt * _animDuration);
    animValueCam.update(dt * _animCamDuration);
    
    
#pragma mark  Value Mapping
    if (bReceiveOSCGui) {
        
        bDoDisplacement = bDoMetaball =
        bDoMarching = bDoFaces = bDoCubeMesh =  bDoNoiseShader = false;
        
        confTresh = 0.6;
        
        if (_marker == A){
            if (_subMarker == 0) {
                bDoDisplacement = true;
                //-
                bDoMetaball = false;
                bDoMarching = false;
                bDoFaces = false;
                bDoCubeMesh = false;
                bDoNoiseShader  = false;
                
                centroid = ofMap(osc->ch1.centroid, 0, 7000.0, 0.0, 1.0);
                specComp = ofMap(osc->ch2.complx, 0.0, 30.0, 0.0, 1.0);
                pitchConf = 0.0;
                specCompB =  ofMap(osc->ch1.complx, 0.0, 30.0, 0.0, 1.0);
            }
            else if (_subMarker == 1){
                bDoDisplacement = true;
                //-
                bDoMetaball = false;
                bDoMarching = false;
                bDoFaces = false;
                bDoCubeMesh = false;
                bDoNoiseShader = false;
                
                centroid = ofMap(osc->ch2.centroid, 0, 7000.0, 0.0, 1.0);
                specComp = ofMap(osc->ch1.complx, 0.0, 30.0, 0.0, 1.0);
                pitchConf = 0.0;
                specCompB =  ofMap(osc->ch2.complx, 0.0, 30.0, 0.0, 1.0);
                
            }
            else if (_subMarker == 2){
                bDoDisplacement = true;
                bDoFaces        = true;
                //-
                bDoMetaball = false;
                bDoMarching = false;
                bDoCubeMesh = false;
                bDoNoiseShader = false;
                
                centroid = ofMap(osc->ch1.centroid, 0.0, 7000.0, 0.0, 1.0);
                specComp = ofMap(osc->ch2.complx, 0.0, 30.0, 0.0, 1.0);
                specCompB = ofMap(osc->ch1.complx, 0.0, 30.0, 0.0, 1.0);
                
                if(osc->ch2.confidence > confTresh) pitchConf = ofMap(osc->ch2.confidence, confTresh, 1.0, 0.0, 1.0 );
                else pitchConf = 0.0;
                
            }
            else if (_subMarker == 3){
                bDoDisplacement = true;
                bDoFaces        = true;
                //-
                bDoMetaball = false;
                bDoMarching = false;
                bDoCubeMesh = false;
                bDoNoiseShader = false;
                
                if(osc->ch1.centroid > osc->ch2.centroid) centroid = ofMap(osc->ch1.centroid, 0.0, 7000.0, 0.0, 1.0);
                else centroid = ofMap(osc->ch2.centroid, 0, 7000.0, 0.0, 1.0);
                
                if(osc->ch1.complx > osc->ch2.complx){
                    specComp = ofMap(osc->ch1.complx, 0.0, 30.0, 0.0, 1.0);
                    specCompB = ofMap(osc->ch2.complx, 0.0, 30.0, 0.0, 1.0);
                }
                else{
                    specComp = ofMap(osc->ch2.complx, 0.0, 30.0, 0.0, 1.0);
                    specCompB = ofMap(osc->ch1.complx, 0.0, 30.0, 0.0, 1.0);
                }
                
                if(osc->ch1.confidence > osc->ch2.confidence && osc->ch1.confidence > confTresh){
                    pitchConf = ofMap(osc->ch1.confidence, confTresh, 1.0, 0.0, 1.0 );
                }else if (osc->ch2.confidence >= osc->ch1.confidence && osc->ch2.confidence > confTresh){
                    pitchConf = ofMap(osc->ch2.confidence, confTresh, 1.0, 0.0, 1.0 );
                }else{
                    pitchConf = 0.0;
                }
            }
        }
        else if (_marker == B){
            
            bDoMetaball = true;
            bDoNoiseShader  = true;
            //-
            if(1-animValue.val() && _subMarker==0){
                bDoDisplacement = true;
                //bDoFaces        = false;
            }
            else{
                bDoDisplacement = false;
            }
            bDoFaces        = false;
            bDoMarching     = false;
            bDoCubeMesh     = false;
            
            pitchFreq1 = ofMap(osc->ch1.pitch, 130, 1500, 0.0, 1.0);
            pitchFreq2 = ofMap(osc->ch2.pitch, 130, 1500, 0.0, 1.0);
            pitchFreq = .5 * (pitchFreq1 + pitchFreq2 );
            
            pitchConf1 = osc->ch1.confidence;
            pitchConf2 = osc->ch2.confidence;
            pitchConf = .5 * (osc->ch1.confidence + osc->ch2.confidence);
            
            power1 = ofMap(osc->ch1.power, log10(0.000001), log10(1.0), 0.0, 1.0); //ch1
            power2 = ofMap(osc->ch2.power, log10(0.000001), log10(1.0), 0.0, 1.0); //ch2
            power = .5 * (power1+power2);
            
            centroid1 = ofMap(osc->ch1.centroid, 0, 7000.0, 0.0, 1.0);
            centroid2 = ofMap(osc->ch2.centroid, 0, 7000.0, 0.0, 1.0);
            centroid  = .5 * (centroid1+centroid2);
            
            specComp1 = ofMap(osc->ch1.complx, 0.0, 30.0, 0.0, 1.0);
            specComp2 = ofMap(osc->ch2.complx, 0.0, 30.0, 0.0, 1.0);
            specComp = .5 * (specComp1+specComp2);
            
            
        }
        else if (_marker == C){
            
            if(_subMarker == 0){
                
                if(1-animValue.val()){
                    bDoNoiseShader=bDoMetaball = true;
                    bDoCubeMesh = false;
                }
                else{
                    bDoMetaball=bDoNoiseShader=false;
                    bDoCubeMesh = true;
                }
                //-
                bDoMarching = false;
                bDoFaces = false;
                bDoDisplacement = false;
                
                power1 = ofMap(osc->ch1.power, log10(0.000001), log10(1.0), 0.0, 1.0); //ch1
                centroid = ofMap(osc->ch1.centroid, 0, 7000.0, 0.0, 1.0); //ch1
                
                power2 = ofMap(osc->ch2.power, log10(0.000001), log10(1.0), 0.0, 1.0); //ch2
                pitchConf = osc->ch2.confidence; //ch2
                
            }
            else if(_subMarker == 1){
                
                bDoCubeMesh = true;
                //-
                bDoMetaball=bDoNoiseShader=false;
                //-
                bDoMarching = false;
                bDoFaces = false;
                bDoDisplacement = false;
                
                centroid = ofMap(osc->ch1.centroid, 0, 7000.0, 0.0, 1.0); //ch1
                //power-------------
                power1 = ofMap(osc->ch1.power, log10(0.000001), log10(1.0), 0.0, 1.0); //ch1
                power2 = ofMap(osc->ch2.power, log10(0.000001), log10(1.0), 0.0, 1.0); //ch2
                //confidence-------
                pitchConf = .5 * (osc->ch1.confidence + osc->ch2.confidence);
                //pitch-----------
                pitchFreq1 = ofMap(osc->ch1.pitch, 130, 1500, 0.0, 1.0);
                pitchFreq2 = ofMap(osc->ch2.pitch, 130, 1500, 0.0, 1.0);
                pitchFreq = .5 * (pitchFreq1 + pitchFreq2 );
                
            }
            else if(_subMarker == 2){
                bDoCubeMesh = true;
                bDoMarching = true;
                //-
                bDoMetaball = false;
                bDoFaces = false;
                bDoDisplacement = false;
                bDoNoiseShader = false;
                
                pitchFreq1 = ofMap(osc->ch1.pitch, 130, 1500, 0.0, 1.0);
                pitchFreq2 = ofMap(osc->ch2.pitch, 130, 1500, 0.0, 1.0);
                
                pitchFreq = .5 * (pitchFreq1 + pitchFreq2 );
                
                power1 = ofMap(osc->ch1.power, log10(0.000001), log10(1.0), 0.0, 1.0); //ch1
                power2 = ofMap(osc->ch2.power, log10(0.000001), log10(1.0), 0.0, 1.0); //ch2
                power = .5 * (power1+power2);
                
                centroid1 = ofMap(osc->ch1.centroid, 0, 7000.0, 0.0, 1.0);
                centroid2 = ofMap(osc->ch2.centroid, 0, 7000.0, 0.0, 1.0);
                centroid  = .5 * (centroid1+centroid2);
                
            }
        }
        else if (_marker == D){
            
            bDoDisplacement = true;
            bDoCubeMesh = true;
            
            if(_subMarker==0){
                bDoMarching = false;
            }
            else if(_subMarker==1){
                if(pitchConf>0.6) bDoMarching = true;
                else bDoMarching = false;
            }
            else if(_subMarker==2 || _subMarker==3){
                bDoMarching = true;
            }
            
            //-
            bDoMetaball = false;
            bDoFaces = false;
            bDoNoiseShader = false;
            
            
            pitchFreq1 = ofMap(osc->ch1.pitch, 130, 1500, 0.0, 1.0);
            pitchFreq2 = ofMap(osc->ch2.pitch, 130, 1500, 0.0, 1.0);
            pitchFreq = .5 * (pitchFreq1 + pitchFreq2 );
            
            pitchConf1 = osc->ch1.confidence;
            pitchConf2 = osc->ch2.confidence;
            pitchConf = .5 * (osc->ch1.confidence + osc->ch2.confidence);
            
            power1 = ofMap(osc->ch1.power, log10(0.000001), log10(1.0), 0.0, 1.0); //ch1
            power2 = ofMap(osc->ch2.power, log10(0.000001), log10(1.0), 0.0, 1.0); //ch2
            power = .5 * (power1+power2);
            
            centroid1 = ofMap(osc->ch1.centroid, 0, 7000.0, 0.0, 1.0);
            centroid2 = ofMap(osc->ch2.centroid, 0, 7000.0, 0.0, 1.0);
            centroid  = .5 * (centroid1+centroid2);
            
            specComp1 = ofMap(osc->ch1.complx, 0.0, 30.0, 0.0, 1.0);
            specComp2 = ofMap(osc->ch2.complx, 0.0, 30.0, 0.0, 1.0);
            specComp = .5 * (specComp1+specComp2);
            
            hfc1 = ofMap(osc->ch1.hfc, 0.0, 1000.0, 0.0, 1.0);
            hfc2 = ofMap(osc->ch2.hfc, 0.0, 1000.0, 0.0, 1.0);
            
            
        }
        else if (_marker == Z){
            if(animValue.isOrWillBeAnimating()){
                
                bDoDisplacement = true;
                bDoCubeMesh = true;
                bDoMarching = true;
                
                bDoMetaball = false;
                bDoFaces = false;
                bDoNoiseShader = false;
                
                //--------------------------------------------------
                
                pitchFreq1 = ofMap(osc->ch1.pitch, 130, 1500, 0.0, 1.0);
                pitchFreq2 = ofMap(osc->ch2.pitch, 130, 1500, 0.0, 1.0);
                pitchFreq = .5 * (pitchFreq1 + pitchFreq2 );
                
                pitchConf1 = osc->ch1.confidence;
                pitchConf2 = osc->ch2.confidence;
                pitchConf = .5 * (osc->ch1.confidence + osc->ch2.confidence);
                
                power1 = ofMap(osc->ch1.power, log10(0.000001), log10(1.0), 0.0, 1.0); //ch1
                power2 = ofMap(osc->ch2.power, log10(0.000001), log10(1.0), 0.0, 1.0); //ch2
                power = .5 * (power1+power2);
                
                centroid1 = ofMap(osc->ch1.centroid, 0, 7000.0, 0.0, 1.0);
                centroid2 = ofMap(osc->ch2.centroid, 0, 7000.0, 0.0, 1.0);
                centroid  = .5 * (centroid1+centroid2);
                
                specComp1 = ofMap(osc->ch1.complx, 0.0, 30.0, 0.0, 1.0);
                specComp2 = ofMap(osc->ch2.complx, 0.0, 30.0, 0.0, 1.0);
                specComp = .5 * (specComp1+specComp2);
                
                hfc1 = ofMap(osc->ch1.hfc, 0.0, 1000.0, 0.0, 1.0);
                hfc2 = ofMap(osc->ch2.hfc, 0.0, 1000.0, 0.0, 1.0);
            }
            else{
                bDoDisplacement = false;
                bDoMetaball = false;
                bDoMarching = false;
                bDoFaces = false;
                bDoCubeMesh = false;
                bDoNoiseShader  = false;
            }
            
        }
    }
    else{
        bDoDisplacement = bDoDisplacementGui;
        bDoMetaball = bDoMetaballGui;
        bDoMarching = bDoMarchingGui;
        bDoFaces = bDoFacesGui;
        bDoCubeMesh = bDoCubeMeshGui;
        bDoNoiseShader = bDoShaderGui;
    }
    
    
#pragma mark  Displacement
    
    TIME_SAMPLE_START("Displaced Sphere");
    if(bDoDisplacement){
        
        if(bReceiveOSCGui){
            
            float x, y, vol, rad, res, vel;
            //dispResolution = 48;
            
            if (_marker==A) {
                x   = centroid*.1;
                y   = .001 + specComp*.049 + specCompB*0.02;
                vol = 0.5+centroid*0.1;
                rad = 30+specComp*100;
                res = dispResolution;
                vel  = 1;
            }
            else if (_marker==B){
                float _centroid, _specComp, _specCompB;
                
                if(osc->ch1.centroid > osc->ch2.centroid) _centroid = ofMap(osc->ch1.centroid, 0.0, 7000.0, 0.0, 1.0);
                else _centroid = ofMap(osc->ch2.centroid, 0, 7000.0, 0.0, 1.0);
                
                if(osc->ch1.complx > osc->ch2.complx){
                    _specComp = ofMap(osc->ch1.complx, 0.0, 30.0, 0.0, 1.0);
                    _specCompB = ofMap(osc->ch2.complx, 0.0, 30.0, 0.0, 1.0);
                }
                else{
                    _specComp = ofMap(osc->ch2.complx, 0.0, 30.0, 0.0, 1.0);
                    _specCompB = ofMap(osc->ch1.complx, 0.0, 30.0, 0.0, 1.0);
                }
                
                if(osc->ch1.confidence > osc->ch2.confidence && osc->ch1.confidence > confTresh){
                    pitchConf = ofMap(osc->ch1.confidence, confTresh, 1.0, 0.0, 1.0 );
                }else if (osc->ch2.confidence >= osc->ch1.confidence && osc->ch2.confidence > confTresh){
                    pitchConf = ofMap(osc->ch2.confidence, confTresh, 1.0, 0.0, 1.0 );
                }else{
                    pitchConf = 0.0;
                }
                
                
                if(animValue.isOrWillBeAnimating()){
                    rad = trans(30+specComp*100, 0.0, animValue.val());
                    y = trans(.001 + _specComp*.049 + _specCompB*0.02, 0.0, animValue.val());
                }
                else{
                    rad = 0.0;
                }
                
                x   = _centroid*.1;
                vol = 0.5+_centroid*0.1;
                res = dispResolution;
                vel  = 1;
                
            }
            else if (_marker==D){
                
                if (_subMarker == 0) {
                    
                    x   = centroid*.1;
                    y   = (.05+specComp*.05)*.5;
                    vol = (0.5+centroid*0.1)*.35;
                    res = dispResolution;
                    vel  = 1;
                    //fade
                    if(animValue.isOrWillBeAnimating()){
                        rad = trans(0, (0.5+centroid*0.1)*.75, animValue.val());
                    }else{
                        rad = 30+specComp*100;
                    }
                    
                    bDoFaceSh = true;
                    dispNzAmnt = 300 - centroid * 450;
                    if(dispNzAmnt<0) dispNzAmnt = 0;
                    
                }
                else if (_subMarker == 1){
                    
                    res = dispResolution;
                    vel  = 1;
                    
                    if(animValue.isOrWillBeAnimating()){
                        float tr = animValue.val();
                        x = trans(centroid*.1, centroid*.15, tr);
                        y = trans(specComp*.05, specComp*.1, tr);
                        vol = trans((0.5+centroid*0.1)*.35, vol = 0.5+centroid*0.5, tr); ///--->paraVivo
                        ///vol = trans((0.5+centroid*0.1)*.35, vol = 0.5+centroid*0.25, tr); ///--->paraGrabacionEstudio
                        rad = trans(30+specComp*100, 15+specComp*100, tr);
                        
                        dispNzAmnt = trans(300 - centroid * 450, 0.0, tr);
                        if(dispNzAmnt<0) dispNzAmnt = 0;
                        
                    }else{
                        x   = centroid*.15;
                        y   = specComp*.1;
                        vol = 0.5+centroid*0.5; ///--->paraVivo
                        ///vol = 0.5+centroid*0.25; ///--->paraGrabacion estudio
                        rad = 15+specComp*100;
                        
                        
                        if(pitchConf>0.6){
                            bDoFaceSh = true;
                            dispNzAmnt=pitchConf*400;
                        }else{
                            bDoFaceSh = false;
                            dispNzAmnt=0.0;
                        }
                    }
                }
                else if (_subMarker == 2){
                    
                    res = dispResolution;
                    vel  = 1;
                    bDoFaceSh = true;
                    
                    if(animValue.isOrWillBeAnimating()){
                        float tr = animValue.val();
                        x = trans(centroid*.15, pitchFreq*.5, tr);
                        y = trans(specComp*.1, pitchConf*.05, tr);
                        vol = trans(0.5+centroid*0.5, (0.5+pitchConf*0.1)*.5, tr); ///--->paraVivo
                        ///vol = trans(0.5+centroid*0.25, (0.5+pitchConf*0.1)*.5, tr); ///--->paraGrabacionEstudio
                        rad = trans(15+specComp*100, (30+pitchFreq*100)*.5, tr);
                        dispNzAmnt = trans(pitchConf*400, pitchFreq*50, tr);
                        
                    }else{
                        x   = pitchFreq*.5;
                        y   = pitchConf*.05;
                        vol = (0.5+pitchConf*0.1)*.5;
                        rad = (30+pitchFreq*100)*.5;
                        dispNzAmnt=pitchFreq*50 ;
                    }
                }
                else if (_subMarker == 3){
                    bDoFaceSh = true;
                    rad = (30+pitchFreq*100)*.5;
                    res = dispResolution*.75;
                    vel  = 1;
                    
                    
                    if(animValue.isOrWillBeAnimating()){
                        float tr = animValue.val();
                        x = trans(pitchFreq*.5, pitchConf*.1, tr);
                        y = trans(pitchConf*.05, pitchFreq*.05, tr);
                        vol = trans((0.5+pitchConf*0.1)*.5, (0.5+power*0.1)*.5, tr);
                        dispNzAmnt = trans(pitchFreq*50, pitchFreq * 150, tr);
                        
                    }else{
                        x   = pitchConf*.1;
                        y   = pitchFreq*.05;
                        vol = (0.5+power*0.1)*.5;
                        dispNzAmnt = pitchFreq * 150;
                        
                    }
                }
            }
            else if(_marker==Z){
                
                res = dispResolution*.75;
                vel  = 1;
                
                if(animValue.isOrWillBeAnimating()){
                    float tr = animValue.val();
                    x = trans(pitchConf*.1, 0.0, tr);
                    y = trans(pitchFreq*.05, 0.0, tr);
                    vol = trans((0.5+power*0.1)*.5, 0.0, tr);
                    rad = trans((30+pitchFreq*100)*.5, 0.0, tr);
                    dispNzAmnt = trans(pitchFreq * 150, 0.0, tr);
                    bDoFaceSh = true;
                    
                }else{
                    x   = 0.0;
                    y   = 0.0;
                    vol = 0.0;
                    rad = 0.0;
                    dispNzAmnt = 0.0;
                    bDoFaceSh = false;
                }
                
            }
            //----------------
            displacement.update(x, y, vol, rad, res, ofGetFrameNum()*vel);///<--
            //----------------
        }else{
            displacement.update(xGui, yGui, volumeGui, radiusGui, resolGui, ofGetFrameNum()*velGui);
        }
    }
    TIME_SAMPLE_STOP("Displaced Sphere");
    
    
#pragma mark  Metaballs
    
    TIME_SAMPLE_START("Metaball");
    static int metaCnt_1, metaCnt_2 ;
    if(bDoMetaball){
        
        if(bReceiveOSCGui){
            if(_marker == B){
                if (_subMarker==0) {
                    
                    if(animValue.isOrWillBeAnimating()){
                        float tr = animValue.val();
                        metaScale_1 = trans(0.0,  pitchConf1*500, tr);
                        metaShY_1 = trans(0.0,   0.02-pitchConf1*0.0125, tr);
                        metaMaxRadius_1 = trans(0.0, 100-pitchConf1*.75*100, tr);
                        metaShVol_1 = trans(0.0,  0.4-pitchConf1*0.1, tr);
                        metaTr_1 = trans(0.0, power1*350, tr);
                        //-
                        metaScale_2 = trans(0.0,  pitchConf2*500, tr);
                        metaShY_2 = trans(0.0,   0.02-pitchConf2*0.0125, tr);
                        metaMaxRadius_2 = trans(0.0, 100-pitchConf2*.75*100, tr);
                        metaShVol_2 = trans(0.0,  0.4-pitchConf2*0.1, tr);
                        metaTr_2 = trans(0.0, power2*350, tr);
                    }
                    else{
                        metaScale_1 = pitchConf1*500;
                        metaShY_1 = 0.02-pitchConf1*0.0125;
                        metaMaxRadius_1 = 100-pitchConf1*.75*100;
                        metaShVol_1 = 0.4-pitchConf1*0.1;
                        metaTr_1 = power1*350;///----
                        //metaTr_1 = (power1*5+pitchConf1)*50;
                        
                        metaScale_2 = pitchConf2*500;
                        metaShY_2 = 0.02-pitchConf2*0.0125;
                        metaMaxRadius_2 = 100-pitchConf2*.75*100;
                        metaShVol_2 = 0.4-pitchConf2*0.1;
                        metaTr_2 = power2*350;
                    }
                    
                    //channel--1******************************************
                    metaVel_1 = (pitchConf1*3+power1 +pitchFreq1*2)*1.0;
                    metaShX_1 = 0.02;
                    metaShVel_1 = pitchConf1*15.0;
                    //channel--2******************************************
                    metaVel_2 = (pitchConf2*3+power2 +pitchFreq2*2)*1.0;
                    metaShX_2 = 0.02;
                    metaShVel_2 = pitchConf2*15.0;
                    
                }
                else if(_subMarker==1){
                    
                    if(animValue.isOrWillBeAnimating()){
                        float tr = animValue.val();
                        
                        //channel1**************************************************
                        metaScale_1 = trans(pitchConf1*500, pitchConf1*1000, tr);
                        metaVel_1   = trans((pitchConf1*3+power1+pitchFreq1*2)*1.0, pitchConf1*2+pitchFreq1, tr);
                        metaMaxRadius_1 = trans(100-pitchConf1*.75*100, 100-pitchConf1*.85*100, tr);
                        //-
                        metaShX_1 = 0.02;;
                        metaShY_1 = 0.02-pitchConf1*0.0125;
                        metaShVel_1 = pitchConf1*15.0;
                        metaShVol_1 = trans(0.4-pitchConf1*0.1,0.4-pitchConf1*0.175, tr);
                        //-
                        metaTr_1 = trans(power1*350, pitchFreq1 * 150, tr);
                        
                        //channel2**************************************************
                        metaScale_2 = trans(pitchConf2*500, pitchConf1*1000, tr);
                        metaVel_2   = trans((pitchConf2*3+power2+pitchFreq2*2)*1.0, pitchConf2*2+ pitchFreq2, tr);
                        metaMaxRadius_2 = trans(100-pitchConf2*.75*100, 100-pitchConf2*.85*100, tr);
                        //-
                        metaShX_2 = 0.02;;
                        metaShY_2 = 0.02-pitchConf2*0.0125;
                        metaShVel_2 = pitchConf2*15.0;
                        metaShVol_2 = trans(0.4-pitchConf2*0.1,0.4-pitchConf2*0.175, tr);
                        //-
                        metaTr_2 = trans(power2*350, pitchFreq2 * 150, tr);
                        
                        
                    }
                    else{
                        //channel--1***********************************
                        metaScale_1 = pitchConf1*1000;
                        metaVel_1 = pitchConf1*2+pitchFreq1;
                        
                        metaMaxRadius_1 = 100-pitchConf1*.85*100;
                        //-
                        metaShX_1 = 0.02;
                        metaShY_1 = 0.02-pitchConf1*0.0125;
                        metaShVel_1 = pitchConf1*15.0;
                        metaShVol_1 = 0.4-pitchConf1*0.175;//0.2
                        //-
                        metaTr_1 = pitchFreq1 * 150;
                        //channel--2*************************************
                        metaScale_2 = pitchConf2 * 1000;
                        metaVel_2 = pitchConf2*2 + pitchFreq2;
                        metaMaxRadius_2 = 100 - pitchConf2 * .85 * 100;
                        //-
                        metaShX_2 = 0.02;
                        metaShY_2 = 0.02-pitchConf2*0.0125;
                        metaShVel_2 = pitchConf2*15.0;
                        metaShVol_2 = 0.4 - pitchConf2*0.175;
                        //-
                        metaTr_2 = pitchFreq2 * 150;
                        
                    }
                    
                }
                else if(_subMarker==2){
                    
                    if(animValue.isOrWillBeAnimating()){
                        float tr = animValue.val();
                        //channel--1***************************************************
                        metaScale_1 = trans(pitchConf1*1000, pitchConf1*600, tr);;
                        metaVel_1 = trans(pitchConf1*2+pitchFreq1, (pitchConf1*2+power1)*1.0, tr);
                        metaMaxRadius_1 = trans(100-pitchConf1*.85*100, 100-pitchConf1*.7*100, tr);
                        //-
                        metaShX_1 = 0.02;
                        metaShY_1 =  0.02-pitchConf1*0.0125;
                        metaShVel_1 = pitchConf1*15.0;
                        metaShVol_1 = trans(0.4-pitchConf1*0.175, 0.4-pitchConf1*0.05, tr);
                        //-
                        metaTr_1 = trans(pitchFreq1*150, (power1*3+pitchConf1)*100, tr);
                        
                        //channel--2***************************************************
                        metaScale_2 = trans(pitchConf2*1000, pitchConf2*600, tr);;
                        metaVel_2 = trans(pitchConf2*2+pitchFreq2, (pitchConf2*2+power2)*1.0, tr);
                        metaMaxRadius_2 = trans(100-pitchConf2*.85*100, 100-pitchConf2*.7*100, tr);
                        //-
                        metaShX_2 = 0.02;
                        metaShY_2 =  0.02-pitchConf2*0.0125;
                        metaShVel_2 = pitchConf2*15.0;
                        metaShVol_2 = trans(0.4-pitchConf2*0.175, 0.4-pitchConf2*0.05, tr);
                        //-
                        metaTr_2 = trans(pitchFreq2*150, (power2*3+pitchConf2)*100, tr);
                    }
                    else{
                        //channel--1******************************************
                        metaScale_1 = pitchConf1*600;
                        
                        metaMaxRadius_1 = 100-pitchConf1*.7*100;
                        metaVel_1 = (pitchConf1*2+power1)*1.0;
                        
                        metaShVol_1 = 0.4-pitchConf1*0.05;
                        metaShX_1 = 0.02;
                        metaShY_1 = 0.02-pitchConf1*0.0125;
                        metaShVel_1 = pitchConf1*15.0;
                        
                        metaTr_1 = (power1*3+pitchConf1)*100;
                        
                        //channel--2******************************************
                        metaScale_2 = pitchConf2*600;
                        metaVel_2 = (pitchConf2*2+power2)*1.0;
                        metaMaxRadius_2 = 100-pitchConf2*.7*100;
                        
                        metaShX_2 = 0.02;
                        metaShY_2 = 0.02-pitchConf2*0.0125;
                        metaShVel_2 = pitchConf2*15.0;
                        metaShVol_2 = 0.4-pitchConf2*0.05;
                        
                        metaTr_2 = (power2*3+pitchConf2)*100;
                        
                    }
                }
            }
            else if (_marker==C){
                
                if(animValue.isOrWillBeAnimating()){
                    float tr = animValue.val();
                    
                    metaScale_1 = trans(pitchConf1*600, 0.0, tr);
                    metaShX_1= trans(0.02, 0.0, tr);
                    metaShY_1 = trans(0.02-pitchConf2*0.0125, 0.0, tr);
                    metaMaxRadius_1 = trans(100-pitchConf1*.7*100, 0.0, tr);
                    metaShVol_1 = trans( 0.4-pitchConf1*0.05, 0.0, tr);
                    metaTr_1   = trans((power1*3+pitchConf1)*100, 0.0, tr);
                    //-
                    metaScale_2 = trans(pitchConf2*600, 0.0, tr);
                    metaShX_2 = trans(0.02, 0.0, tr);
                    metaShY_2 = trans(0.02-pitchConf2*0.0125, 0.0, tr);
                    metaMaxRadius_2 = trans(100-pitchConf2*.7*100, 0.0, tr);
                    metaShVol_2 = trans( 0.4-pitchConf2*0.05, 0.0, tr);
                    metaTr_2    = trans((power2*3+pitchConf2)*100, 0.0, tr);
                    
                    
                }else{
                    metaScale_1 = 0.0;
                    metaScale_2 = 0.0;
                }
                
                //channel--1
                metaVel_1 = (power1+pitchConf1)*2;
                metaShVel_1 = pitchConf1*15.0;
                //channel--2
                metaVel_2 = (power2+pitchConf2)*2;
                metaShVel_2 = pitchConf2*15.0;
            }
        }
        else{
            
            metaVel_1 = metaVelGui;
            minRadius = minRadiusGui;
            metaMaxRadius_1 = maxRadiusGui*100;
            metaScale_1 = cubeScaleGui;
            metaScale_1 = cubeScaleGui;
            
            metaShX_1 = xShGui;
            metaShY_1 = yShGui;
            metaShVol_1 = volumeShGui;
            metaShVel_1 = velShGui;
            
        }
        
        //UPDATE CENTERS POS--------------------------------------------
        //ch1---------------------------
        for (int i=0; i<centers_1.size(); i++){
            centers_1[i].x =  ofSignedNoise(metaCnt_1*.0025, i, centers_1[i].y*0.1 ) *.5;
            centers_1[i].y =  ofSignedNoise(metaCnt_1*.0017, i, centers_1[i].z*0.22) *.5;
            centers_1[i].z =  ofSignedNoise(metaCnt_1*.005 , i, centers_1[i].x*0.3 ) *.5;
        }
        
        //chÁ2---------------------------
        for (int i=0; i<centers_2.size(); i++){
            centers_2[i].x =  ofSignedNoise(metaCnt_2*.0025, i, centers_2[i].y*0.1 ) *.5;
            centers_2[i].y =  ofSignedNoise(metaCnt_2*.0017, i, centers_2[i].z*0.22) *.5;
            centers_2[i].z =  ofSignedNoise(metaCnt_2*.005 , i, centers_2[i].x*0.3 ) *.5;
        }
        
        metaCnt_1 += (1 + metaVel_1);
        metaCnt_2 += (1 + metaVel_2);
        
    }
    TIME_SAMPLE_STOP("Metaball");
    
    
#pragma mark Cube Mesh
    
    TIME_SAMPLE_START("Cube Mesh");
    if(bDoCubeMesh){
        if(bReceiveOSCGui){
            
            if(_marker == C){
                if(_subMarker == 0){
                    
                    if (animValue.isOrWillBeAnimating()){
                        float tr = animValue.val();
                        cubeX = trans(0, centroid*.05, tr);
                        cubeY = trans(0 * pitchConf, pitchConf*.1, tr);
                        cubeVel = trans(0, .6 + .4*pitchConf, tr);
                        cubeVol = trans(0 * pitchConf, .5 + 1. * power1, tr);
                        cubeRadius = trans(0, (power1+power2) * 100, tr);
                        cubeSize = trans(0, 8.0 - pitchConf*5, tr);
                    }
                    else{
                        cubeX = centroid*.05;
                        cubeY = pitchConf*.1;
                        cubeVel = .6 + .4*pitchConf;
                        cubeVol = .5 + 1. * power1;
                        cubeRadius = (power1+power2) * 100;
                        cubeSize = 8.0 - pitchConf*6;
                    }
                    
                }
                else if (_subMarker == 1){
                    
                    if (animValue.isOrWillBeAnimating()){
                        float tr = animValue.val();
                        cubeX = trans(centroid*.05, pitchFreq* .05, tr);
                        cubeY = trans(pitchConf*.1, .05+(power1+power2)*.05, tr);
                        cubeVel = trans(.6 + .4*pitchConf, .7 + .35*(power1+power2), tr);
                        cubeVol = trans(.5 + 1. * power1, .6 + (.6 * pitchFreq), tr);
                        cubeSize = trans(8.0 - pitchConf*6, 4.0 - pitchFreq*3, tr);
                        cubeRadius = trans((power1+power2) * 100, (power1+power2)* 200, tr) ;
                    }
                    else{
                        // cubeX = (power1+power2) * .05; ///-
                        cubeX = pitchFreq* .05;
                        cubeY = .05+(power1+power2)*.05;
                        cubeVel = .7 + .35*(power1+power2);
                        cubeVol = .6 + .6 * pitchFreq;
                        cubeRadius = (power1+power2) * 200;
                        cubeSize = 4.0 - pitchFreq*3.0;
                        //cubeX = pitchFreq* .05; ///-
                        //cubeVol = (.6 + (.6 * pitchFreq)*2) * 1.2; ///-
                        
                    }
                    
                }
                else if (_subMarker == 2){
                    if (animValue.isOrWillBeAnimating()){
                        float tr = animValue.val();
                        cubeX = trans(pitchFreq* .05, 0.4, tr);
                        cubeY = trans(pitchConf*.1, 0.075 + power * 0.1, tr);
                        cubeVel = trans(.7 + .35*(power1+power2), power, tr);
                        cubeVol = trans((.6 + .6 * pitchFreq) * 1.0, 1 - centroid, tr);
                        cubeRadius = trans((power1+power2) * 200, 250 - centroid*250, tr);
                        cubeSize = trans(4.0 - pitchFreq*3.0, 1 + centroid*3, tr);
                    }
                    else{
                        cubeX = 0.4;
                        cubeY = 0.075 + power * 0.1 ;
                        cubeVel = power;
                        cubeVol = 1 - centroid;
                        cubeRadius = 250 - centroid*250;
                        cubeSize = 1.0 + centroid*3;///
                    }
                    
                }
            }
            else if (_marker == D){
                
                if (_subMarker == 0){
                    
                    if (animValue.isOrWillBeAnimating()) {
                        float tr = animValue.val();
                        cubeX = trans(0.4, 0.4, tr);
                        cubeY = trans(0.075 + power * 0.1, 0.5 + centroid * 0.5, tr);
                        cubeVel = trans(power, power*.5, tr);
                        cubeVol = trans(1 - centroid, centroid*.5, tr);
                        cubeRadius = trans(250 - centroid*250, 50+power*200, tr);
                        cubeSize = trans(1.0 + centroid*3, 4-centroid*3.5, tr);
                    }else{
                        cubeX = 0.4;
                        cubeY = 0.5 + centroid * 0.5 ;///*
                        cubeVel = power*.5;//
                        cubeVol = centroid*.5;
                        cubeRadius = 50+power*200;
                        cubeSize = 4-centroid*3.5;
                    }
                    
                }
                else if (_subMarker == 1){
                    if (animValue.isOrWillBeAnimating()){
                        float tr = animValue.val();
                        cubeX = trans(0.4, centroid*.05, tr);
                        cubeY = trans(0.5 + centroid * 0.5 , centroid*.1, tr);
                        cubeVel = trans(power*.5, power*.5, tr);
                        cubeVol = trans(centroid*.5, .5 + centroid*.2, tr);
                        cubeRadius = trans(50+power*200, 100 + power * 400, tr);
                        cubeSize = trans(4-centroid*3.5, 4 - centroid*3, tr);
                    }
                    else{
                        cubeX = centroid*.05;
                        pitchConf<0.6 ? cubeY=centroid*.1 : cubeY=pitchConf*.5;
                        cubeVel = power*.5;
                        cubeVol = .5 + centroid*.2;
                        cubeRadius = 100 + power * 400;
                        pitchConf<0.6 ? cubeSize=4-centroid*3: cubeSize=4+centroid*2;
                    }
                    
                }
                else if (_subMarker == 2){
                    
                    if (animValue.isOrWillBeAnimating()){
                        float tr = animValue.val();
                        cubeX = centroid*.05;
                        cubeY = trans(pitchConf*.5, pitchConf*.25,tr);
                        cubeVel = power*.5;
                        cubeVol = trans(.5 + centroid*.2, (.5 + .5 * pitchFreq) * 1.1, tr);
                        cubeRadius = trans(100 + power * 400, 100 + pitchFreq * 200, tr);
                        cubeSize = trans(cubeSize=4+centroid*2, 2.0 + pitchFreq*5, tr);
                    }
                    else{
                        cubeX = centroid*.05;
                        cubeY=pitchConf*.25;
                        cubeVel = power*.5;
                        cubeVol = (.5 + .5 * pitchFreq) * 1.1;
                        cubeRadius = 100 + pitchFreq * 200;
                        cubeSize = 2.0 + pitchFreq*5;
                    }
                }
                else if (_subMarker == 3){
                    if (animValue.isOrWillBeAnimating()){
                        float tr = animValue.val();
                        cubeX = trans(centroid*.05, 0.4, tr);
                        cubeY = trans(pitchConf*.25, 0.075 + pitchFreq * 0.5, tr);
                        cubeVel = trans(power*.5, 0.0, tr);
                        cubeVol = trans((.5 + .5 * pitchFreq) * 1.1, .5 + .5*pitchFreq, tr);
                        cubeRadius = trans(100 + pitchFreq * 200, pitchFreq * 600, tr);
                        cubeSize = trans(2.0 + pitchFreq*5, 1.5 + pitchFreq*5, tr);
                    }
                    else{
                        cubeX = 0.4;
                        cubeY = 0.075 + pitchFreq * 0.5 ;
                        cubeVel = 0.0;
                        cubeVol = .5 + .5*pitchFreq;
                        cubeRadius = pitchFreq * 600;
                        cubeSize =  1.5 + pitchFreq*5;
                    }
                }
            }
            else if (_marker==Z){
                if (animValue.isOrWillBeAnimating()){
                    float tr = animValue.val();
                    cubeX = trans(0.4, 0.0, tr);
                    cubeY = trans(0.075 + pitchFreq * 0.5, 0.0, tr);
                    cubeVel = trans(0.0, 0.0, tr);
                    cubeVol = trans(.5 + .5*pitchFreq, 0.0, tr);
                    cubeRadius = trans(pitchFreq * 600, 0.0, tr);
                    cubeSize = trans(1.5 + pitchFreq*5, 0.0, tr);
                }
                else{
                    cubeX = 0.0;
                    cubeY = 0.0;
                    cubeVel = 0.0;
                    cubeVol = 0.0;
                    cubeRadius = 0.0;
                    cubeSize =  0.0;
                }
            }
            
        }else{
            
            cubeX = xShGui;
            cubeY = yShGui;
            cubeVol = volumeShGui;
            cubeSize = sizeShGui;
            cubeRadius = radiusShGui;
            cubeVel = velShGui;
            
        }
        
        sphereCubes.setRadius(cubeRadius);
        
    }
    TIME_SAMPLE_STOP("Cube Mesh");
    
    
#pragma mark  Marching Cubes
    
    TIME_SAMPLE_START("Marching Cubes");
    static int marchCnt;
    if(bDoMarching){
        float noiseStep, noiseScale, noiseScale2, marchStep;
        if(bReceiveOSCGui){
            if (_marker==C && _subMarker==2){
                
                noiseStep = marchCnt * 0.005;
                noiseScale = .04 ;
                noiseScale2 = noiseScale * (2.25 + pitchFreq*1.5);
                marchStep =  2 * power;
                
                marchShX = 0.02;
                marchShY = 0.01 + pitchFreq*0.1;
                marchShVel = 0.7 + power*pitchFreq*20;
                marchShVol = .5;
                
                if(animValue.isOrWillBeAnimating()){
                    float tr = animValue.val();
                    mc.threshold = trans(1.0, 1-power, tr);
                    marchShSize = trans(0, (.75+pitchFreq*4) * 0.005, tr);
                }
                else{
                    mc.threshold =  1-power;
                    marchShSize = (.75+pitchFreq*4) * 0.0065;
                }
                
                bDoCubeMesh ? marchRadius=cubeRadius*animValue.val() : marchRadius=marchRadiusGui;
                
                
                
            }
            else if (_marker == D){
                
                if(_subMarker == 1){
                    
                    noiseStep = marchCnt * 0.01;
                    noiseScale = 0.07;
                    noiseScale2 = noiseScale * 3.5;
                    marchStep = 3.25;
                    mc.threshold = 0.7;
                    
                    marchShX = 0.02;
                    marchShY = 0.01;
                    marchShVel = 0.7;
                    marchShVol = 0.0;
                    marchShSize = (2+pitchFreq) * 0.005;
                    
                    bDoCubeMesh ? marchRadius=cubeRadius*1.5 : marchRadius=marchRadiusGui;
                    
                }
                else if (_subMarker==2){
                    
                    noiseStep = marchCnt * 0.01;
                    noiseScale = 0.07;
                    noiseScale2 = noiseScale * 3.5;
                    
                    marchShX = 0.02;
                    marchShVol = .5;
                    marchShSize = (2+pitchFreq) * 0.005;
                    
                    if(animValue.isOrWillBeAnimating()){
                        float tr = animValue.val();
                        marchStep = trans(3.25, 7 * power, tr);
                        mc.threshold = trans(0.7,  .3 + 0.7 * (1-pitchFreq), tr);///check
                        marchShY = trans(0.01, 0.01 + pitchFreq*0.5, tr);
                        marchShVel = trans(0.7, 0.7 + pitchFreq, tr);
                        
                    }
                    else{
                        marchStep =  7 * power;
                        mc.threshold = .3 + 0.7 * (1-pitchFreq);
                        marchShY = 0.01 + pitchFreq*0.5;
                        marchShVel = 0.7 + pitchFreq;
                    }
                    if(mc.threshold<0.3)mc.threshold=0.3;
                    bDoCubeMesh ? marchRadius=cubeRadius*1.5 : marchRadius=marchRadiusGui;
                    
                }
                else if (_subMarker==3){
                    if(animValue.isOrWillBeAnimating()){
                        float tr = animValue.val();
                        noiseStep = trans(marchCnt * 0.01, marchCnt * 0.005, tr);
                        noiseScale = trans(0.07, 0.04, tr);
                    }
                    else{
                        noiseStep = marchCnt * 0.005;
                        noiseScale = .04 ;
                    }
                    
                    
                    noiseScale2 = noiseScale * 3.5;
                    marchStep =  7 * power;
                    mc.threshold = .2 + 0.8 * (1-pitchFreq);
                    
                    marchShX = 0.02;
                    marchShY = 0.01 + pitchFreq*0.5;
                    marchShVel = 0.7 + pitchFreq;
                    marchShVol = .5;
                    marchShSize = (2+pitchFreq) * 0.005;
                    
                    
                    bDoCubeMesh ? marchRadius=cubeRadius : marchRadius=marchRadiusGui;
                }
            }
            else if (_marker ==Z){
                
                noiseStep = marchCnt * 0.005;
                noiseScale = .04 ;
                
                noiseScale2 = noiseScale * 3.5;
                marchStep =  7 * power;
                mc.threshold = .2 + 0.8 * (1-pitchFreq);
                
                marchShX = 0.02;
                marchShY = 0.01 + pitchFreq*0.5;
                marchShVel = 0.7 + pitchFreq;
                marchShVol = .5;
                marchShSize = (2+pitchFreq) * 0.005;
                
                
                bDoCubeMesh ? marchRadius=cubeRadius : marchRadius=marchRadiusGui;
                
            }
            ///limit threshold...
            if(mc.threshold<0.65)mc.threshold=0.65;
            
        }
        else{
            //NOISE
            float noiseStep = marchCnt * 0.01;
            float noiseScale = marchNoise1Gui;
            float noiseScale2 = marchNoise1Gui*marchNoise2Gui;
            
            marchShX = xShGui;
            marchShY = yShGui;
            marchShVel = velShGui;
            marchShVol = volumeShGui;
            marchRadius = marchRadiusGui;
            marchShSize = sizeShGui * 0.005;
            marchStep = marchStepGui;
            
            
        }
        //----------------------
        for(int i=0; i<mc.resX; i++){
            for(int j=0; j<mc.resY; j++){
                for(int k=0; k<mc.resZ; k++){
                    //noise
                    float nVal = ofNoise(float(i)*noiseScale + noiseStep, float(j)*noiseScale + noiseStep, float(k)*noiseScale + noiseStep);
                    if(nVal > 0.)	nVal *= ofNoise(float(i)*noiseScale2 + noiseStep, float(j)*noiseScale2 - noiseStep, float(k)*noiseScale2 + noiseStep);
                    mc.setIsoValue( i, j, k, nVal );
                }
            }
        }
        
        mc.update();
        marchCnt += 1+marchStep;
        
        mc.scale.set( marchRadius, marchRadius, marchRadius );
        
    }
    TIME_SAMPLE_STOP("Marching Cubes");
    
    
#pragma mark  Faces
    TIME_SAMPLE_START("Faces Ext");
    if(bDoFaces){
        sphereDistor.setMode( OF_PRIMITIVE_TRIANGLES );
        /*
         if (bDoDisplacement){
         triangles = displacement.mainMesh.getUniqueFaces();
         }
         */
        
        float strength;
        if (bReceiveOSCGui){
            strength = -2 * strengthGui * pitchConf ;
        }else{
            strength = -2 * strengthGui;
        }
        
        if(bDoDisplacement){
            triangles = displacement.mainMesh.getUniqueFaces();
            ofVec3f faceNormal;
            float t = ofGetElapsedTimef();
            
            for(int i = 0; i < triangles.size(); i++ ) {
                faceNormal = triangles[i].getFaceNormal();
                for(int j = 0; j < 3; j++ ) {
                    
                    float noise = ofSignedNoise(t *.01, triangles[i].getVertex(j).y)*20.0*.01;
                    triangles[i].setVertex( j, triangles[i].getVertex(j) + faceNormal * strength + noise);
                    
                }
            }
            sphereDistor.getMesh().setFromTriangles( triangles );
        }
        
    }
    TIME_SAMPLE_STOP("Faces Ext");
    
    ///CAMERA-----------------------
    TIME_SAMPLE_START("Camera Pos");
    {
        static float longit, latit, dist;
        float tr = animValueCam.val();
        
        if(_marker==A){
            longit = 30;
            latit = -30;
            dist = 400;
            camCounter = 0.0;
        }
        else if (_marker==B){
            if(_subMarker==0){
                if(animValueCam.isOrWillBeAnimating()){
                    longit = trans(30, -10, tr);
                    latit = trans(-30 ,-10, tr);
                    dist = trans(400, 1000, tr);
                }
                else{
                    longit = -10;
                    latit = -10;
                    dist = 1000;
                }
                camCounter = 0.0;
            }
            else if(_subMarker==1){
                if(animValueCam.isOrWillBeAnimating()){
                    //longit = trans(-10, camCounter+power*90, tr);
                    longit = -10;
                    latit = -10;
                    dist = trans(1000, 2000 - pitchConf*1500, tr);
                    //camCounter += trans(0.0, 1.5, tr);
                    
                }
                else{
                    longit = -10;
                    latit = -10;
                    dist = 2000 - pitchConf*1500;
                }
                camCounter = 0.0;
            }
            else if (_subMarker==2){
                if(animValueCam.isOrWillBeAnimating()){
                    longit = -10;
                    latit = -10;
                    dist = trans(2000 - pitchConf*1500, 1000, tr);
                }
                else{
                    longit = -10;
                    latit = -10;
                    dist = 1000;
                }
                camCounter = 0.0;
            }
        }
        else if(_marker==C){
            if(_subMarker==0){
                if(animValueCam.isOrWillBeAnimating()){
                    longit = trans(-10, (power1+power2)*90, tr);
                    latit  = trans(-10, -30, tr);
                    dist   = trans(1000, 500-pitchConf*200, tr);
                }
                else{
                    longit = (power1+power2)*90 ;
                    latit = -30;
                    dist = 500-pitchConf*200;
                }
                camCounter = 0.0;
            }
            else if(_subMarker==1){
                if(animValueCam.isOrWillBeAnimating()){
                    longit = trans((power1+power2)*90, (power1+power2)*180, tr);
                    latit = -30;
                    dist = trans(500-pitchConf*200, 500-pitchConf*150, tr);
                }
                else{
                    longit = (power1+power2)*180 ;
                    latit = -30;
                    dist = 500-pitchConf*150;
                }
                camCounter = 0.0;
            }
            else if(_subMarker==2){
                if(animValueCam.isOrWillBeAnimating()){
                    longit = trans((power1+power2)*180,  centroid*180, tr);
                    latit = -30;
                    dist = trans(500-pitchConf*150, 350+centroid*70, tr);
                }
                else{
                    longit = centroid*180;
                    latit = -30;
                    dist = 350+centroid*70;
                }
                camCounter=0.0;
            }
        }
        else if(_marker==D){
            if(_subMarker==0){
                if(animValueCam.isOrWillBeAnimating()){
                    longit = trans(camCounter+centroid*180, camCounter+power*90, tr);
                    latit = trans(-30, sin(camCounter*.01)*30, tr);
                    dist = trans(350+centroid*70, 400-centroid*200, tr);
                    camCounter += trans(0.0, 1.0, tr);
                }
                else{
                    longit = camCounter + power*90;
                    latit = sin(camCounter*.01)*30;
                    dist = 400-centroid*200;
                    camCounter += 1.0;
                }
            }
            else if(_subMarker==1){
                if(animValueCam.isOrWillBeAnimating()){
                    longit = camCounter + power*90;
                    latit = trans(sin(camCounter*.01)*30, -30, tr);
                    dist = trans(400-centroid*200, 500, tr);
                    camCounter += trans(1.0, 0.75, tr);
                }
                else{
                    longit = camCounter + power*90;
                    latit = -30;
                    dist = 500;
                    camCounter += 0.75;
                }
            }
            else if (_subMarker==2){
                if(animValueCam.isOrWillBeAnimating()){
                    longit = trans(camCounter+power*90, camCounter, tr);
                    latit  = -30;
                    dist = 500;
                    camCounter += trans(0.75, 3.0, tr);
                }
                else{
                    longit = camCounter;
                    latit = -30;
                    dist = 500;
                    camCounter += 3.0;
                }
                
            }
            else if(_subMarker==3){
                if(animValueCam.isOrWillBeAnimating()){
                    longit = trans(camCounter, camCounter+pitchConf*90, tr);
                    latit = trans(-30, sin(camCounter*.01)*60, tr);
                    dist = trans(500, 1000-pitchFreq*950, tr);
                    camCounter += 3.0;
                }
                else{
                    longit = camCounter+pitchConf*90;
                    latit = sin(camCounter*.01)*60;
                    dist = 1000-pitchFreq*950;
                    camCounter += 3.0;
                }
            }
        }
        else if(_marker==Z){
            longit = camCounter + pitchConf*90;
            latit = sin(camCounter*.01)*60;
            dist = 1000-pitchFreq*950;
            camCounter += 3.0;
            
        }
        
        fxdCam.orbit(longit, latit, dist);
        fxdCam.lookAt(ofVec3f(0,0,0));
        light.orbit(longit+30, latit, dist);
    }
    TIME_SAMPLE_STOP("Camera Pos");

    
  
    
    
}

//--------------------------------------------------------------
void EntropicaApp::exit(){
    cout<<"Unloading shaders..."<<endl;
    faceShader.unload();
    cout<<"faceShader unloaded"<<endl;
    noiseShader.unload();
    cout<<"noiseShader unloaded"<<endl;
    cubeShader.unload();
    cout<<"cubeShader unloaded"<<endl;
    
    
    
    bDoCubeMesh = bDoDisplacement = bDoMetaball = bDoNoiseShader
    = bDoMarching = bDoCubeMesh = bDoFaces = false;
    
    ofLogNotice()<<"ENTROPICA EXIT";
}
//--------------------------------------------------------------

#pragma mark - Draw funcs

//--------------------------------------------------------------
void EntropicaApp::drawScene(){
    
    ofRectangle viewport = ofRectangle(0, 0, fbo->getWidth(), fbo->getHeight());
    
    if(bPostProcessing){
        bUseFixCam ? post.begin(fxdCam, viewport) : post.begin(eCam, viewport) ;
        ofBackground(backgroundColor);
    }
    else{
        ofBackground(backgroundColor);
        bUseFixCam ? fxdCam.begin(viewport) : eCam.begin(viewport);
    }
    
    light.enable();
    material.begin();
    
    if(bDrawAxisGui)ofDrawAxis(500);
    
    ofEnableDepthTest();
    
    ///Displacement-------------------------------------
    TIME_SAMPLE_START("DisplacementSphere");
    ofPushStyle();
    if (_marker==D || _marker==Z) {
        if (bDoFaceSh) {
            faceShader.begin();
            faceShader.setUniform1f("timeVal", ofGetFrameNum());
            faceShader.setUniform1f("noiseAmnt", dispNzAmnt);
        }
        
    }
    
    if(bDoDisplacement){
        static int cubeTimeCnt;
        if(!bDoFaceSh)phongSahder.begin();
        ofPushStyle();
        ofSetColor(mainColor);
        bDoFaces ? sphereDistor.draw() : displacement.mainMesh.draw();
        ofPopStyle();
        if(!bDoFaceSh)phongSahder.end();
        
    }
    
    if (_marker==D || _marker==Z){
        if (bDoFaceSh)faceShader.end();
    }
    ofPopStyle();
    TIME_SAMPLE_STOP("DisplacementSphere");
    
    ///Marching Cubes--------------------------------------
    TIME_SAMPLE_START("MarchingCubes");
    ofPushStyle();
    if (bDoMarching){
        
        cubeShader.begin();
        cubeShader.setUniform1f("timeVal", ofGetFrameNum() );
        cubeShader.setUniform1f("mouseX", marchShX);
        cubeShader.setUniform1f("mouseY", marchShY);
        cubeShader.setUniform1f("volume", marchShVol);
        cubeShader.setUniform1f("velocity", marchShVel);
        cubeShader.setUniform1f("size", marchShSize*1.5);///*
        
        
        ofSetColor(mainColor);
        //bWireGui?	mc.drawWireframe() : mc.draw();
        mc.draw();
        cubeShader.end();
    }
    ofPopStyle();
    TIME_SAMPLE_STOP("MarchingCubes");
    
    ///Metaball---------------------------------------------
    TIME_SAMPLE_START("Meta");
    ofPushStyle();
    if(bDoMetaball){
        //channel-1************************************
        static int metaTimeCnt_1;
        if (bDoNoiseShader) {
            noiseShader.begin();
            //noiseShader.setUniform1f("timeVal", ofGetFrameNum());
            noiseShader.setUniform1f("timeVal", metaTimeCnt_1);
            noiseShader.setUniform1f("mouseX", metaShX_1);
            noiseShader.setUniform1f("mouseY", metaShY_1*1000);
            noiseShader.setUniform1f("volume", metaShVol_1);
            noiseShader.setUniform1f("velocity", metaShVel_1);
        }
        
        
        ofSetColor(mainColor);
        ofPushMatrix();
        ofTranslate(metaTr_1, metaTr_1, 0);
        for(int i=0; i<centers_1.size();i++){
            ofDrawSphere(centers_1[i]*metaScale_1, metaMaxRadius_1);
        }
        ofPopMatrix();
        
        if (bDoNoiseShader) noiseShader.end();
        metaTimeCnt_1 += (1+metaShVel_1);
        
        //channel-2*************************************
        static int metaTimeCnt_2;
        if (bDoNoiseShader) {
            noiseShader.begin();
            //noiseShader.setUniform1f("timeVal", ofGetFrameNum());
            noiseShader.setUniform1f("timeVal", metaTimeCnt_2);
            noiseShader.setUniform1f("mouseX", metaShX_2);
            noiseShader.setUniform1f("mouseY", metaShY_2*1000);
            noiseShader.setUniform1f("volume", metaShVol_2);
            noiseShader.setUniform1f("velocity", metaShVel_2);
        }
        
        ofSetColor(mainColor);
        ofPushMatrix();
        ofTranslate(-metaTr_2, -metaTr_2, 0);
        for(int i=0; i<centers_2.size();i++){
            ofDrawSphere(centers_2[i]*metaScale_2, metaMaxRadius_2);
        }
        ofPopMatrix();
        
        if (bDoNoiseShader) noiseShader.end();
        
        metaTimeCnt_2 += (1+metaShVel_2);
        
        
    }
    ofPopStyle();
    TIME_SAMPLE_STOP("Meta");
    
    
    ///CubeMesh------------------------------------------------
    TIME_SAMPLE_START("Cubes");
    ofPushStyle();
    if(bDoCubeMesh){
        
        static int cubeTimeCnt;
        
        cubeShader.begin();
        cubeShader.setUniform1f("timeVal", cubeTimeCnt);
        cubeShader.setUniform1f("mouseX", cubeX);
        cubeShader.setUniform1f("mouseY", cubeY);
        cubeShader.setUniform1f("volume", cubeVol);
        cubeShader.setUniform1f("size", cubeSize);
        
        
        ofSetColor(mainColor);
        sphereCubes.draw();
        
        cubeShader.end();
        
        cubeTimeCnt += (1+cubeVel);
        
    }
    ofPopStyle();
    TIME_SAMPLE_STOP("Cubes");
    
    
    material.end();
    if (bPostProcessing) {
        post.end();
    }
    else{
        bUseFixCam ? fxdCam.end() : eCam.end();
    }
    
    
    
    ofDisableDepthTest();
    ofDisableLighting();
    glPopAttrib();
}
//--------------------------------------------------------------
void EntropicaApp::drawGui(){
    gui.draw();
}
//--------------------------------------------------------------
#pragma mark - Flow Control

//--------------------------------------------------------------
void EntropicaApp::setScene(int gralMarker){
    switch (gralMarker) {
        case 0:
            setScene(A,0);
            break;
        case 1:
            setScene(A,1);
            break;
        case 2:
            setScene(A,2);
            break;
        case 3:
            setScene(A,3);
            break;
        case 4:
            setScene(B,0);
            break;
        case 5:
            setScene(B,1);
            break;
        case 6:
            setScene(B,2);
            break;
        case 7:
            setScene(C,0);
            break;
        case 8:
            setScene(C,1);
            break;
        case 9:
            setScene(C,2);
            break;
        case 10:
            setScene(D,0);
            break;
        case 11:
            setScene(D,1);
            break;
        case 12:
            setScene(D,2);
            break;
        case 13:
            setScene(D,3);
            break;
        case 14:
            setScene(Z,0);
            break;
        default:
            break;
    }
    
}
//--------------------------------------------------------------
void EntropicaApp::setScene (Mark iMarker, int iSubmarker){
    
    Mark marker = iMarker;
    int subMarker = iSubmarker;
    _animDuration = 1.0;
    _animCamDuration = 1.0;
    
    //Limit subMarker------------------------------------
    if(marker==A || marker==D){
        iSubmarker<=3 ? subMarker=iSubmarker : subMarker=3;
    }
    else if(marker==B || marker==C){
        iSubmarker<=2 ? subMarker=iSubmarker : subMarker=2;
    }
    else{
        _subMarker= iSubmarker;
    }
    //Change marker and parameters------------------------
    if(marker!=_marker || subMarker!=_subMarker){
        
        if(marker==A){
            _animDuration = 2.0;
        }
        else if(marker==B){
            if(subMarker==0){
                _animDuration = 0.1;
                _animCamDuration = 0.25;
            }
            else if(subMarker==1){
                _animDuration = 0.3;
                _animCamDuration = 0.5;
            }
            else if(subMarker==2){
                // _animDuration = 0.2;
                _animDuration = 0.1;
                _animCamDuration = 0.5;
            }
        }
        else if(marker==C){
            if(subMarker==0){
                _animDuration = 0.25;////->paraVivo
                _animCamDuration = 0.25;////->paraVivo
                ///_animDuration = 1.0;////->paraGrabacionEstudio
                ///_animCamDuration = 1.0;////->paraGrabacionEstudio
            }
            else if(subMarker==2){
                _animDuration =.5;
            }
            
        }
        else if(marker==D){
            if(subMarker==1){
                mc.setResolution(18, 18 , 18);
            }
            else if (subMarker==2){
                _animDuration = 0.5;
                _animCamDuration = 0.5;
            }
            else if(subMarker==3){
                _animDuration = 0.5;
                _animCamDuration = 0.5;
            }
            
        }
        else if (marker==Z){
            
        }
        
        _marker = marker;
        _subMarker = subMarker;
        
        animValue.reset(0.0);
        animValue.animateTo(1.0);
        animValueCam.reset(0.0);
        animValueCam.animateTo(1.0);
    }
}
//--------------------------------------------------------------
#pragma mark - Key funcs
//--------------------------------------------------------------
void EntropicaApp::keyPressed(int key){
    
    Mark marker;
    int subMarker;
    
    switch (key) {
        
        case OF_KEY_RIGHT:
            if(_gralMarker<=13)_gralMarker++;
            setScene(_gralMarker);
            break;
        case OF_KEY_LEFT:
            if(_gralMarker>0)_gralMarker--;
            setScene(_gralMarker);
            break;
            

        default:
            break;
    }

    //if(key==75) bKeyCommands = !bKeyCommands; //key 75 -> shift+k
    
    
       
}
//--------------------------------------------------------------
#pragma mark - Syphon funcs
//--------------------------------------------------------------

//--------------------------------------------------------------
void EntropicaApp::setupPostProcessing(int w, int h){
    // Setup post-processing chain
    
    post.init(w, h);
    
    post.createPass<BloomPass>()->setEnabled(false);
    dof = post.createPass<DofPass>();
    dof->setAperture(0.2);
    dof->setMaxBlur(0.05);
    dof->setEnabled(false);
    post.createPass<FxaaPass>()->setEnabled(true);///-
    godRays = post.createPass<GodRaysPass>();
    godRays->setLightDirDOTviewDir(0.3);
    godRays->setEnabled(false);///-
    limbDarkening=post.createPass<LimbDarkeningPass>();
    limbDarkening->setEnabled(false);
    ssao=post.createPass<SSAOPass>();
    ssao->setAoClamp(0.65);
    ssao->setLumInfluence(0.9);
    ssao->setEnabled(false);
    
    
}

//--------------------------------------------------------------
void EntropicaApp::setControlGui(){
    
    gui.setup(); // most of the time you don't need a name
    gui.setPosition(300,0);
    gui.add(bReceiveOSCGui.setup("RECEIVE OSC", true));
    gui.add(bDoDisplacementGui.setup("Do Displacement", false));
    gui.add(bDoMetaballGui.setup("Do Metaball", true));
    gui.add(bDoCubeMeshGui.setup("Do CubeMesh", false));
    gui.add(bDoMarchingGui.setup("Do Marching", false));
    gui.add(bDoFacesGui.setup("Do Faces", false));
	//gui.add(bWireGui.setup("Wireframe", false));
    gui.add(bDrawAxisGui.setup("Draw Axis", false));
    gui.add(bLitGui.setup("LitSphere", false));
    //-
    gui.add(lDisp.setup("Displacement ----", ""));
	gui.add(velGui.setup( "Velocity", 1, 0, 5 ));
    gui.add(volumeGui.setup( "Volume", 0.5, 0, 1 ));
    gui.add(xGui.setup( "X", 0.02, 0, 0.1 ));
    gui.add(yGui.setup( "Y", 0.01, 0, 0.1 ));
    gui.add(radiusGui.setup( "Radius", 30, 0, 500 ));
    gui.add(resolGui.setup( "Resolution", 48, 1, 100 ));
    gui.add(strengthGui.setup( "Strength", -25, -200, 200 ));
    gui.add(faceNoiseGui.setup( "FaceNoise", 20., 0., 20.));
    //gui.add(angleGui.setup( "Angle", 1, 0, 10 ));
    //-
    gui.add(lMeta.setup("Metaball ----", ""));
    gui.add(cubeScaleGui.setup( "ScaleCube", 250, 0, 500));
    gui.add(metaVelGui.setup( "VelOrbit", 1, 0, 10));
    gui.add(metaDistOrbGui.setup( "DistOrbit", 1., 0., 1.));
    gui.add(minRadiusGui.setup( "minRadius", .1, 0, 1));
    gui.add(maxRadiusGui.setup( "maxRadius", .2, 0, 2));
    //-
    gui.add(lMarch.setup("Marching ----", ""));
    gui.add(marchNoise1Gui.setup( "MarhNoise-1", .06, 0, .2));
    gui.add(marchNoise2Gui.setup( "MarhNoise-2", 2., 0, 5.));
    gui.add(marchStepGui.setup( "MarchStep", .5, 0., 5.));
    gui.add(marchRadiusGui.setup( "MarchRadius", 250, 0, 500));
    //-
    gui.add(lCubeShader.setup("CubeMesh ----", ""));
    gui.add(bDoShaderGui.setup("Do Shader", false));
    gui.add(xShGui.setup( "X", 0.02, 0, 2.0 ));
    gui.add(yShGui.setup( "Y", 0.01, 0, 0.5 ));
    gui.add(velShGui.setup( "Velocity", .1, 0, 20.0 ));
    gui.add(volumeShGui.setup( "Volume", 0.5, 0, 1.0 ));
    gui.add(sizeShGui.setup( "Size", 3, 0, 10.0 ));
    gui.add(radiusShGui.setup( "Radius", 100, 0, 500 ));


}

#pragma mark - String funcs

//--------------------------------------------------------------
string EntropicaApp::getInfoString(){
    string infoString = "FBO Resolution: "+ofToString(fbo->getWidth())+"x"+ofToString(fbo->getHeight()) +
    "\nDoing PostProcessing: " + boolToString(bPostProcessing) +
    "\nAnimValue: " + ofToString(animValue.val())
    + "\nAnimCamValue: " + ofToString(animValueCam.val())
    + "\n----------------"
    + "\nDisplacement: " + boolToString(bDoDisplacement)
    + "\nMetaballs: " + boolToString(bDoMetaball)
    + "\nMarching Cubes: " + boolToString(bDoMarching)
    + "\nCubes Mesh: " + boolToString(bDoCubeMesh)
    + "\nFaces Ext: " + boolToString(bDoFaces)
    + "\n----------------"
    + "\nthreshold:" + ofToString(mc.threshold)
    + "\nDispShere VertNum: " + ofToString(displacement.mainMesh.getNumVertices())
    + "\nNumVert SphDist: " + ofToString(sphereDistor.getMesh().getNumVertices())
    + "\nNumFaces: " + ofToString(triangles.size())
    //+ "\nNumVertMeta: " + ofToString(iso_1.getMesh().getNumVertices())
    + "\nVertexCubeMesh: " + ofToString(sphereCubes.getMesh().getNumVertices())
    +"\ncamCounter--" + ofToString(camCounter);
    
    return infoString;
}
//--------------------------------------------------------------
string EntropicaApp::getHelpString(){
    string helpString ="RIGHT: Next Marker \nLEFT: Previous Marker\n'i': Fade IN / 'o': Fade OUT \n'f': Viewer fullscreen";
    return helpString;
}
//--------------------------------------------------------------
string EntropicaApp::getMarkerString(){
    string markerString =  "MARKER: "+ markerToString(_marker)+ "\nsubMarker: "+ ofToString(_subMarker);
    return markerString;
}


//--------------------------------------------------------------
string EntropicaApp::getInfoForAnalyzerString(){
    
    string info = "App: ENTROPICA"
    "\n-------------"
    "\nch1-power: " + ofToString(osc->ch1.power)
    + "\nch2-power: " + ofToString(osc->ch2.power);
    
    return info;
    
}









