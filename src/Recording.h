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
        bool bIsRecording;
        bool bWasRecorded;
    
    public:
    
        Recording() : bIsRecording{true}, bWasRecorded{false} {}
    
        void start();
    
    
//        bool bIsRecording;
//        bool bWasRecorded;
    //    float startTime;
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
