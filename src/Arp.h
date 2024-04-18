#include <Arduino.h>
#include <Expressions.h>
#include <Light.h>
#include <pitches.h>
#include <MIDI.h>

#ifndef ARP_INCL_GUARD
#define ARP_INCL_GUARD

#define PULSES_PER_SIXTEENTH_NOTE 6
#define PULSES_PER_QUARTER_NOTE 24
#define PULSES_PER_BAR 96

// How many notes can be pressed or active
#define MAX_NOTES 16
// Max number of discrete steps in a sequence
// 8 (bars) * 16th (notes)
#define MAX_STEPS_IN_SEQ 8 * 16

// MIDI CCs to listen to
#define CC_CHANNEL_IN 14
#define CC_CHANNEL_OUT 15

#define CC_BASE_NOTE_LENGTH 20
#define CC_SEQUENCE_LENGTH 21
#define CC_OCTAVE_ONE_UP 22
#define CC_OCTAVE_ONE_DOWN 23
#define CC_OCTAVE_TWO_UP 24
#define CC_OCTAVE_TWO_DOWN 25
#define CC_DOUBLE_LENGTH 26
#define CC_RATCHET 27
#define CC_REST 28
#define CC_HALF_LENGTH 29
#define CC_RANDOM_LENGTH 30
#define CC_FIFTH_UP 31
#define CC_RANDOM_INTERVAL 85

#define CC_PANIC 117
#define CC_GENERATE_SEQUENCE 118
#define CC_USE_SPEAKER 119

class Arp {
  private:
    LedControl* lc;
    Expressions* expr;
    Light* light;
    int voicePin;
    bool regenerateQueued = false;

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

    // MIDI channels; 0-15
    // 255 = do whatever the original MIDI message wanted
    byte midiChannelIn = 255;
    byte midiChannelOut = 255;

    // CC controlled params; all need to be 0-127
    byte ccBaseNoteLength = 0;
    byte ccSequenceLength = 0;
    byte ccOctaveOneUpChance = 0;
    byte ccOctaveOneDownChance = 0;
    byte ccOctaveTwoUpChance = 0;
    byte ccOctaveTwoDownChance = 0;
    byte ccFifthChance = 0;
    byte ccRandomIntervalChance = 0;
    byte ccRandomLengthChance = 0;
    byte ccHalfLengthChance = 0;
    byte ccDoubleLengthChance = 0;
    byte ccRatchetChance = 0;
    byte ccRestChance = 0;
    byte ccUseSpeaker = 0;
    byte ccPanic = 0;
    byte ccGenerate = 0;

    bool quarterFlipFlop = false;
    unsigned long long pulseCount = 0;
    void reset();

    int findStepIndexForPulse(uint16_t pulse);
    bool noteInBounds(byte note);
    void handleControlCommand(byte channel, byte cc, byte value);
    void handleControlChange(byte channel, byte cc, byte value);
    void handleMidiChannelChange(byte channel, byte cc, byte value);
    void handleNoteOn(byte channel, byte pitch, byte velocity);
    void handleNoteOff(byte channel, byte pitch, byte velocity);
    void handleClock();
    void handleStartContinue(bool reset);
    void handleStop();
    void handleStep(int stepIndex);
    uint16_t addStep(byte stepIndex, byte noteInterval, int8_t noteOffset, byte noteLength, uint16_t startPosition);
    byte getNoteLength();
    byte getSequenceLength();
    void generateSequence();
    String padded(String input);
    bool convertCCToBool(byte value);
    String convertCCToString(byte value);
    bool correctInChannel(byte channel);
  public:
    Arp(Expressions* _expr, Light* _light, int voicePin);
    void setup();
    bool update();
    void playButtonPress();
    void sendNoteOn(byte channel, byte pitch, byte velocity);
    void sendNoteOff(byte channel, byte pitch, byte velocity);
};

#endif