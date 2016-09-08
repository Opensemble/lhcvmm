
#include "VideoRenderer.h"

void VideoRenderer::setup(int iFramerate, RecordingMode initMode, Resolution initRes){
    
    // global variables setup
    bIsRecording = false;
    framerate = iFramerate;
    currentRecMode = initMode;
    currentOutResolution = initRes;
    pngFirstFrame = true;
    
    
    //Fbo y buffers initial setup
    setOutputResolution(currentOutResolution);
    
    
    //videoRecorder setup---------------------------------
    fileName = "movFiles/testMovie";
    fileExt = ".mov"; // ffmpeg uses the extension to determine the container type. run 'ffmpeg -formats' to see supported formats
    // override the default codecs if you like
    // run 'ffmpeg -codecs' to find out what your implementation supports (or -formats on some older versions)
    //vidRecorder.setVideoCodec("mpeg4");
    vidRecorder.setVideoCodec("libx264");//h264 codec
    vidRecorder.setVideoBitrate("800k");
    vidRecorder.setAudioCodec("mp3");
    vidRecorder.setAudioBitrate("192k");
    
    ofAddListener(vidRecorder.outputFileCompleteEvent, this, &VideoRenderer::recordingComplete);
}
//------------------------------
void VideoRenderer::update(){
    
    if(bIsRecording){
        if (currentRecMode == PNG_SEQUENCE) {
            
            if(!pngFirstFrame){
                // wait for the thread to finish saving the
                // previous frame and then unmap it
                saverThread.waitReady();
                pixelBufferBack.unmap();
            }
            
            // copy the fbo texture to a buffer
            fbo.getTexture().copyTo(pixelBufferBack);
            
            // bind and map the buffer as PIXEL_UNPACK so it can be
            // accessed from a different thread  from the cpu
            // and send the memory address to the saver thread
        
            pixelBufferFront.bind(GL_PIXEL_UNPACK_BUFFER);
            unsigned char *  p = pixelBufferFront.map<unsigned char>(GL_READ_ONLY);
            
            saverThread.save(p);
            
            // swap the front and back buffer so we are always
            // copying the texture to one buffer and reading
            // back from another to avoid stalls
            swap(pixelBufferBack,pixelBufferFront);
            
            pngFirstFrame = false;
            
            
        }else if(currentRecMode == MOV_FILE){
            
            fbo.getTexture().readToPixels(pixels);
            bool success = vidRecorder.addFrame(pixels);
            if (!success) {
                ofLogWarning("This frame was not added!");
            }
            
        }
        
    }
    
    // Check if the video recorder encountered any error while writing video frame or audio smaples.
    if (vidRecorder.hasVideoError()) {
        ofLogWarning("The video recorder failed to write some frames!");
    }
    
    if (vidRecorder.hasAudioError()) {
        ofLogWarning("The video recorder failed to write some audio samples!");
    }

}
//------------------------------
void VideoRenderer::draw(int x, int y, int w, int h){
    
    fbo.draw(x, y, w, h);

}
//------------------------------
void VideoRenderer::exit(){
    ofRemoveListener(vidRecorder.outputFileCompleteEvent, this, &VideoRenderer::recordingComplete);
    vidRecorder.close();
}
//--------------------------------------------------------------
void VideoRenderer::startRecording(){
    bIsRecording = true;
    
    saverThread.setLastFrameMarker(ofGetFrameNum());
    saverThread.frameNumOffset = frameNumOffset;
    
    
    if(currentRecMode == MOV_FILE){
        if(!vidRecorder.isInitialized()) {
            //vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, vidGrabber.getWidth(), vidGrabber.getHeight(), 30, sampleRate, channels);
            vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, fbo.getWidth(), fbo.getHeight(), framerate); // no audio
            //            vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, 0,0,0, sampleRate, channels); // no video
            //          vidRecorder.setupCustomOutput(vidGrabber.getWidth(), vidGrabber.getHeight(), 30, sampleRate, channels, "-vcodec mpeg4 -b 1600k -acodec mp2 -ab 128k -f mpegts udp://localhost:1234"); // for custom ffmpeg output string (streaming, etc)
            
            // Start recording
            vidRecorder.start();
        }
    }
    ofLogNotice("-RECORDING STARTED");
}
//--------------------------------------------------------------
void VideoRenderer::stopRecording(){
    bIsRecording = false;
    if(currentRecMode == MOV_FILE) vidRecorder.close();
    ofLogNotice("-RECORDING ENDED");
}
//--------------------------------------------------------------
void VideoRenderer::setOutputResolution(Resolution res){
    
    currentOutResolution = res;
    
    fbo.clear();
    pixels.clear();
    pixelBufferBack.unmap();
    pixelBufferFront.unmap();
    
    switch (res) {
        case r256:
            fbo.allocate(256, 256, GL_RGB);
            break;
        case r512:
            fbo.allocate(512, 512, GL_RGB);
            break;
        case r1024:
            fbo.allocate(1024, 1024, GL_RGB);
            break;
        case r2048:
            fbo.allocate(2048, 2048, GL_RGB);
            break;
        case r4096:
            fbo.allocate(4096, 4096, GL_RGB);
            break;
            
        default:
            break;
    }
    
    
    saverThread.setImageSize(fbo.getWidth(), fbo.getHeight());
    pixelBufferBack.allocate(fbo.getWidth()*fbo.getHeight() *3, GL_DYNAMIC_READ);
    pixelBufferFront.allocate(fbo.getWidth()*fbo.getHeight()*3, GL_DYNAMIC_READ);
}
//--------------------------------------------------------------
void VideoRenderer::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args){
    cout << "The recoded video file is now complete." << endl;
}
//--------------------------------------------------------------
string VideoRenderer::getRecordingModeAsString(){
    
    string s;
    
    switch (currentRecMode) {
        case PNG_SEQUENCE:
            s = "SECUENCIA PNG";
            break;
        case MOV_FILE:
            s = "MOV-H264";
            break;
            
        default:
            break;
    }
    
    return s;
}
//--------------------------------------------------------------
string VideoRenderer::getResolutionAsString(){
    
    string s;
    
    switch (currentOutResolution) {
        case r256:
            s = "256x256";
            break;
        case r512:
            s = "512x512";
            break;
        case r1024:
            s = "1024x1024";
            break;
        case r2048:
            s = "2048x2048";
            break;
        case r4096:
            s = "4096x4096";
            break;
            
        default:
            break;
    }
    
    return s;
    
}