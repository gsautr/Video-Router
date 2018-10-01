#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("Video Router");
    ofSetEscapeQuitsApp(false);
    ofLog::setAutoSpace(true);
    ofSetFrameRate(60);
    
    ofEnableAlphaBlending();
    
    fboIndex = 0;
    
    
#ifdef TARGET_OSX
    ofSetDataPathRoot("../Resources/data/");
#endif
    string folder;
#ifdef TARGET_LINUX_ARM
    folder = "/home/pi/";
#else
    folder = "/Users/Gilbert/Google Drive/-- Autr --/-- Videos --/__Instas";
#endif
    
    bTex = false;
    
    ui = new FluidUi();
    ui->setup();
//    ui->setSmoothing(0.8);
    
    FluidGroup * row = &ui->addRow("Row");
    
    
    
    
    sourceGroup = &row->addGroup("A");
    
    browser.setup(sourceGroup, {FLUID_BROWSER_VIDEO, FLUID_BROWSER_SYPHON, FLUID_BROWSER_GRABBER}, {});
    browser.addDir("videos");
    FluidGroup * shadersGroup = &sourceGroup->addGroup("Shaders");
    shaders.setup(shadersGroup);
    
//    shaders.loadISF("ISF/Luma Key");
    shaders.loadISF("ISF/Bright");
    shaders.loadISF("ISF/Color Controls");
    shaders.loadISF("ISF/Color Invert");
    shaders.loadISF("ISF/Zoom");
//    shaders.loadISF("ISF/XYZoom");
//    shaders.loadISF("ISF/Rotate");
//    shaders.loadISF("ISF/Sharpen RGB");
////    shaders.loadISF("ISF/ZoomBlur");
////    shaders.loadISF("ISF/3d Rotate");
//    shaders.loadISF("ISF/RGB EQ");
//    shaders.loadISF("ISF/Shift Hue");
    
    
    FluidUi * bGroup = &row->addGroup("B");
    browserB.setup(bGroup, {FLUID_BROWSER_VIDEO, FLUID_BROWSER_SYPHON, FLUID_BROWSER_GRABBER}, {});
    browserB.addDir("videos");
    
    
    
    FluidGroup * shadersBGroup = &bGroup->addGroup("Shaders");
    shadersB.setup(shadersBGroup);
    
//    shadersB.loadISF("ISF/Luma Key");
    shadersB.loadISF("ISF/Exposure Adjust");
    shadersB.loadISF("ISF/Luma");
    
    sourceGroup->setWidthMultiple(0.5);
    bGroup->setWidthMultiple(0.5);
    
    output.setup("Main Output");
//    processor.loadISF("ISF/Duotone");
    
    blender.setup(ui);
    blender.addSource(shaders.fbo, 1, 0);
    blender.addSource(shadersB.fbo, 0, 0);

    midi.setup(ui);
    
    lastInteractedChanged = ui->lastInteractedUpdated.newListener([this](string & s){
        midi.setCurrentElement(ui->getElementById(s));
    });
    
    buffer.player.setVolume(0);
    bufferB.player.setVolume(0);
    
}


void ofApp::randomiseVideo() {
    buffer.pause(unit);
    ofxJSONElement units = browser.getUnits();
    index = ofRandom(0, units.size());
    unit = units[index];
    buffer.play(unit);
}

void ofApp::videoPlayerEnded(string & name) {
//    randomiseVideo();
}

//--------------------------------------------------------------
void ofApp::update(){
    midi.update();
    shaders.update();
    buffer.update(browser.getCurrent());
    shadersB.update();
    bufferB.update(browserB.getCurrent());
    ui->update();
//    output.update();
    
}

//--------------------------------------------------------------
void ofApp::drawOutput(ofEventArgs & args){
//    ofLog() << "BUFF" << buffer.getWidth() << buffer.getHeight();

    int ww = shaders.getWidth();
    int hh = shaders.getHeight();
    
//    if ((ww != 0)&&(hh != 0)) {
    
//    ofLog() << "A" << ww << hh;
    if (((fbo.getWidth() != ww)||(fbo.getHeight() != hh))) {
        fbo.allocate(ww, hh, GL_RGBA);
    }
        
//    }
    
    shaders.drawFbo(buffer);
    shadersB.drawFbo(bufferB);
    
    blender.draw(0,0,ofGetWidth(), ofGetHeight());
//    shaders.fbo.draw(0,0,ofGetWidth(), ofGetHeight());
    
    output.publish(blender.fbo.getTexture());
    
    if (!bTex) {
        
        browser.addTexture("Main Output");
        buffer.addTexture("Main Output", blender.fbo.getTexture());
        bTex = true;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ui->draw(0,0, ofGetWidth(), ofGetHeight());
//    ofSetColor(255,0,0,100);
//    ofDrawRectangle(ui->all[fboIndex]->getBackgroundArea());
//    ofSetColor(255);
//    ofDrawBitmapString(midi.lastMessage, 10, ofGetHeight() - 60);
    if (midi.isMidiLearning()) {
        
        
        FluidBase * ele = ui->getLastInteracted();
        ofNoFill();
        ofSetColor(255,0,0);
        ofSetLineWidth(1);
        if (ele != NULL) {
            ofRectangle r = ele->getElementArea();
            if (ele->isScrollableInParent()) {
                r.y += ele->getScrollParent()->getScrollY();
            }
            ofDrawRectangle(r);
        }
        ofSetLineWidth(4);
        
        ofDrawRectangle(0,0, ofGetWidth(), ofGetHeight());
        ofFill();
    }
}

void ofApp::exit(ofEventArgs & args) {
    ofLog() << "Exit";
    ofExit();
}

void ofApp::exitOutput(ofEventArgs & args) {
    
    ofLog() << "Exit Output";
    ofExit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(ofKeyEventArgs & args){
}
//--------------------------------------------------------------
void ofApp::keyPressedOutput(ofKeyEventArgs & args){
    if (args.key == 'f') ofToggleFullscreen();
    if (args.key == 'w') fboIndex += 1;
    if (args.key == 'q') fboIndex -= 1;
}

//--------------------------------------------------------------
void ofApp::keyReleased(ofKeyEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    if (button != 0) {
        
        midi.setMidiLearn(true);
    } else {
        
        midi.setMidiLearn(false);
    }
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
    if (shaders.getBounds().inside(dragInfo.position)) shaders.loadFiles(dragInfo.files);
    if (shadersB.getBounds().inside(dragInfo.position)) shadersB.loadFiles(dragInfo.files);
    ofRectangle rLeft(0, 0, ofGetWidth()/2, ofGetHeight());
    ofRectangle rRight = rLeft;
    rRight.x += ofGetWidth() / 2;
    
    if (rLeft.inside(dragInfo.position)) browser.addSources(dragInfo.files);
    if (rRight.inside(dragInfo.position)) browserB.addSources(dragInfo.files);
    
}
