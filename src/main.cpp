#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    
    ofGLFWWindowSettings settings;
    settings.setSize(300, 800);
    settings.setPosition(ofVec2f(0,0));
    settings.multiMonitorFullScreen = false;
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
    
    settings.setSize(1000, 720);
    settings.setPosition(ofVec2f(300,0));
    settings.multiMonitorFullScreen = false;
    settings.resizable = true;
    settings.shareContextWith = guiWindow;
    shared_ptr<ofAppBaseWindow> imageWindow = ofCreateWindow(settings);
    imageWindow->setVerticalSync(false);
    
    shared_ptr<ofApp> mainApp(new ofApp);
    ofAddListener(imageWindow->events().draw, mainApp.get(), &ofApp::drawOutput);
    ofAddListener(imageWindow->events().exit, mainApp.get(), &ofApp::exitOutput);
    ofAddListener(imageWindow->events().keyPressed, mainApp.get(), &ofApp::keyPressedOutput);
    
    ofRunApp(guiWindow, mainApp);
    ofRunMainLoop();

}
