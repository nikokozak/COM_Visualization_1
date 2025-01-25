#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"

// listening port
#define PORT 55555

class ofApp : public ofBaseApp{

	public:
		void setup() override;
		void update() override;
		void draw() override;
		void exit() override;

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
		void mouseEntered(int x, int y) override;
		void mouseExited(int x, int y) override;
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg) override;
		
		ofxOscReceiver receiver;
		vector<string> messages;
		const int MAX_MESSAGES = 50; // Keep last 50 messages
		
		void takeSnapshot();

		ofPolyline baseLine;
		vector<ofPolyline> lineSnapshots;
		float rotationAngle = 0.0f; // Starting angle
		float totalRotationTime = 10.0f; // Total time for one full rotation
		float lastSnapshotTime = 0.0f; // Time of last snapshot
		float snapshotInterval = 0.05f; // Interval between snapshots
		float rotationRadius = 300.0f; // Radius size of the rotation
		float coneHeight = 100.0f;
		int lineSegments = 150;
		int samples = 35;
		
		ofEasyCam cam;

		ofMesh mesh;

		ofLight light;
		ofMaterial material;

		ofVec3f lightPos = ofVec3f(300, 300, 500);
		bool movingLight = false;
		bool showGui = true;

	private:
		ofxPanel gui;
		ofxIntSlider sampleSlider;
		ofxFloatSlider height;
		ofxIntSlider segments;
		ofxFloatSlider rotationTime;
		ofxFloatSlider snapshotIntervalSlider;

		// GUI parameters
		ofParameter<float> totalRotationTimeParam{"Rotation Time", 10.0f, 1.0f, 30.0f};
		ofParameter<float> snapshotIntervalParam{"Snapshot Interval", 0.05f, 0.01f, 0.5f};
		ofParameter<float> rotationRadiusParam{"Rotation Radius", 250.0f, 50.0f, 1000.0f};
		ofParameter<float> coneHeightParam{"Cone Height", 100.0f, 10.0f, 500.0f};
		ofParameter<int> lineSegmentsParam{"Line Segments", 150, 10, 300};
		
		void paramChanged(float& value);  // For float parameters
		void paramChanged(int& value);    // For int parameters
};
