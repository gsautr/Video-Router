#pragma once
#define FLUID_TOOL_BROWSER
#define FLUID_TOOL_BUFFER
#define FLUID_TOOL_SHADERS
#define FLUID_TOOL_OUTPUT

#include "ofMain.h"
#include "ofxFluidUi.h"
#include "ofxFluidTools.h"
#include "Midi.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void drawOutput(ofEventArgs & args);
    void exit(ofEventArgs & args);
    void exitOutput(ofEventArgs & args);

    void keyPressed(ofKeyEventArgs & args);
    void keyPressedOutput(ofKeyEventArgs & args);
    void keyReleased(ofKeyEventArgs & args);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofFbo fbo;
    ofTexture texture;
    
    ofEventListener lastInteractedChanged;
    
    FluidBrowserUi browser;
    FluidBuffer buffer;
    FluidOutput output;
    ofxJSONElement unit;
    FluidUi * ui;
    FluidGroup * sourceGroup;
    FluidShadersUi shaders;
    
    
    FluidBrowserUi browserB;
    FluidBuffer bufferB;
    FluidShadersUi shadersB;
    
    FluidSlider * alphaUi;
    FluidTabs * blendUi;
    
    MidiUi midi;
    
    int index;
    
    void videoPlayerEnded(string & name);
    void randomiseVideo();
    
    bool bTex;
		
    
    
};
