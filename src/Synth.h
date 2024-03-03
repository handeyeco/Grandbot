#include <Arduino.h>
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
// 8 (bars) * 16th (notes)
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

    byte currNote = 0;

    byte sequenceIntervals[MAX_STEPS_IN_SEQ] = {0, 1, 2, 3};

    // Offset the note at this step (like for octaves)
    // 0=no offset; 12=+1 oct; -24=-2oct
    int8_t sequenceOffset[MAX_STEPS_IN_SEQ] = {0, 0, 0, 0};
    uint16_t sequenceStartPositions[MAX_STEPS_IN_SEQ] = {
      PULSES_PER_QUARTER_NOTE * 0,
      PULSES_PER_QUARTER_NOTE * 1,
      PULSES_PER_QUARTER_NOTE * 2,
      PULSES_PER_QUARTER_NOTE * 3
    };

    // Total sequence length in discrete steps
    byte totalSequenceSteps = 4;
    // Total sequence length in pulses
    uint16_t totalSequenceLength = 4 * PULSES_PER_QUARTER_NOTE;

    bool quarterFlipFlop = false;

    unsigned long long pulseCount = 0;
    void reset();

    int findStepIndexForPulse(uint16_t pulse);
    bool noteInBounds(byte note);
    void handleNoteOn(byte channel, byte pitch, byte velocity);
    void handleNoteOff(byte channel, byte pitch, byte velocity);
    void handleClock();
    void handleStartContinue(bool reset);
    void handleStop();
    void handleStep(int stepIndex);
    uint16_t addStep(byte stepIndex, byte noteInterval, int8_t noteOffset, byte noteLength, uint16_t startPosition);
    void generateSequence();
  public:
    Synth(Expressions* _expr, Light* _light, int voicePin);
    void setup();
    bool update();
    void playButtonPress();
    void sendNoteOn(byte channel, byte pitch, byte velocity);
    void sendNoteOff(byte channel, byte pitch, byte velocity);
};

#endif