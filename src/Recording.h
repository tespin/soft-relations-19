//
//  Recording.h
//  soft-relations
//
//  Created by Tristan Espinoza on 9/21/19.
//

#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class Recording {
    
    private:
        unsigned int label;
    
        bool bIsRecording;
        bool bWasRecorded;
        bool bIsDead;
        float startTime;
        float endTime;
        float currentReplayTime;
        float replayStartTime;
        float length;
    
        ofPolyline currentFrame;
        ofPolyline currentReplayFrame;
        std::map<float, ofPolyline> frames;
        std::vector<ofPolyline> frameVector;
    
    public:
    
        Recording(unsigned int label)
        :label(label)
        ,bIsRecording(true)
        ,bWasRecorded(false)
        ,bIsDead(false)
        ,startTime(ofGetElapsedTimef())
        ,endTime(0.f)
        ,currentReplayTime(0.f)
        ,replayStartTime(0.f)
        ,length(0.f){
        }
    
        void start();
        void update();
        void checkStatus(ofxCv::RectTracker tracker);

        void record(ofPolyline &frame);
        void endRecording();
        void replay();
        void displayCurrent();
    
        ofPolyline getCurrentFrame() const {
            return currentFrame;
        }
    
        float getStartTime() const {
            return startTime;
        }
    
        unsigned int getLabel() const {
            return label;
        }
    
        bool isRecording() const {
            return bIsRecording;
        }
    
        bool wasRecorded() const {
            return bWasRecorded;
        }
};
