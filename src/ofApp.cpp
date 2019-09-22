#include "ofApp.h"

void ofApp::setup(){
    ofSetBackgroundColor(255);
    ofEnableAlphaBlending();
//    cam.setDeviceID(0);
    cam.setup(ofGetWidth(), ofGetHeight());
    
    setupGui();
    
//    tracker.setPersistence(persistence);
//    tracker.setMaximumDistance(maxDistance);
    
    bNeedsUpdate = false;
    currentClipperType = ClipperLib::ctIntersection;
}

void ofApp::update(){
    currentTime = ofGetElapsedTimef();
    
    tracker.setPersistence(persistence);
    tracker.setMaximumDistance(maxDistance);
    tracker = contourFinder.getTracker();
    
    cam.update();
    
    if (cam.isFrameNew())
    {
        bNeedsUpdate = true;
        thresholdInput();
        findContours();
        initializeRecording();
        updateRecording();
        
        for (Recording &recording: recordings)
        {
            recording.checkStatus(tracker);
            recording.update();
        }
//        endRecording();
//        addNewRecording();
//        assignPolyType();
//        record();
        
//        for (Recording &recording: recordings)
//        {
//            unsigned int _label = recording.label;
//
//            for (std::size_t i = 0; i < contourFinder.size(); i++)
//            {
//                unsigned int contourLabel = contourFinder.getLabel(i);
//
//                if (_label == contourLabel) recording.frame = contourFinder.getPolyline(i);
//            }
//
//            if (recording.bIsRecording)
//            {
//                record(recording, _label);
//            }
//        }
        
    }
    
    
    
//    updateClipper();
    
//    for (Recording &recording: recordings)
//    {
//        if (!recording.bIsRecording)
//        {
//            std::cout << "Recording " + std::to_string(recording.label) + " is no longer recording." << std::endl;
//        }
//    }
    if (recordings.size() % 5 == 0)
    {
        for (Recording &recording: recordings)
        {
//            std::cout << recording.getStartTime() << std::endl;
        }
//        std::cout << "size: " + std::to_string(recordings.size()) << std::endl;
    }
    
//    std::cout << recordings.size() << std::endl;
    
    if (recordings.size() != 0)
    {
//        std::cout << recordings[0].frames.size() << std::endl;
//        std::cout << recordings[0].bWasRecorded << std::endl;
//        std::cout << recordings[0].startTime << std::endl;
    }
}

void ofApp::draw(){
    ofSetBackgroundColor(255);
    
    ofSetColor(255);
//    cam.draw(0, 0);
//    thresh.draw(0, 0);
    for (Recording &recording: recordings)
    {
        if (recording.wasRecorded()) recording.replay();
    }
//    displayLabelStatus();
    
//    for (Recording &recording: recordings)
//    {
//        if (recording.bWasRecorded) replay(recording);
//    }
    
//    ofSetColor (210, 168, 210);
//    contourFinder.draw();
    
//    drawSubjects();
//    drawMasks();
//    drawClips();
    
//    if (recordings.size() != 0)
//    {
//        ofSetColor(255, 0, 0);
//        recordings[0].frame.draw();
        
//        ofBeginShape();
//        for (std::size_t i = 0; i < recordings[0].frame.size(); i++)
//        {
//            ofVertex(recordings[0].frame[i]);
//        }
//        ofEndShape();
//    }
    
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
    gui.add(persistence.set("Persistence", 96, 0, 500));
    gui.add(maxDistance.set("Maximum Distance", 96, 0, 500));
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
    subjects.clear();
    masks.clear();
    
    for (ofPolyline polyline: contourFinder.getPolylines())
    {
        masks.push_back(polyline);
    }
}

void ofApp::initializeRecording(){
    std::vector<unsigned int> newLabels = tracker.getNewLabels();
    
    for (std::size_t i = 0; i < contourFinder.size(); i++)
    {
        unsigned int label = contourFinder.getLabel(i);
        std::vector<unsigned int>::iterator iter = std::find(newLabels.begin(), newLabels.end(), label);
        
        if (iter != newLabels.end())
        {
            Recording recording{label};
//            recording.setStartTime();
            recordings.push_back(recording);
        }
        
//        if (iter != newLabels.end())
//        {
//            Recording recording;
//            recording.bIsRecording = true;
//            recording.label = _label;
//            recordings.push_back(recording);
//        }
        
//        if (recordings.size() > 20)
//        {
//            recordings.pop_front();
//        }
    }
    
//    std::vector<unsigned int> newLabels = tracker.getNewLabels();
//
//    for (std::size_t i = 0; i < contourFinder.size(); i++)
//    {
//        unsigned int label = contourFinder.getLabel(i);
//
//        std::vector<unsigned int>::iterator iter = std::find(newLabels.begin(), newLabels.end(), label);
//    for (std::size_t i = 0; i < contourFinder.size(); i++)
        //    {
        //        unsigned int label = contourFinder.getLabel(i);
        //
        //        std::vector<unsigned int>::iterator iter = std::find(newLabels.begin(), newLabels.end(), label);
        //
        //        if (iter != newLabels.end())
        //        {
        //            Recording recording;
        //            recording.bIsRecording = true;
        //            recording.startTime = currentTime;
        //            recording.label = label;
        //
        //            recordings.push_back(recording);
        //        }
        //    }
//
//        if (iter != newLabels.end())
//        {
//            Recording recording;
//            recording.bIsRecording = true;
//            recording.startTime = currentTime;
//            recording.label = label;
//
//            recordings.push_back(recording);
//        }
//    }
}

