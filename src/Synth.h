#include <Arduino.h>
#include <SequenceStep.h>
#include <Expressions.h>
#include <Light.h>
#include <pitches.h>
#include <MIDI.h>

#ifndef SYNTH_INCL_GUARD
#define SYNTH_INCL_GUARD

#define PULSES_PER_SIXTEENTH_NOTE 6
#define PULSES_PER_QUARTER_NOTE 24
#define PULSES_PER_BAR 96

// How many notes can be pressed or active
#define MAX_NOTES 16
// Max number of discrete steps in a sequence
#define MAX_STEPS_IN_SEQ 8 * 16


class Synth {
  private:
    LedControl* lc;
    Expressions* expr;
    Light* light;
    int voicePin;

    byte pressedNotes[MAX_NOTES];
    byte numPressedNotes = 0;
    byte activeNotes[MAX_NOTES] = {62, 65, 69, 65};
    byte numActiveNotes = 4;

    byte currNote = -1;

    // 8 * 16 total 16th positons
    SequenceStep* sequenceSteps[MAX_STEPS_IN_SEQ];
    int totalSequenceSteps = 4;

    // Total sequence length in pulses
    uint16_t totalSequenceLength = 4 * PULSES_PER_QUARTER_NOTE;

    bool quarterFlipFlop = false;

    unsigned long long pulseCount = 0;
    void resetCounts();

    int findStepIndexForPulse(uint16_t pulse);
    void handleNoteOn(byte channel, byte pitch, byte velocity);
    void handleNoteOff(byte channel, byte pitch, byte velocity);
    void handleClock();
    void handleStartContinue(bool reset);
    void handleStop();
    void handleStep(int stepIndex);
    void generateSequence();
  public:
    Synth(LedControl* _lc, Expressions* _expr, Light* _light, int voicePin);
    void setup();
    bool update();
    void playButtonPress();
    void sendNoteOn(byte channel, byte pitch, byte velocity);
    void sendNoteOff(byte channel, byte pitch, byte velocity);
};

#endif