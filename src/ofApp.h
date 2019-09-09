#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxClipper.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

        void setupGui();
        void thresholdInput();
    
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
    
        void replay();
        void calculateReplayStart();
    
        ofVideoGrabber cam;
        ofxCv::ContourFinder contourFinder;
    
        ofImage thresh;
    
        ofxPanel gui;
        ofParameter<float> blurLevel;
        ofParameter<unsigned int> threshold;
        ofParameter<bool> invert;
        ofParameter<unsigned int> erodeIterations;
        ofParameter<unsigned int> dilateIterations;
        ofParameter<float> contourMinArea;
        ofParameter<float> contourMaxArea;
        ofParameter<bool> findHoles;
    
        ofPolyline recording;
        ofPolyline shape;
        ofPolyline shape2;
    
        bool bIsRecording;
        bool bWasRecorded;
    
        float currentTime;
        float startRecordingTime;
        float endTime;
        float recordingLength;
        float replayTime;
        float prevTime;
        float startReplayTime;
        float timeBetweenReplays;
    
        std::map<float, ofPolyline> recordings;
    
};
