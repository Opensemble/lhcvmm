#include "ofMain.h"
#include "ofApp.h"
#include "TimelineApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){

//	ofGLWindowSettings settings;
//	settings.setGLVersion(3,2);
//	ofCreateWindow(settings);
    
    //classic
//    ofSetupOpenGL(1024, 576, OF_WINDOW);
//	ofRunApp(new ofApp());

    
    
    
    //multi
    ofGLFWWindowSettings settings;
    
    settings.width = 1024;
    settings.height = 576;
    settings.setPosition(ofVec2f(300,0));
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    
    settings.width = 300;
    settings.height = 300;
    settings.setPosition(ofVec2f(0,0));
    settings.resizable = false;
    shared_ptr<ofAppBaseWindow> timelineWindow = ofCreateWindow(settings);
    
    shared_ptr<ofApp> mainApp(new ofApp);
    shared_ptr<TimelineApp> timelineApp(new TimelineApp);
    mainApp->timelineApp = timelineApp;
    
    ofRunApp(timelineWindow, timelineApp);
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();

}
