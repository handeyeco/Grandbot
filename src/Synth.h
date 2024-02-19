#include <Arduino.h>
#include <Expressions.h>
#include <Light.h>
#include <pitches.h>
#include <MIDI.h>

#ifndef SYNTH_INCL_GUARD
#define SYNTH_INCL_GUARD

#define MAX_NOTES 16

class Synth {
  private:
    LedControl* lc;
    Expressions* expr;
    Light* light;
    int voicePin;

    byte pressedNotes[MAX_NOTES];
    byte numPressedNotes = 0;

    byte currNoteIndex;
    int nextNotePosition;

    // 8 * 16 total 16th positons
    byte sequenceNotes[128];
    byte sequenceLengths[128];
    // number of discrete notes, NOT number of 16th notes
    int totalSequenceLength = 0;

    int pulseCount = 0;
    int sixteenthCount = 0;
    void handleSixteenth();
    int quarterCount = 0;
    void handleQuarter();
    void resetCounts();

    void handleNoteOn(byte channel, byte pitch, byte velocity);
    void handleNoteOff(byte channel, byte pitch, byte velocity);
    void handleClock();
    void handleStartContinue(bool reset);
    void handleStop();
    void generateSequence();
  public:
    Synth(LedControl* _lc, Expressions* _expr, Light* _light, int voicePin);
    void setup();
    bool update();
    void sendNoteOn(byte channel, byte pitch, byte velocity);
    void sendNoteOff(byte channel, byte pitch, byte velocity);
};

#endif