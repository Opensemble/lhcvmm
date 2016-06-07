/*
 * VideoRenderer.h
 *  Renderer de contexto openGL a "Secuencia PNG" o Archivo de video
 *  Creado utilizando el addon ofxVideoRecorder(https://github.com/timscaffidi/ofxVideoRecorder)
 *  Y el ejemplo de openFrameworks threadedPixelBufferExample (https://github.com/openframeworks/openFrameworks/tree/master/examples/gl/threadedPixelBufferExample)
 */

#pragma once

#include "ofMain.h"
#include "ofxVideoRecorder.h"
#include "ImageSaverThread.h"

enum RecordingMode{
    PNG_SEQUENCE,
    MOV_FILE
};

enum Resolution{
    r256,
    r512,
    r1024,
    r2048,
    r4096
};

class VideoRenderer{

    public:
    
        void setup(int iFramerate, RecordingMode initMode, Resolution initRes);
        void update();
        void draw(int x, int y, int w, int h);
        void exit();
    
        void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);
    
    
        void startRecording();
        void stopRecording();
    
        void setOutputResolution(Resolution res);
        void setRecordingMode(RecordingMode mode){currentRecMode = mode;}
    
    
        ofFbo* getFbo(){return &fbo;}
        int getFboWidth(){return fbo.getWidth();}
        int getFboHeight(){return fbo.getHeight();}
        bool getIsRecording(){return bIsRecording;}
        RecordingMode getRecordingMode(){return currentRecMode;}
        Resolution getOutputResolution(){return currentOutResolution;}
        string getRecordingModeAsString();
        string getResolutionAsString();
    
        int getLastFrameMarker(){return saverThread.getlastFrameMarker();}
    
    private:
    
        bool            bIsRecording;
        RecordingMode   currentRecMode;
        Resolution      currentOutResolution;
        int framerate;
    
        ofFbo fbo;
        ofPixels pixels;
        
        //pngSequence-Saver objects and variables
        ofBufferObject pixelBufferBack, pixelBufferFront;
        bool pngFirstFrame;
        ImageSaverThread saverThread;
        
        //MovFile Recorder objects and variables
        ofxVideoRecorder    vidRecorder;
        ofFbo recordFbo;
        ofPixels recordPixels;
        string fileName;
        string fileExt;
        


};