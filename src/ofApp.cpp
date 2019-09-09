#include "ofApp.h"

void ofApp::setup(){
    ofSetBackgroundColor(255);
    
    currentClipperType = ClipperLib::ctIntersection;
}

void ofApp::update(){
    currentTime = ofGetElapsedTimef();
    
    updateClipper();
    
    if (clips.size() != 0)
    {   
        // record
        // when/how to end recording?
    }
}

void ofApp::draw(){
    ofSetBackgroundColor(255);
    ofSetColor(255);
    
    drawSubjects();
    drawMasks();
    drawClips();
}

void ofApp::mousePressed(int x, int y, int button){
}

void ofApp::mouseReleased(int x, int y, int button){
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

void ofApp::findContours(){
    contourFinder.setMinAreaRadius(contourMinArea);
    contourFinder.setMaxAreaRadius(contourMaxArea);
    contourFinder.setFindHoles(findHoles);
    contourFinder.setInvert(invert);
    contourFinder.findContours(thresh);
}

void ofApp::updateClipper(){
    clips.clear();
    clipper.Clear();
    clipper.addPolylines(subjects, ClipperLib::ptSubject);
    clipper.addPolylines(masks, ClipperLib::ptClip);
    
    clips = clipper.getClipped(currentClipperType);
}

void ofApp::drawSubjects(){
    for (ofPolyline &subject: subjects)
    {
        ofSetColor(210, 168, 210, 50);
        ofBeginShape();
        for (std::size_t i = 0; i < subject.size(); i++)
        {
            ofVertex(subject[i]);
        }
        ofEndShape();
    }
}

void ofApp::drawMasks(){
    for (ofPolyline &mask: masks)
    {
        ofSetColor(210, 168, 210, 50);
        ofBeginShape();
        for (std::size_t i = 0; i < mask.size(); i++)
        {
            ofVertex(mask[i]);
        }
        ofEndShape();
    }
}

void ofApp::drawClips(){
    for (ofPolyline &clip: clips)
    {
        ofSetColor(182, 255, 143, 100);
        ofBeginShape();
        for (std::size_t i = 0; i < clips.size(); i++)
        {
            ofVertex(clip[i]);
        }
        ofEndShape();
    }
}
