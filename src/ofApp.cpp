#include "ofApp.h"

void ofApp::setup(){
    ofSetBackgroundColor(255);
    ofEnableAlphaBlending();
    cam.setDeviceID(0);
    cam.setup(ofGetWidth(), ofGetHeight());

    kinect.open();
    kinect.initColorSource();
	kinect.initBodyIndexSource();
    
    setupGui();
    
    bNeedsUpdate = false;
    bCalibrating = false;
    currentClipperType = ClipperLib::ctIntersection;
    
    maskFbo.allocate(1280, 720);
    faceFbo.allocate(1280, 720);
	kinectImg.allocate(1280, 720, OF_IMAGE_COLOR);
}

void ofApp::update(){
    currentTime = ofGetElapsedTimef();
    
    tracker.setPersistence(persistence);
    tracker.setMaximumDistance(maxDistance);
    tracker = contourFinder.getTracker();
    
    cam.update();
    kinect.update();
    
    if (cam.isFrameNew())
    {
        thresholdInput();
        findContours();
        addPaths();
        
//        maskFbo.begin();
//        ofClear(0, 0, 0, 0);
//        for (auto &path: paths)
//        {
//            path.draw();
//        }
//        maskFbo.end();
//
//        faceFbo.begin();
//        ofClear(0, 0, 0 , 0);
//        cam.getTexture().setAlphaMask(maskFbo.getTexture());
//        cam.draw(0, 0);
//        faceFbo.end();
        
        if (recordings.size() > 10) recordings.pop_front();
//        std::cout << "Size: " + recordings.size() << std::endl;
        
        initializeRecording();
        updateRecording();
        
        for (Recording &recording: recordings)
        {
            recording.checkStatus(tracker);
            recording.update();
        }
        
        assignPolyType();
        
        if (bNeedsUpdate)
        {
            updateClipper();
        }
        
        bNeedsUpdate = false;
//        updateClipper();
    }
    
//    maskFbo.begin();
//    ofClear(0, 0, 0, 0);
//    for (auto &path: paths)
//    {
//        path.draw();
//    }
//    maskFbo.end();
//    maskFbo.draw(0, 0);
//
//    faceFbo.begin();
//    ofClear(0, 0, 0 , 0);
//    cam.getTexture().setAlphaMask(maskFbo.getTexture());
//    cam.draw(0, 0);
//    faceFbo.end();
}

void ofApp::draw(){
    ofSetBackgroundColor(255);
    
    ofSetColor(255);
//    cam.draw(0, 0);
	kinect.getBodyIndexSource()->draw(0, 0, 1280, 720);
	contourFinder.draw();

    if (bCalibrating)
    {
        thresh.draw(0, 0);
        
        ofSetColor(255, 0, 0);
        //contourFinder.draw();
    }
    else
    {
        for (Recording &recording: recordings)
        {
            if (recording.isRecording()) recording.displayCurrent();
            else if (recording.wasRecorded())
            {
                if (recording.getFrames().size() > 20) recording.replay();
            }
        }
    }
    
//    faceFbo.draw(0, 0);
    
    gui.draw();
//    drawSubjects();
//    drawMasks();
//    drawClips();
}

void ofApp::mousePressed(int x, int y, int button){
}

void ofApp::mouseReleased(int x, int y, int button){
}

void ofApp::keyPressed(int key){
    switch(key)
    {
        case 'c':
            bCalibrating = !bCalibrating;
            break;
        case 'n':
            recordings.clear();
        default:
            break;
    }
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
    
	//ofPixels &pixels = kinect.getColorSource()->getPixels();
	//kinectImg.setFromPixels(pixels);

    ofxCv::convertColor(cam, thresh, CV_RGB2GRAY);
    ofxCv::threshold(thresh, threshold);
    ofxCv::blur(thresh, thresh, blurLevel);
    ofxCv::erode(thresh, thresh, erodeIterations);
    ofxCv::dilate(thresh, thresh, dilateIterations);
    
//    if (bCalibrating) thresh.resize(640, 320);
//    else {}
    
    thresh.update();
}

void ofApp::findContours(){
    contourFinder.setMinAreaRadius(contourMinArea);
    contourFinder.setMaxAreaRadius(contourMaxArea);
    contourFinder.setFindHoles(findHoles);
    contourFinder.setInvert(invert);
    //contourFinder.findContours(thresh);
	contourFinder.findContours(kinect.getBodyIndexSource()->getPixels());
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
        ofSetColor(255, 0, 0, 10);
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
        ofSetColor(0, 255, 0, 10);
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
        ofSetColor(126, 255, 182, 75);
        ofBeginShape();
        for (std::size_t i = 0; i < clip.size(); i++)
        {
            ofVertex(clip[i]);
        }
        ofEndShape();
    }
}

void ofApp::assignPolyType(){
    masks.clear();
    subjects.clear();
    
    for (ofPolyline polyline: contourFinder.getPolylines())
    {
        masks.push_back(polyline);
    }
    
    for (Recording &recording: recordings)
    {
        subjects.push_back(recording.getCurrentFrame());
    }
    
//    std::cout << "masks: " + std::to_string(masks.size()) << std::endl;
//    std::cout << "subjects: " + std::to_string(subjects.size()) << std::endl;
    
    bNeedsUpdate = true;
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
            recordings.push_back(recording);
        }
    }
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
        }
    }
}

void ofApp::addPaths(){
    paths.clear();
    for (std::size_t i = 0; i < contourFinder.size(); i++)
    {
        ofPath path = ofPath();
        ofPolyline poly = contourFinder.getPolyline(i);
        for (std::size_t i = 0; i < poly.size(); i++)
        {
            if (i == 0)
            {
                path.newSubPath();
                path.moveTo(poly[i]);
            }
            else path.lineTo(poly[i]);
        }
        path.close();
        paths.push_back(path);

    }
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
