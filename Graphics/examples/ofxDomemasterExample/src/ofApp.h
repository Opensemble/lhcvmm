#pragma once

#include "ofMain.h"
#include "ofxDomemaster.h"

#include "VideoRenderer.h"

#define FRAME_RATE 30
#define DURATION 4.0

class ofApp : public ofBaseApp{
	private:
		float meshScale = 0.66f;
		bool showCubemap = false;
		bool showMask = true;

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawScene(int i , int width, int height);

		ofxDomemaster domemaster;
		ofSpherePrimitive sphere;
		ofBoxPrimitive box;

		//animation data variables----------------
		bool    isAnimating;
		bool    isPlayingForward;
    int     frameCounter;//animation Frame Counter
    float   frameDuration;//Duration in seconds of each frame
    int     framesMaxNumber;//Number of frames of the entire animation
    float   animValue;//Current frame in relationship with the duration of the entire animation (0.0 - 1.0)
    float   animationTime;

		ofFbo sceneFbo; //FBO for drawing scene
    ofTrueTypeFont font;
    ofShader shader;

};
