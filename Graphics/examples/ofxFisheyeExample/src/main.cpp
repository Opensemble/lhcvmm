#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	ofSetLogLevel(OF_LOG_VERBOSE);
	int windowWidth = 768;
	int windowHeight = 768;
    
	#ifdef TARGET_OPENGLES
    ofGLESWindowSettings settings;
	settings.width = windowWidth;
	settings.height = windowHeight;
	settings.setGLESVersion(3);
	ofCreateWindow(settings);
	#else
	ofSetupOpenGL(windowWidth, windowHeight, OF_WINDOW);
	#endif

	ofRunApp( new ofApp());

}
