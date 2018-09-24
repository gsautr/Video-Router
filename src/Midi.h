#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxFluidUi.h"

class Midi : public ofxMidiListener {
public:
    
    ofxMidiIn midiIn;
    std::vector<ofxMidiMessage> midiMessages;
    std::size_t maxMessages = 10; //< max number of messages to keep track of
    
    FluidUi * currentUiElement;
    
    bool bMidiLearn;
    
    std::map<int, FluidUi *> assignments;
    
    void setup() {
        
        // print input ports to console
        midiIn.listInPorts();
        
        // open port by number (you may need to change this)
        openPort(2);
        midiIn.addListener(this);
        midiIn.setVerbose(true);
        bMidiLearn = false;
    }
    bool isMidiLearning() {
        return bMidiLearn;
    }
    void setMidiLearn(bool b) {
        bMidiLearn = b;
    }
    void update() {
        
        for(unsigned int i = 0; i < midiMessages.size(); ++i) {
            
            ofxMidiMessage &message = midiMessages[i];
            
            if(message.status < MIDI_SYSEX) {
                
                if(message.status == MIDI_CONTROL_CHANGE) {
                    if (bMidiLearn) {
                        assignments[message.control] = currentUiElement;
                        bMidiLearn = false;
                    } else {
                        FluidUi * ele = assignments[message.control];
                        
                        if (ele == NULL) return;
                        
                        if ((ele->isSubType(FLUID_SLIDER))||(ele->isSubType(FLUID_KNOB))) {
                            float v = ofMap(message.value, 0, 127, ele->getMin(), ele->getMax());
                            ele->setValueWithSmoothing(v);
                        }
                        if (ele->isSubType(FLUID_AXISSLIDER)) {
                            float v = ofMap(message.value, 0, 127, ele->getMinX(), ele->getMaxX());
                            ele->setValueXWithSmoothing(v);
                        }
                        if ((ele->isSubType(FLUID_TOGGLE))) {
                            bool b = ofMap(message.value, 0, 127, 0, 1);
                            
                            ele->setState(!ele->getState());
                        }
                    }
//                    ofLog() << message.control << message.value;
                }
                
            }
            
        }
    }
    
    void setCurrentElement(FluidUi * u) {
        currentUiElement = u;
    }
    
    void openPort(int i ) {
        
        midiIn.openPort(2);
    }
    void newMidiMessage(ofxMidiMessage& msg) {
        
        // add the latest message to the message queue
        midiMessages.push_back(msg);
        
        // remove any old messages if we have too many
        while(midiMessages.size() > maxMessages) {
            midiMessages.erase(midiMessages.begin());
        }
    }
};

class MidiUi : public Midi {
public:
    FluidUi * ui;
    ofEventListener midiListener;
    FluidDropdown * portUi;
    void setup(FluidUi * u) {
        ui = u;
        Midi::setup();
        vector<string> ports =  midiIn.getInPortList();
        portUi = &ui->addDropdown("MIDI", ports);
        midiListener = portUi->indexUpdated.newListener([this](int & i){
            midiIn.openPort(i);
        });
        portUi->setIndex(0);
    }
    
};
