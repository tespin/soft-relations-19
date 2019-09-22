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
    if (bIsRecording) record();
}

void Recording::record(){
        frames.insert(std::map<float, ofPolyline>::value_type(ofGetElapsedTimef(), currentFrame));
}