void ofApp::updateRecording(){
    for (std::size_t i = 0; i < contourFinder.size(); i++)
    {
        unsigned int label = contourFinder.getLabel(i);
        
        for (Recording &recording: recordings)
        {
            if (recording.isRecording() && label == recording.getLabel())
            {
                recording.record(contourFinder.getPolyline(i));
            }
//            if (label == recording.getLabel()) recording.update(contourFinder.getPolyline(i));
        }
    }
}

void ofApp::endRecording(){
//    std::vector<unsigned int> deadLabels = tracker.getDeadLabels();
//
//    for (std::size_t i = 0; i < contourFinder.size(); i++)
//    {
//        unsigned int _label = contourFinder.getLabel(i);
//
//        std::vector<unsigned int>::iterator iter = std::find (deadLabels.begin(), deadLabels.end(), _label);
//
//        if (iter != deadLabels.end())
//        {
//            if (recordings[i].label == _label)
//            {
//                recordings[i].bIsRecording = false;
//
//                std::cout << "Recording " + std::to_string(_label) + " has stopped" << std::endl;
//            }
//        }
//    }
}

void ofApp::record(Recording recording, unsigned int label){
//    recording.frames.insert(std::make_pair(currentTime, recording.frame));
//
//    std::vector<unsigned int> deadLabels = tracker.getDeadLabels();
//
//    std::vector<unsigned int>::iterator iter = std::find(deadLabels.begin(), deadLabels.end(), label);
//
//    if (iter != deadLabels.end())
//    {
//        recording.endTime = currentTime;
//        recording.length = recording.endTime - recording.startTime;
//        recording.bWasRecorded = true;
//        recording.bIsRecording = false;
//
//        std::cout << "recording became dead" << std::endl;
//    }
//    else if (recording.frames.size() > 100)
//    {
//        recording.endTime = currentTime;
//        recording.length = recording.endTime - recording.startTime;
//        recording.bWasRecorded = true;
//        recording.bIsRecording = false;
//
//        std::cout << "recording boundary passed" << std::endl;
//    }
}

void ofApp::replay(Recording recording){
//    if (recording.replayStartTime >= recording.startTime + recording.length)
//    {
//        recording.replayStartTime = currentTime;
//    }
//
//    float replayTime = currentTime - recording.replayStartTime + recording.startTime;
//
//    std::map<float, ofPolyline>::iterator lowBound;
//    lowBound = recording.frames.lower_bound(replayTime);
//
//    if (lowBound != recording.frames.end())
//    {
//        recording.currentFrame = lowBound->second;
//    }
//
//    ofSetColor(210, 168, 210);
//
//    ofBeginShape();
//    for (std::size_t i = 0; i < recording.currentFrame.size(); i++)
//    {
//        ofVertex(recording.currentFrame[i]);
//    }
//    ofEndShape();
}

void ofApp::displayLabelStatus(){
    std::vector<unsigned int> newLabels = tracker.getNewLabels();
    std::vector<unsigned int> currentLabels = tracker.getCurrentLabels();
    
    ofColor status;
    for (std::size_t i = 0; i < contourFinder.size(); i++)
    {
        ofPolyline test;
        test = contourFinder.getPolylines()[i];
        
        unsigned int _label = contourFinder.getLabel(i);
        
        std::vector<unsigned int>::iterator newIter = std::find(newLabels.begin(), newLabels.end(), _label);
        
        std::vector<unsigned int>::iterator currentIter = std::find(currentLabels.begin(), currentLabels.end(), _label);
        
        if (newIter != newLabels.end())
        {
            // is a new label
            status = ofColor(255, 0, 0);
        }
        else
        {
            status = ofColor(0, 255, 0);
        }
        
        ofSetColor(status);
        test.draw();
        
    }
        
}
