#include <Arduino.h>
#include <MIDI.h>
#include <Voice.h>

#ifndef SYNTH_INCL_GUARD
#define SYNTH_INCL_GUARD

class Synth {
  private:
    int voicePin;
    void handleNoteOn(byte channel, byte pitch, byte velocity);
    void handleNoteOff(byte channel, byte pitch, byte velocity);
  public:
    Synth(int voicePin);
    void setup();
    bool update();
    void sendNoteOn(byte channel, byte pitch, byte velocity);
    void sendNoteOff(byte channel, byte pitch, byte velocity);
};

#endif