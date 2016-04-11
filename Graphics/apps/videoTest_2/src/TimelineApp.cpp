

#include "TimelineApp.h"

void TimelineApp::setup(){
//	parameters.setName("parameters");
//	parameters.add(radius.set("radius",50,1,100));
//	parameters.add(color.set("color",100,ofColor(0,0),255));
//	gui.setup(parameters);

    ofBackground(0);
	ofSetVerticalSync(false);
    
    ofxTimeline::removeCocoaMenusFromGlut("AllTracksExample");
    timeline.setup();
    
#ifdef TIMELINE_VIDEO_INCLUDED
    timeline.addVideoTrack("fingers", "fingers.mov");
#endif
#ifdef TIMELINE_AUDIO_INCLUDED
    timeline.addAudioTrack("audio", "4chan.wav");
    timeline.setDurationInSeconds(timeline.getAudioTrack("audio")->getDuration());
#endif
    
    timeline.addCurves("curves", ofRange(0, 255));
    timeline.addBangs("bangs");
    timeline.addFlags("flags");
    timeline.addColors("colors");
    timeline.addLFO("lfo");
    timeline.addSwitches("switches");
    
    timeline.setPageName("Page 1");
    timeline.addPage("Page 2");
    timeline.addPage("Page 3");
    timeline.addPage("Page 4");
    timeline.setCurrentPage(0);
    
    timeline.enableSnapToOtherKeyframes(false);
    timeline.setLoopType(OF_LOOP_NORMAL);
}

void TimelineApp::update(){

}

void TimelineApp::draw(){
	//gui.draw();
    timeline.draw();
}
