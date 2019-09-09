#include "ofApp.h"

void ofApp::setup(){
    cam.setDeviceID(0);
    cam.setup(ofGetWidth(), ofGetHeight());
    setupGui();
    
    contourFinder.getTracker().setPersistence(10);
    contourFinder.getTracker().setMaximumDistance(40);
    
    prevTime = ofGetElapsedTimef();
}

void ofApp::update(){
    currentTime = ofGetElapsedTimef();
    
    cam.update();
    
    if (cam.isFrameNew())
    {
        thresholdInput();
        
        contourFinder.setMinAreaRadius(contourMinArea);
        contourFinder.setMaxAreaRadius(contourMaxArea);
        contourFinder.setFindHoles(findHoles);
        contourFinder.setInvert(invert);
        contourFinder.findContours(thresh);
        
        shape = contourFinder.getPolyline(0);
        shape2 = contourFinder.getPolyline(1);
    }
    
    if (bIsRecording)
    {
        recordings.insert(std::make_pair(currentTime, shape));
    }
    
    timeBetweenReplays = ofGetElapsedTimef() - prevTime;
}

void ofApp::draw(){
    ofSetBackgroundColor(255);
    
    ofSetColor(255);
    thresh.draw(0, 0);
    
    ofPushStyle();
    ofSetColor(210, 168, 210);
//    contourFinder.draw();
    shape.draw();
    ofPopStyle();
    gui.draw();
    
    if (bWasRecorded)
    {
        replay();
    }
}

void ofApp::mousePressed(int x, int y, int button){
    startRecordingTime = ofGetElapsedTimef();
    prevTime = ofGetElapsedTimef();
    bIsRecording = true;
    bWasRecorded = false;
}

void ofApp::mouseReleased(int x, int y, int button){
    endTime = ofGetElapsedTimef();
    recordingLength = endTime - startRecordingTime;
    bIsRecording = false;
    bWasRecorded = true;
    calculateReplayStart();
}

void ofApp::calculateReplayStart(){
    startReplayTime = currentTime;
}

void ofApp::replay() {
    if (replayTime >= startRecordingTime + recordingLength)
    {
        startReplayTime = currentTime;
    }
    
    replayTime = currentTime - startReplayTime + startRecordingTime;
    
    std::map<float, ofPolyline>::iterator lowBound;
    lowBound = recordings.lower_bound(replayTime);
    
    if (lowBound != recordings.end())
    {
        recording = lowBound->second;
    }
    
    ofSetColor(210, 168, 210);
    
    ofBeginShape();
    for (std::size_t i = 0; i < recording.size(); i++)
    {
        ofVertex(recording[i]);
    }
    ofEndShape();
}

void ofApp::setupGui(){
    gui.setup();
    gui.add(blurLevel.set("Blur Level", 0, 0, 30));
    gui.add(threshold.set("Threshold", 65, 0, 255));
    gui.add(erodeIterations.set("Erode Iterations", 3, 0, 50));
    gui.add(dilateIterations.set("Dilate Iterations", 3, 0, 50));
    gui.add(invert.set("Invert", true));
    gui.add(findHoles.set("Find Holes", true));
    gui.add(contourMinArea.set("Contour Min Area", 25, 1, 100));
    gui.add(contourMaxArea.set("Contour Max Area", 200, 1, 1000));
}

void ofApp::thresholdInput(){
    ofxCv::convertColor(cam, thresh, CV_RGB2GRAY);
    ofxCv::threshold(thresh, threshold);
    ofxCv::blur(thresh, thresh, blurLevel);
    ofxCv::erode(thresh, thresh, erodeIterations);
    ofxCv::dilate(thresh, thresh, dilateIterations);
    
    thresh.update();
}
