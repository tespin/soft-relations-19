#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxClipper.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{

	public:
    
        struct Recording
        {
            float startTime;
            float endTime;
            float replayStartTime;
            float length;
            unsigned int label;
            ofPolyline frame;
            ofPolyline currentFrame;;
            std::map<float, ofPolyline> frames;
            bool bIsRecording;
            bool bWasRecorded;
        };
    
		void setup();
		void update();
		void draw();

        void setupGui();
        void thresholdInput();
        void findContours();
        void updateClipper();
        void assignPolyType();
        void addNewRecording();
//        void record();
        void record(Recording recording, unsigned int label);
//        void replay();
        void replay(Recording recording);
    
        void drawSubjects();
        void drawMasks();
        void drawClips();
    
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
    
        ofVideoGrabber cam;
        ofxCv::ContourFinder contourFinder;
        ofxCv::RectTracker tracker;
    
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
    
        ofx::Clipper clipper;
        
        ClipperLib::ClipType currentClipperType;
        
        std::vector<ofPolyline> clips;
        std::vector<ofPolyline> subjects;
        std::vector<ofPolyline> masks;
    
        float currentTime;
//        float replayTime;
//        float recordingStartTime;
//        float recordingEndTime;
//        float replayStartTime;
    
//        std::vector<ofPolyline> recordings;
        std::vector<Recording> recordings;
//        std::map<float, ofPolyline> frames;
    
        bool bNeedsUpdate;
    
};
