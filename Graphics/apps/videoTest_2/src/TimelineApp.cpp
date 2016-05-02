

#include "TimelineApp.h"

void TimelineApp::setup(){

    ofBackground(40);
	ofSetVerticalSync(false);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    timeline.setWorkingFolder("timeline");
    
    ofxTimeline::removeCocoaMenusFromGlut("AllTracksExample");
    timeline.setup();
    timeline.setFrameRate(FRAME_RATE);
    timeline.setDurationInSeconds(DURATION);
    timeline.setFrameBased(TRUE);
    
    
#ifdef TIMELINE_VIDEO_INCLUDED
    timeline.addVideoTrack("fingers", "fingers.mov");
#endif
#ifdef TIMELINE_AUDIO_INCLUDED
    timeline.addAudioTrack("audio", "4chan.wav");
    timeline.setDurationInSeconds(timeline.getAudioTrack("audio")->getDuration());
#endif
    
    timeline.addSwitches("mode");
    timeline.addSwitches("radMode");
    timeline.addCurves("width", ofRange(0, 1));
    timeline.addCurves("height", ofRange(0, 1));
    timeline.addCurves("Xpos", ofRange(0, 1));
    timeline.addCurves("Ypos", ofRange(0, 1));
    timeline.addCurves("Hres", ofRange(0, 1));
    timeline.addCurves("Vres", ofRange(0, 1));
    
    timeline.setPageName("General");
    
    //timeline.addSwitches("switches");
    
    timeline.addPage("Parameters");
    timeline.setCurrentPage("Parameters");
    timeline.addCurves("velocity", ofRange(0, 1));
    timeline.addCurves("maskRadius", ofRange(0, 1));
    timeline.addCurves("radDeform", ofRange(0, 1));
    timeline.addCurves("cubesize", ofRange(0, 1));
    timeline.addCurves("fisheye", ofRange(0, 1));
    
    timeline.addPage("Noise");
    timeline.setCurrentPage("Noise");
    timeline.addCurves("nzTime", ofRange(0, 1));
    timeline.addCurves("nzXAmp", ofRange(0, 1));
    timeline.addCurves("nzXFreq", ofRange(0, 1));
    timeline.addCurves("nzXRug", ofRange(0, 1));
    timeline.addCurves("nzYAmp", ofRange(0, 1));
    timeline.addCurves("nzYFreq", ofRange(0, 1));
    timeline.addCurves("nzYRug", ofRange(0, 1));
    timeline.addCurves("nzZAmp", ofRange(0, 1));
    timeline.addCurves("nzZFreq", ofRange(0, 1));
    timeline.addCurves("nzZRug", ofRange(0, 1));
    
    timeline.setCurrentPage("General");

    timeline.enableSnapToOtherKeyframes(false);
    //timeline.setLoopType(OF_LOOP_NORMAL);
    timeline.setLoopType(OF_LOOP_NONE);
    
    
}
//---------------------

void TimelineApp::draw(){
    timeline.draw();
}
