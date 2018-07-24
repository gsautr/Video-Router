#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofLog::setAutoSpace(true);
    ofSetFrameRate(60);
    string folder;
#ifdef TARGET_LINUX_ARM
    folder = "/home/pi/";
#else
    folder = "/Users/Gilbert/Google Drive/-- Autr --/-- Videos --/__Instas";
#endif
    browser.setup({FLUID_BROWSER_VIDEO}, {folder});
    buffer.load(browser.getUnits());
    
    randomiseVideo();
    
    
    ofAddListener(buffer.videoPlayerEnded, this, &ofApp::videoPlayerEnded);
    
}

void ofApp::randomiseVideo() {
    
    buffer.pause(unit);
    ofxJSONElement units = browser.getUnits();
    index = ofRandom(0, units.size());
    unit = units[index];
    
    buffer.play(unit);
}

void ofApp::videoPlayerEnded(string & name) {
    randomiseVideo();
}

//--------------------------------------------------------------
void ofApp::update(){
    buffer.update(unit);
}

//--------------------------------------------------------------
void ofApp::draw(){
    buffer.draw(unit, 0, 0, ofGetWidth(), ofGetHeight());
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
