#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

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
		int samples = 40;
		float rotationAngle = 0.0f; // Starting angle
		float totalRotationTime = 10.0f; // Total time for one full rotation
		float lastSnapshotTime = 0.0f; // Time of last snapshot
		float snapshotInterval = 0.15f; // Interval between snapshots
		float rotationRadius = 300.0f; // Radius size of the rotation
		
		ofEasyCam cam;
};
