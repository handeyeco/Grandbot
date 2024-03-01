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
    byte lastNoteBeforeSeqChange = -1;

    byte currNoteIndex = -1;
    int nextNotePosition = 0;

    // 8 * 16 total 16th positons
    byte sequenceNotes[128] = {62, 65, 69, 65, 62, 65, 69, 65};
    byte sequenceLengths[128] = {4, 4, 4, 4, 4, 4, 4, 4};
    // number of discrete notes, NOT number of 16th notes
    int totalSequenceLength = 8;

    unsigned long long pulseCount = 0;
    int sixteenthCount = 0;
    int quarterCount = 0;
    void handleSixteenth();
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