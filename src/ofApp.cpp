#include "ofApp.h"

void ofApp::setup(){
    ofSetBackgroundColor(255);
    ofEnableAlphaBlending();
//    cam.setDeviceID(0);
    cam.setup(ofGetWidth(), ofGetHeight());
    
    setupGui();
    
    tracker.setPersistence(1000);
    tracker.setMaximumDistance(1280);
    
    bNeedsUpdate = false;
    currentClipperType = ClipperLib::ctIntersection;
}

void ofApp::update(){
    currentTime = ofGetElapsedTimef();
    tracker = contourFinder.getTracker();
    
    cam.update();
    
    if (cam.isFrameNew())
    {
        bNeedsUpdate = true;
        thresholdInput();
        findContours();
        assignPolyType();
    }
    
    updateClipper();
    
//    if (bNeedsUpdate)
//    {
//                updateClipper();
//        std::cout << clips.size() << std::endl;
//        bNeedsUpdate = false;
//    }
    
    if (clips.size() != 0)
    {   
        // record
        // when/how to end recording?
    }
}

void ofApp::draw(){
    ofSetBackgroundColor(255);
    
    ofSetColor(255);
//    cam.draw(0, 0);
    thresh.draw(0, 0);
    
    ofSetColor (210, 168, 210);
//    contourFinder.draw();
    
    drawSubjects();
    drawMasks();
    drawClips();
    
    gui.draw();
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
//        subject.draw();
        
        ofSetColor(255, 0, 0, 50);
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
//        mask.draw();
        
//        ofSetColor(210, 168, 210, 50);
        ofSetColor(0, 255, 0, 50);
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
//        ofSetColor(182, 255, 143, 100);
        ofSetColor(0, 0, 255, 50);
        ofBeginShape();
        for (std::size_t i = 0; i < clip.size(); i++)
        {
            ofVertex(clip[i]);
        }
        ofEndShape();
    }
}

void ofApp::assignPolyType(){
    std::vector<unsigned int> newLabels = tracker.getNewLabels();
    
    subjects.clear();
    masks.clear();
    for (std::size_t i = 0; i < contourFinder.size(); i++)
    {
        unsigned int label = contourFinder.getLabel(i);
        ofPolyline currentPolyline = contourFinder.getPolyline(i);
        
        if (tracker.existsPrevious(label))
        {
            if (i % 2 == 0)
            {
                subjects.push_back(currentPolyline);
            }
            else
            {
                masks.push_back(currentPolyline);
            }
        }
        
//        std::vector<unsigned int>::iterator iter = std::find(newLabels.begin(), newLabels.end(), label);
//        if (iter != newLabels.end())
//        {
//            if (i % 2 == 0)
//            {
//                subjects.clear();
//                subjects.push_back(contourFinder.getPolyline(i));
//            }
//            else
//            {
//                masks.clear();
//                masks.push_back(contourFinder.getPolyline(i));
//            }
//        }
//        else
//        {
//
//        }
//        bNeedsUpdate = true;
        
    }
}
