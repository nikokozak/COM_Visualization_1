#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Set up the 3D environment
    ofSetFrameRate(60);
    // ofEnableDepthTest(); // Enable depth testing for 3D rendering
    ofBackground(0); // Set background to black
    
    // Precalculate the base line
    for (int i = 0; i < 40; i++) {
        float t = (float)i / 39.0f;
        float x = distanceFromCenter + t * rotationRadius;
        float y = 100.0f - (t * 200.0f);
        float z = distanceFromCenter + t * rotationRadius;
        baseLine.addVertex(x, y, z);
        controlPoints.push_back(glm::vec3(x, y, z));
    }
    
    // Generated our interpolated points
    interpolatedPoints = evalCR(controlPoints, interpolationSteps);

    // Setup lighting
    light.setPosition(lightPos.x, lightPos.y, lightPos.z);
    light.setDiffuseColor(ofColor(255, 255, 255));
    light.setSpecularColor(ofColor(255, 255, 255));
    //light.setAmbientColor(ofColor(50, 50, 50));  // Add ambient color to light
    light.setPointLight();
    
    // Setup material
    material.setDiffuseColor(ofColor(255, 255, 255, 255));
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    material.setShininess(10);  // Lower value for softer highlights
    material.setAmbientColor(ofColor(50, 50, 50));  // Add ambient color to material

    ofSetGlobalAmbientColor(ofColor(100, 100, 100));  // Adjust values (0-255) to control ambient intensity

    // Setup GUI
    gui.setup("Parameters");
    gui.setPosition(20, 20);
    gui.setDefaultWidth(300);
    gui.setDefaultHeight(20);
    gui.add(height.setup("Cone Height", 0, 0, 500));
    gui.add(segments.setup("Line Segments", 150, 10, 300));
    gui.add(rotationTime.setup("Rotation Time", 10, 1, 30));
    gui.add(snapshotIntervalSlider.setup("Snapshot Interval", 0.05, 0.01, 0.5));
    gui.add(sampleSlider.setup("Samples", 35, 10, 200));
    gui.add(rotationRadiusSlider.setup("Rotation Radius", 250, 50, 1000));
    gui.add(lightPosX.setup("Light Pos X", 300, -1000, 1000));
    gui.add(lightPosY.setup("Light Pos Y", 300, -1000, 1000));
    gui.add(lightPosZ.setup("Light Pos Z", 300, -1000, 1000));
    gui.add(dbMin.setup("DB Min", -30, -0, -100));
    gui.add(dbMax.setup("DB Max", -40, -0, -100));
    gui.add(drawMeshToggle.setup("Draw Mesh", true));
    gui.add(pointSize.setup("Point Size", 2, 1, 10));
    gui.add(distanceFromCenterSlider.setup("Distance From Center", 50, 10, 200));
    gui.add(verticalPointsSlider.setup("Vertical Points", 5, 1, 20));
    gui.add(hueMin.setup("Hue Min", 0, 0, 255));
    gui.add(hueMax.setup("Hue Max", 255, 0, 255));
    gui.add(heightMin.setup("Height Min", -200, -500, 0));
    gui.add(heightMax.setup("Height Max", 200, 0, 500));
    
    // Update initial values
    rotationRadius = rotationRadiusSlider;
    coneHeight = height;
    lineSegments = segments;
    totalRotationTime = rotationTime;
    snapshotInterval = snapshotIntervalSlider;
    samples = sampleSlider;
    distanceFromCenter = distanceFromCenterSlider;
    
    ofxGuiSetFont("DIN Alternate Bold", 10);
    ofxGuiSetDefaultWidth(300);
    ofxGuiSetDefaultHeight(18);
    ofxGuiSetTextPadding(4);
    ofxGuiSetFillColor(ofColor(128, 128, 128));
    ofxGuiSetBackgroundColor(ofColor(0, 0, 0));
    ofxGuiSetBorderColor(ofColor(200, 200, 200));

    // Add near start of setup
    receiver.setup(PORT);
    messages.reserve(MAX_MESSAGES);

    // Add to setup
    fftData.resize(FFT_SIZE, 0.0f);  // Initialize with zeros
}

