#include <Arduino.h>
#include <LedControl.h>
#include <Light.h>
#include <pitches.h>
#include <MIDI.h>

#ifndef SYNTH_INCL_GUARD
#define SYNTH_INCL_GUARD

class Synth {
  private:
    LedControl* lc;
    Light* light;
    int voicePin;
    byte beat = 0;
    byte count = 0;
    void handleNoteOn(byte channel, byte pitch, byte velocity);
    void handleNoteOff(byte channel, byte pitch, byte velocity);
    void handleClock();
  public:
    Synth(LedControl* _lc, Light* _light, int voicePin);
    void setup();
    bool update();
    void sendNoteOn(byte channel, byte pitch, byte velocity);
    void sendNoteOff(byte channel, byte pitch, byte velocity);
};

#endif