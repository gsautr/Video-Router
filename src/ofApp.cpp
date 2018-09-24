#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("Video Router");
    ofSetEscapeQuitsApp(false);
    ofLog::setAutoSpace(true);
    ofSetFrameRate(60);
    
    ofEnableAlphaBlending();
    
    
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
    
    midi.setup(ui);
    FluidGroup * row = &ui->addRow("Row");
    
    
    
    
    sourceGroup = &row->addGroup("A");
    
    browser.setup(sourceGroup, {FLUID_BROWSER_VIDEO, FLUID_BROWSER_SYPHON, FLUID_BROWSER_GRABBER}, {});
    browser.addSource("Harpham.mov");
    FluidGroup * shadersGroup = &sourceGroup->addGroup("Shaders");
    shaders.setup(shadersGroup);
    
//    shaders.loadISF("ISF/Luma Key");
    shaders.loadISF("ISF/Exposure Adjust");
    shaders.loadISF("ISF/Color Invert");
    shaders.loadISF("ISF/Zoom");
    shaders.loadISF("ISF/XYZoom");
    shaders.loadISF("ISF/Rotate");
    shaders.loadISF("ISF/Sharpen RGB");
//    shaders.loadISF("ISF/ZoomBlur");
//    shaders.loadISF("ISF/3d Rotate");
    shaders.loadISF("ISF/RGB EQ");
    shaders.loadISF("ISF/Shift Hue");
    
    
    FluidUi * bGroup = &row->addGroup("B");
    browserB.setup(bGroup, {FLUID_BROWSER_VIDEO, FLUID_BROWSER_SYPHON, FLUID_BROWSER_GRABBER}, {});
    browserB.addSource("Harpham.mov");
    
//    blendUi = &bGroup->addTabs("Blend", {"Add", "Multiply"});
    alphaUi = &bGroup->addSlider("Opacity", 0, 0, 255);
    
    FluidGroup * shadersBGroup = &bGroup->addGroup("Shaders");
    shadersB.setup(shadersBGroup);
    
//    shadersB.loadISF("ISF/Luma Key");
    shadersB.loadISF("ISF/Exposure Adjust");
    shadersB.loadISF("ISF/Luma");
    
    sourceGroup->setWidthMultiple(0.5);
    bGroup->setWidthMultiple(0.5);
    
    output.setup("Main Output");
//    processor.loadISF("ISF/Duotone");
    
    
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
    output.update();
    
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
    fbo.begin();
    ofEnableAlphaBlending();
    ofClear(0);
    ofClearAlpha();
    ofSetColor(255);
    shaders.draw(buffer);
    ofSetColor(255,255,255,alphaUi->getValue());
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    shadersB.draw(bufferB);
    fbo.end();
    
    output.draw(fbo);
    output.publish();
    
//    buffer.getTexture().draw(0,0, ofGetWidth(), ofGetHeight());
//    shaders.chain.draw(0,0, ofGetWidth() , ofGetHeight());
    fbo.draw(0,0, ofGetWidth(), ofGetHeight());
    
    if (!bTex) {
        
        browser.addTexture("Main Output");
        buffer.addTexture("Main Output", output.fbo.getTexture());
        bTex = true;
    }
    
    
//    ofLog() << "OUTP" << output.fbo.getWidth() << output.fbo.getHeight();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ui->draw(0,0, ofGetWidth(), ofGetHeight());
    
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

}
