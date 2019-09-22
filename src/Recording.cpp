//
//  Recording.cpp
//  soft-relations
//
//  Created by Tristan Espinoza on 9/21/19.
//

#include "Recording.h"

void Recording::start(){
    
}

void Recording::update(){
    if (bIsDead || frames.size() > 199) endRecording();
}

void Recording::checkStatus(ofxCv::RectTracker tracker){
    std::vector<unsigned int> deadLabels = tracker.getDeadLabels();
    
    std::vector<unsigned int>::iterator iter = std::find(deadLabels.begin(), deadLabels.end(), label);
    
    if (iter != deadLabels.end())
    {
        std::cout << "Recording: " + std::to_string(label) + "is dead." << std::endl;
        bIsDead = true;
    }
}

void Recording::record(ofPolyline &frame){
    frames.insert(std::map<float, ofPolyline>::value_type(ofGetElapsedTimef(), frame));
    
    std::cout <<
        "Recording: "
        + std::to_string(label)
        + "\nFrames: "
        + std::to_string(frames.size())
    << std::endl;    
}

void Recording::endRecording(){
    if (!bWasRecorded)
    {
        bIsRecording = false;
        endTime = ofGetElapsedTimef();
        length = endTime - startTime;
        replayStartTime = ofGetElapsedTimef();
        
        std::cout <<
            "Recording " + std::to_string(label)
            + " ended with " + std::to_string(frames.size()) + " frames. "
            + "\nStart: " + std::to_string(startTime)
            + "\nEnd: " + std::to_string(endTime)
            + "\nLength: " + std::to_string(length)
        << std::endl;
        
        bWasRecorded = true;
    }
}

void Recording::replay(){
    if (currentReplayTime >= startTime + length) replayStartTime = ofGetElapsedTimef();
    
    currentReplayTime = ofGetElapsedTimef() - replayStartTime + startTime;
    
    std::map<float, ofPolyline>::iterator lowBound;
    lowBound = frames.lower_bound(currentReplayTime);
    
    if (lowBound != frames.end()) currentFrame = lowBound->second;
    
    ofSetColor(255, 0, 255, 50);
    
    ofBeginShape();
    for (std::size_t i = 0; i < currentFrame.size(); i++)
    {
        ofVertex(currentFrame[i]);
    }
    ofEndShape();
}
