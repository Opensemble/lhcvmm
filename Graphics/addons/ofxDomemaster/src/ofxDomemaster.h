#pragma once

#include "ofMain.h"

class ofxDomemaster{

	public:
		static const int renderCount = 5;
    ofxDomemaster();
		void setup();
		void draw();
		void drawMask();
    void begin(int i);
    void end(int i);
    void resize(int w, int h);
    void setCameraPosition(float x, float y, float z);
    void setMeshScale(float s);
		int width;
    int height;

  private:
    ofRectangle view;
    vector<ofCamera> renderCamera;
    vector<ofFbo> renderFbo;
    vector<ofVboMesh> renderMesh;
    ofCamera fisheyeCamera;
    ofRectangle fisheyeView;
    ofImage mask;

    float meshScale;
    float meshScaleExt;
    ofNode cameraNode;
    enum positions{
        bottom,
        front,
        left,
        right,
        top
    };

};
