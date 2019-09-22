//
//  Recording.hpp
//  soft-relations
//
//  Created by Tristan Espinoza on 9/21/19.
//

#pragma once

#include "ofMain.h"

class Recording {
    
    private:
        unsigned int label;
    
        bool bIsRecording;
        bool bWasRecorded;
        float startTime;
    
        ofPolyline currentFrame;
        std::map<float, ofPolyline> frames;
    
    public:
    
        Recording(unsigned int label)
        :label(label)
        ,bIsRecording(true)
        ,bWasRecorded(false)
        ,startTime(ofGetElapsedTimef()){
        }
    
        void start();
        void update();
        
        void record();
    
    float getStartTime() const {
        return startTime;
    }
    
    unsigned int getLabel() const {
        return label;
    }

//        bool bIsRecording;
//        bool bWasRecorded;
    //    float endTime;
    //    float replayStartTime;
    //    float length;
    //    unsigned int label;
    //    ofPolyline frame;
    //    ofPolyline currentFrame;;
    //    std::map<float, ofPolyline> frames;
    //    bool bIsRecording;
    //    bool bWasRecorded;
};
