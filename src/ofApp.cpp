#include "ofApp.h"

void ofApp::setup(){
    ofSetBackgroundColor(255);
    ofEnableAlphaBlending();
//    cam.setDeviceID(0);
    cam.setup(ofGetWidth(), ofGetHeight());
    
    setupGui();
    
    tracker.setPersistence(500);
    tracker.setMaximumDistance(640);
    
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
        addNewRecording();
//        assignPolyType();
//        record();
        
        for (Recording &recording: recordings)
        {
            unsigned int _label = recording.label;
            
            for (std::size_t i = 0; i < contourFinder.size(); i++)
            {
                unsigned int contourLabel = contourFinder.getLabel(i);
                
                if (_label == contourLabel) recording.frame = contourFinder.getPolyline(i);
            }
            
            if (recording.bIsRecording)
            {
                record(recording, _label);
            }
        }
        
    }
    
    updateClipper();
    
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
    thresh.draw(0, 0);
    
//    for (Recording &recording: recordings)
//    {
//        if (recording.bWasRecorded) replay(recording);
//    }
    
    ofSetColor (210, 168, 210);
    contourFinder.draw();
    
    drawSubjects();
    drawMasks();
    drawClips();
    
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
    subjects.clear();
    masks.clear();
    
    for (ofPolyline polyline: contourFinder.getPolylines())
    {
        masks.push_back(polyline);
    }
    
//    void ofApp::replay() {
//        if (replayTime >= startRecordingTime + recordingLength)
//        {
//            startReplayTime = currentTime;
//        }
//        
//        replayTime = currentTime - startReplayTime + startRecordingTime;
//        
//        std::map<float, ofPolyline>::iterator lowBound;
//        lowBound = recordings.lower_bound(replayTime);
//        
//        if (lowBound != recordings.end())
//        {
//            recording = lowBound->second;
//        }
//        
//        ofSetColor(210, 168, 210);
//        
//        ofBeginShape();
//        for (std::size_t i = 0; i < recording.size(); i++)
//        {
//            ofVertex(recording[i]);
//        }
//        ofEndShape();
//    }
//    std::vector<unsigned int> newLabels = tracker.getNewLabels();
//
//    subjects.clear();
//    masks.clear();
//    for (std::size_t i = 0; i < contourFinder.size(); i++)
//    {
//        unsigned int label = contourFinder.getLabel(i);
//        ofPolyline currentPolyline = contourFinder.getPolyline(i);
//
//        if (tracker.existsPrevious(label))
//        {
//            if (i % 2 == 0)
//            {
//                subjects.push_back(currentPolyline);
//            }
//            else
//            {
//                masks.push_back(currentPolyline);
//            }
//        }
    
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

void ofApp::addNewRecording(){
    std::vector<unsigned int> newLabels = tracker.getNewLabels();
    
    for (std::size_t i = 0; i < contourFinder.size(); i++)
    {
        unsigned int label = contourFinder.getLabel(i);
        
        std::vector<unsigned int>::iterator iter = std::find(newLabels.begin(), newLabels.end(), label);
        
        if (iter != newLabels.end())
        {
            Recording recording;
            recording.bIsRecording = true;
            recording.startTime = currentTime;
            recording.label = label;
            
            recordings.push_back(recording);
        }
    }
}

void ofApp::record(Recording recording, unsigned int label){
    recording.frames.insert(std::make_pair(currentTime, recording.frame));
    
    std::vector<unsigned int> deadLabels = tracker.getDeadLabels();
    
    std::vector<unsigned int>::iterator iter = std::find(deadLabels.begin(), deadLabels.end(), label);
    
    if (iter != deadLabels.end())
    {
        recording.endTime = currentTime;
        recording.length = recording.endTime - recording.startTime;
        recording.bWasRecorded = true;
        recording.bIsRecording = false;
        
        std::cout << "recording became dead" << std::endl;
    }
    else if (recording.frames.size() > 100)
    {
        recording.endTime = currentTime;
        recording.length = recording.endTime - recording.startTime;
        recording.bWasRecorded = true;
        recording.bIsRecording = false;
        
        std::cout << "recording boundary passed" << std::endl;
    }
}

//void ofApp::record(){
//    for (Recording &recording: recordings)
//    {
//        std::vector<unsigned int> deadLabels = tracker.getDeadLabels();
//        unsigned int label = recording.label;
//
//        for (std::size_t i = 0; i < contourFinder.size(); i++)
//        {
//            unsigned int contourLabel = contourFinder.getLabel(i);
//
//            if (label == contourLabel) recording.frame = contourFinder.getPolyline(i);
//        }
//
//        if (recording.bIsRecording)
//        {
//            recording.frames.insert(std::make_pair(currentTime, recording.frame));
//
//            std::vector<unsigned int>::iterator iter = std::find(deadLabels.begin(), deadLabels.end(), label);
//
//            if (iter != deadLabels.end())
//            {
//                recording.endTime = currentTime;
//                recording.length = recording.endTime - recording.startTime;
//                recording.bIsRecording = false;
//            }
//            else if (recording.frames.size() > 1000)
//            {
//                recording.endTime = currentTime;
//                recording.length = recording.endTime - recording.startTime;
//                recording.bIsRecording = false;
//            }
//        }
//    }
//}

void ofApp::replay(Recording recording){
    if (recording.replayStartTime >= recording.startTime + recording.length)
    {
        recording.replayStartTime = currentTime;
    }
    
    float replayTime = currentTime - recording.replayStartTime + recording.startTime;
    
    std::map<float, ofPolyline>::iterator lowBound;
    lowBound = recording.frames.lower_bound(replayTime);
    
    if (lowBound != recording.frames.end())
    {
        recording.currentFrame = lowBound->second;
    }
    
    ofSetColor(210, 168, 210);

    ofBeginShape();
    for (std::size_t i = 0; i < recording.currentFrame.size(); i++)
    {
        ofVertex(recording.currentFrame[i]);
    }
    ofEndShape();
}

//void ofApp::replay(){
//    for (Recording &recording: recordings)
//    {
//        if (recording.replayStartTime >= recording.startTime + recording.length)
//        {
//            recording.replayStartTime = currentTime;
//        }
//
//        float replayTime = currentTime - recording.replayStartTime + recording.startTime;
//
//        std::map<float, ofPolyline>::iterator lowBound;
//        lowBound = recording.frames.lower_bound(replayTime);
//
//        if (lowBound != recording.frames.end())
//        {
//            recording.currentFrame = lowBound->second;
//        }
//
//    }

    //        if (replayTime >= startRecordingTime + recordingLength)
    //        {
    //            startReplayTime = currentTime;
    //        }
    //
    //        replayTime = currentTime - startReplayTime + startRecordingTime;
    //
    //        std::map<float, ofPolyline>::iterator lowBound;
    //        lowBound = recordings.lower_bound(replayTime);
    //
    //        if (lowBound != recordings.end())
    //        {
    //            recording = lowBound->second;
    //        }
    //
    //        ofSetColor(210, 168, 210);
    //
    //        ofBeginShape();
    //        for (std::size_t i = 0; i < recording.size(); i++)
    //        {
    //            ofVertex(recording[i]);
    //        }
    //        ofEndShape();
//}

