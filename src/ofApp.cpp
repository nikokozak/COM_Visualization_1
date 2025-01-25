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
    snapshot.getVertices();
    snapshot.rotateDeg(rotationAngle, ofVec3f(0,1,0));
    lineSnapshots.push_back(snapshot);
    
    // Create mesh faces between last two snapshots
    if (lineSnapshots.size() >= 2) {
        auto& current = lineSnapshots.back();
        auto& previous = lineSnapshots[lineSnapshots.size() - 2];
        
        // Get vertices from both lines
        auto& currentVerts = current.getVertices();
        auto& prevVerts = previous.getVertices();
        
        // Create triangles between the lines
        for (int i = 0; i < currentVerts.size() - 1; i++) {
            // First triangle
            mesh.addVertex(prevVerts[i]);
            mesh.addVertex(currentVerts[i]);
            mesh.addVertex(currentVerts[i + 1]);
            
            // Second triangle
            mesh.addVertex(prevVerts[i]);
            mesh.addVertex(currentVerts[i + 1]);
            mesh.addVertex(prevVerts[i + 1]);
            
            // Add colors with alpha for semi-transparency
            ofColor color(0, 128, 255, 150);
            mesh.addColor(color);
            mesh.addColor(color);
            mesh.addColor(color);
            mesh.addColor(color);
            mesh.addColor(color);
            mesh.addColor(color);
        }
    }
    
    if (lineSnapshots.size() > samples) {
        lineSnapshots.erase(lineSnapshots.begin());
        // Optional: remove old vertices/faces from mesh
        mesh.clear();
        // Rebuild mesh from remaining snapshots
        for (size_t i = 0; i < lineSnapshots.size() - 1; i++) {
            auto& current = lineSnapshots[i + 1];
            auto& previous = lineSnapshots[i];
            auto& currentVerts = current.getVertices();
            auto& prevVerts = previous.getVertices();
            
            for (int j = 0; j < currentVerts.size() - 1; j++) {
                mesh.addVertex(prevVerts[j]);
                mesh.addVertex(currentVerts[j]);
                mesh.addVertex(currentVerts[j + 1]);
                
                mesh.addVertex(prevVerts[j]);
                mesh.addVertex(currentVerts[j + 1]);
                mesh.addVertex(prevVerts[j + 1]);
                
                ofColor color(0, 128, 255, 150);
                for (int k = 0; k < 6; k++) {
                    mesh.addColor(color);
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    
    ofEnableAlphaBlending();
    
    // Draw mesh
    ofSetColor(255);
    mesh.drawFaces();
    
    // Draw snapshots (optional now - you might want to remove these)
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
    
    ofDisableAlphaBlending();
    
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
