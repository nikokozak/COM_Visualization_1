#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Set up the 3D environment
    ofSetFrameRate(60);
    ofEnableDepthTest(); // Enable depth testing for 3D rendering
    ofBackground(0); // Set background to black
}

//--------------------------------------------------------------
void ofApp::update(){
    while(receiver.hasWaitingMessages()){
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        // Format the message
        string msg = "Address: " + m.getAddress();
        for(int i = 0; i < m.getNumArgs(); i++){
            msg += " | Arg " + ofToString(i) + ": " + m.getArgAsString(i);
        }
        
        // Add to messages vector
        messages.push_back(msg);
        
        // Keep only last MAX_MESSAGES
        while(messages.size() > MAX_MESSAGES){
            messages.erase(messages.begin());
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    
    ofSetLineWidth(2);
    
    // Draw first vertical line
    ofSetColor(255, 255, 255);
    ofDrawLine(0, 0, 0, 
               0, 100, 0);
    
    // Calculate rotation angle (complete revolution every 10 seconds)
    float angle = ofGetElapsedTimef() * (360.0f/10.0f);
    
    // Draw diagonal line
    ofSetColor(255, 255, 255);
    ofPushMatrix();
    ofRotateDeg(angle, 0, 1, 0); // Rotate around Y axis
    
    ofPolyline line;
    for (int i = 0; i < 40; i++) {
        // Interpolate from top of vertical line to floor point
        float t = (float)i / 39.0f; // Normalized time from 0 to 1
        
        // Start point: top of vertical line (0, 100, 0)
        // End point: floor plane, 20 units away diagonally
        float x = t * 40.0f;  // Move from 0 to 20 on x-axis
        float y = 100.0f - (t * 100.0f);  // Move from 100 to -100 on y-axis
        float z = t * 40.0f;  // Move from 0 to 20 on z-axis
        
        //ofVertex(x, y, z);
        line.addVertex(x, y, z);
    }
    line.draw();
    //ofEndShape();
    
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
