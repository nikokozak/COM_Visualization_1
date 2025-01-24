#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Set up the 3D environment
    ofSetFrameRate(60);
    ofEnableDepthTest(); // Enable depth testing for 3D rendering
    ofBackground(0); // Set background to black
    
    // Precalculate the base line
    for (int i = 0; i < 40; i++) {
        float t = (float)i / 39.0f;
        float x = t * rotationRadius;
        float y = 100.0f - (t * 200.0f);
        float z = t * rotationRadius;
        baseLine.addVertex(x, y, z);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    float currentTime = ofGetElapsedTimef();
    rotationAngle = (currentTime / totalRotationTime) * 360.0f;

    baseLine.clear();

        // Recalculate the base line
    for (int i = 0; i < 40; i++) {
        float t = (float)i / 39.0f;
        float x = t * rotationRadius;
        float y = (100.0f - (t * 200.0f)) + (ofNoise(rotationAngle * t) * 20.0f) - 20.0f;
        float z = t * rotationRadius;
        baseLine.addVertex(x, y, z);
    }
    
    if (currentTime - lastSnapshotTime >= snapshotInterval) {
        takeSnapshot();
        lastSnapshotTime = currentTime;
    }
}

void ofApp::takeSnapshot() {
    ofPolyline snapshot = baseLine;
    snapshot.getVertices();  // Force vertex calculation
    snapshot.rotateDeg(rotationAngle, ofVec3f(0,1,0));
    lineSnapshots.push_back(snapshot);
    
    if (lineSnapshots.size() > samples) {
        lineSnapshots.erase(lineSnapshots.begin());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    
    // Static vertical line
    ofSetColor(255);
    // ofDrawLine(0, -100, 0, 0, 100, 0);
    
    // Draw snapshots
    for (size_t i = 0; i < lineSnapshots.size(); ++i) {
        float alpha = ofMap(i, 0, lineSnapshots.size(), 50, 200);
        ofSetColor(255, 255, 255, alpha);
        lineSnapshots[i].draw();
    }
    
    // Current line
    ofPushMatrix();
    ofRotateDeg(rotationAngle, 0, 1, 0);
    ofSetColor(0, 255, 0);
    ofSetLineWidth(2);
    baseLine.draw();
    ofPopMatrix();
    
    cam.end();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
