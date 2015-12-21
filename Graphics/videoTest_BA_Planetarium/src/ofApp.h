#pragma once

#include "ofMain.h"
#include "ofxFisheye.h"
#include "VideoRenderer.h"

#define FRAME_RATE 30
#define DURATION 15.0

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void drawScene(int w, int h);
    
        void startAnimation();
        void stopAnimation();
		
        //animation data variables----------------
        bool    isAnimating;
        int     frameCounter;//animation Frame Counter
        float   frameDuration;//Duration in seconds of each frame
        int     framesMaxNumber;//Number of frames of the entire animation
        float   animValue;//Current frame in relationship with the duration of the entire animation (0.0 - 1.0)
        float   animationTime;
    
        //----------------------------
        ofxFisheye fisheye;
        float fisheyeAmount;
    
        VideoRenderer renderer;
    
        ofFbo drawFbo; //FBO for drawing scene, wihtout fisheye
    
        ofTrueTypeFont	verdana;
    
};
