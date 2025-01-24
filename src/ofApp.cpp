#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    receiver.setup(PORT);
    ofSetBackgroundColor(0);
    ofSetFrameRate(60);
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
    ofSetColor(255);
    
    // Draw messages from bottom to top
    float y = ofGetHeight() - 20;
    for(int i = messages.size() - 1; i >= 0; i--){
        ofDrawBitmapString(messages[i], 20, y);
        y -= 20;
        if(y < 0) break;
    }
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