//--------------------------------------------------------------
void ofApp::update(){
    // Add near start of update
    while(receiver.hasWaitingMessages()) {
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        if(m.getAddress() == "/fft") {
            // Ensure fftData is the same size as the incoming data
            int numArgs = m.getNumArgs();
            if (numArgs != fftData.size()) {
                fftData.resize(numArgs, 0.0f);
                // Control points needs to be 3n + 1
                controlPoints.resize(numArgs, glm::vec3(0.0f, 0.0f, 0.0f));
            }

            // Update fftData with new values
            for(int i = 0; i < numArgs; i++) {
                fftData[i] = m.getArgAsFloat(i);
                controlPoints[i] = glm::vec3(fftData[i], 0.0f, 0.0f);
            }

            string msg = m.getAddress();
            for(int i = 0; i < m.getNumArgs(); i++) {
                msg += " " + m.getArgAsString(i);
            }
            messages.push_back(msg);
            if(messages.size() > MAX_MESSAGES) {
                messages.erase(messages.begin());
            }
        }
    }
    float currentTime = ofGetElapsedTimef();
    rotationAngle = (currentTime / totalRotationTime) * 360.0f;

    baseLine.clear();
    verticalLines.clear();  // Clear previous vertical lines

    light.setPosition(lightPosX, lightPosY, lightPosZ);
    lightPos.x = lightPosX;
    lightPos.y = lightPosY;
    lightPos.z = lightPosZ;

    interpolatedPoints = evalCR(controlPoints, interpolationSteps);

    // Recalculate the base line and vertical lines
    for (int i = 0; i < interpolationSteps; i++) {
        float t = (float)i / (interpolationSteps - 1.0f);
        float timeScale = 0.05;
        float noiseVal = ofMap(interpolatedPoints[i].x, dbMax, dbMin, -40, 40, true);
        float x = distanceFromCenter + t * rotationRadius;
        float y = (coneHeight - (t * (coneHeight * 2))) + noiseVal;
        float z = distanceFromCenter + t * rotationRadius;
        baseLine.addVertex(x, y, z);

        // Create vertical line for this base point
        ofPolyline verticalLine;
        verticalLine.addVertex(x, y, z);  // Start at base point
        
        // Add vertical points
        int numVerticalPoints = verticalPointsSlider;
        float verticalSpacing = 50.0f;  // Adjust this value to change spacing between points
        for(int j = 1; j <= numVerticalPoints; j++) {
            verticalLine.addVertex(x, y + (j * verticalSpacing), z);
        }
        
        verticalLines.push_back(verticalLine);
    }
    
    if (currentTime - lastSnapshotTime >= snapshotInterval) {
        takeSnapshot();
        lastSnapshotTime = currentTime;
    }

    // Update values each frame
    rotationRadius = rotationRadiusSlider;
    coneHeight = height;
    lineSegments = segments;
    totalRotationTime = rotationTime;
    snapshotInterval = snapshotIntervalSlider;
    distanceFromCenter = distanceFromCenterSlider;
    samples = sampleSlider;
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
        
        // Add normals for lighting when creating triangles
        for (int i = 0; i < currentVerts.size() - 1; i++) {
            ofVec3f v1 = prevVerts[i];
            ofVec3f v2 = currentVerts[i];
            ofVec3f v3 = currentVerts[i + 1];
            ofVec3f normal = ((v2 - v1).cross(v3 - v1)).normalized();
            
            // First triangle
            mesh.addVertex(prevVerts[i]);
            mesh.addVertex(currentVerts[i]);
            mesh.addVertex(currentVerts[i + 1]);
            
            mesh.addNormal(normal);
            mesh.addNormal(normal);
            mesh.addNormal(normal);
            
            // Second triangle
            mesh.addVertex(prevVerts[i]);
            mesh.addVertex(currentVerts[i + 1]);
            mesh.addVertex(prevVerts[i + 1]);
            
            mesh.addNormal(normal);
            mesh.addNormal(normal);
            mesh.addNormal(normal);
            
            // Add colors with full opacity for better lighting
            ofColor color(0, 128, 255, 255);
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
                ofVec3f v1 = prevVerts[j];
                ofVec3f v2 = currentVerts[j];
                ofVec3f v3 = currentVerts[j + 1];
                ofVec3f normal = ((v2 - v1).cross(v3 - v1)).normalized();
                
                mesh.addVertex(prevVerts[j]);
                mesh.addVertex(currentVerts[j]);
                mesh.addVertex(currentVerts[j + 1]);
                
                mesh.addVertex(prevVerts[j]);
                mesh.addVertex(currentVerts[j + 1]);
                mesh.addVertex(prevVerts[j + 1]);
                
                mesh.addNormal(normal);
                mesh.addNormal(normal);
                mesh.addNormal(normal);
                
                mesh.addNormal(normal);
                mesh.addNormal(normal);
                mesh.addNormal(normal);
                
                ofColor color(0, 128, 255, 255);
                for (int k = 0; k < 6; k++) {
                    mesh.addColor(color);
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableLighting();

    // Draw Helpers
    if (showGui) {
        ofSetColor(255);  // Reset color to white before drawing GUI
        gui.draw();
        ofDisableLighting();
    }
        
    cam.begin();
    
    ofEnableAlphaBlending();
    ofEnableDepthTest();

    // Set up light before rotation
    light.enable();
    light.setPosition(0, 0, 500);  // Center light above scene
    
    // Begin pushMatrix to make everything stationary
    ofPushMatrix();
    ofRotateDeg(-rotationAngle, 0,1,0);
    
    if(drawMeshToggle) {
        // Draw mesh with material
        material.begin();
        mesh.drawFaces();
        material.end();
    } else {
        // Draw points and their trails
        ofEnableAlphaBlending();
        glPointSize(pointSize);
        
        // For each vertex position in the current snapshot
        int numVerts = lineSnapshots[0].getVertices().size();
        for(int vertIndex = 0; vertIndex < numVerts; vertIndex++) {
            // Draw trail through time for this vertex position
            glBegin(GL_LINE_STRIP);
            for(size_t snapIndex = 0; snapIndex < lineSnapshots.size(); snapIndex++) {
                auto& vert = lineSnapshots[snapIndex].getVertices()[vertIndex];
                // Map height to hue
                float hue = ofMap(vert.y, heightMin, heightMax, hueMin, hueMax, true);
                ofColor col;
                col.setHsb(hue, 200, 255, 255);
                ofSetColor(col);
                glVertex3f(vert.x, vert.y, vert.z);
            }
            glEnd();
            
            // Draw points over the trails
            glBegin(GL_POINTS);
            for(const auto& snapshot : lineSnapshots) {
                auto& vert = snapshot.getVertices()[vertIndex];
                // Map height to hue for points
                float hue = ofMap(vert.y, heightMin, heightMax, hueMin, hueMax, true);
                ofColor col;
                col.setHsb(hue, 200, 255, 255);
                ofSetColor(col);
                glVertex3f(vert.x, vert.y, vert.z);
            }
            glEnd();
        }
        ofDisableAlphaBlending();
    }
    
    // Disable lighting for lines
    ofDisableLighting();
    
    // Current line
    ofPushMatrix();
    ofRotateDeg(rotationAngle, 0, 1, 0);
    ofSetColor(0, 255, 0);
    ofSetLineWidth(2);
    baseLine.draw();
    ofPopMatrix();

    ofDisableAlphaBlending();
    ofPopMatrix();

    light.disable();

    // Add near end of draw, after cam.end()
    ofSetColor(255);
    float y = 20;
    for(const auto& msg : messages) {
        ofDrawBitmapString(msg, 20, y);
        y += 20;
    }

    cam.end();
    ofDisableDepthTest();

/*     // Draw instructions
    ofDrawBitmapString("Press 'l' to toggle light movement mode\n"
                      "When active, use mouse to move light\n"
                      "Press 's' to save light position\n"
                      "Press 'h' to show/hide GUI", 20, ofGetHeight() - 80); */
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'l' || key == 'L') {
        movingLight = !movingLight;
    }
    if(key == 's' || key == 'S') {
        // Print current light position to console
        ofLogNotice() << "Light position: " << lightPos.x << ", " << lightPos.y << ", " << lightPos.z;
    }
    if(key == 'h' || key == 'H') {
        showGui = !showGui;  // Fixed visibility toggle
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(movingLight) {
        // Convert mouse movement to world space
        ofVec3f mousePoint = cam.screenToWorld(ofVec3f(x, y, 0));
        lightPos = mousePoint;
        light.setPosition(lightPos);
    }
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

// Add this new method
void ofApp::paramChanged(float& value) {
    totalRotationTime = totalRotationTimeParam;
    snapshotInterval = snapshotIntervalParam;
    rotationRadius = rotationRadiusParam;
    coneHeight = coneHeightParam;
    lineSegments = lineSegmentsParam;
    
    // Clear existing mesh and snapshots when parameters change
    mesh.clear();
    lineSnapshots.clear();
}
