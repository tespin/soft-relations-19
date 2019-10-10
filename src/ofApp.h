#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxClipper.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "Recording.h"
#include "ofxKinectForWindows2.h"

class ofApp : public ofBaseApp{

	public:
    
		void setup();
		void update();
		void draw();

        void setupGui();
        void thresholdInput();
        void findContours();
        void updateClipper();
        void assignPolyType();
        void initializeRecording();
        void updateRecording();
        void displayLabelStatus();
        void addPaths();
    
        void drawSubjects();
        void drawMasks();
        void drawClips();
    
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
        void keyPressed(int key);
    
        ofVideoGrabber cam;
        ofxCv::ContourFinder contourFinder;
        ofxCv::RectTracker tracker;
    
        ofImage thresh;
		ofPolyline test;

        ofxKFW2::Device kinect;
		ofImage kinectImg;
    
        ofxPanel gui;
        ofParameter<float> blurLevel;
        ofParameter<unsigned int> threshold;
        ofParameter<bool> invert;
        ofParameter<unsigned int> erodeIterations;
        ofParameter<unsigned int> dilateIterations;
        ofParameter<float> contourMinArea;
        ofParameter<float> contourMaxArea;
        ofParameter<bool> findHoles;
        ofParameter<unsigned int> persistence;
        ofParameter<unsigned int> maxDistance;
    
        ofFbo maskFbo;
        ofFbo faceFbo;
        std::vector<ofPath> paths;
    
        ofx::Clipper clipper;
        ClipperLib::ClipType currentClipperType;
        
        std::vector<ofPolyline> clips;
        std::vector<ofPolyline> subjects;
        std::vector<ofPolyline> masks;
    
        float currentTime;
        std::deque<Recording> recordings;
    
        bool bNeedsUpdate;
        bool bCalibrating;
    
};
