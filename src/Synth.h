#include <Arduino.h>
#include <MIDI.h>
#include <Voice.h>

#ifndef SYNTH_INCL_GUARD
#define SYNTH_INCL_GUARD

class Synth {
  private:
    Voice* voice;
    void handleNoteOn(byte channel, byte pitch, byte velocity);
    void handleNoteOff(byte channel, byte pitch, byte velocity);
  public:
    Synth(Voice* _voice);
    void setup();
    bool update();
};

#endif