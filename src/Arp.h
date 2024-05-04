#include <Arduino.h>
#include <pin_defs.h>
#include <Expressions.h>
#include <Light.h>
#include <pitches.h>
#include <MIDI.h>
#include <Grandbot.h>

#ifndef ARP_INCL_GUARD
#define ARP_INCL_GUARD

// For debugging, set to false
#define INITIALIZE_ON_START false

#define PULSES_PER_SIXTEENTH_NOTE 6
#define PULSES_PER_EIGHTH_NOTE 12
#define PULSES_PER_QUARTER_NOTE 24
#define PULSES_PER_BAR 96

// How many notes can be pressed or active
#define MAX_NOTES 16

/*
 * MAX_STEPS_IN_SEQ
 * Max number of discrete steps in a sequence.
 * Has a big impact on RAM. Ideally we'd like to be able
 * to store 8 bars of 32nd notes (100% ratcheting on 16th),
 * but the original Nano can't hold all of that.
 * Luckily the Nano Every can!
*/
// 8 (bars) * 16th (notes)
// #define MAX_STEPS_IN_SEQ 8 * 16 // uncomment for #NANO
// 8 (bars) * 32nd (notes)
#define MAX_STEPS_IN_SEQ 8 * 16 * 2 // uncomment for #NANO_EVERY

// =====================
// MIDI CCs to listen to
// =====================

// Free range 14-15
// MIDI I/O settings
#define CC_CHANNEL_IN 14 // WARNING we listen to this CC regardless of channel
#define CC_CHANNEL_OUT 15

// Free range 20-31
// Regular sequence params
#define CC_BASE_NOTE_LENGTH 20
#define CC_SEQUENCE_LENGTH 21
#define CC_OCTAVE_ONE_UP 22
#define CC_OCTAVE_ONE_DOWN 23
#define CC_OCTAVE_TWO_UP 24
#define CC_OCTAVE_TWO_DOWN 25
#define CC_DOUBLE_LENGTH 26
#define CC_HALF_LENGTH 27
#define CC_RATCHET 28
#define CC_REST 29

// Free range 85-87
// The more chaotic ones
#define CC_FIFTH_UP 85
#define CC_RANDOM_INTERVAL 86
#define CC_RANDOM_LENGTH 87

// Free range 89-90
#define CC_SLIP_CHANCE 89

// Free range 102-119
// Special controls
#define CC_SWING 115
#define CC_SLIP 116
#define CC_PANIC 117 // WARNING we listen to this CC regardless of channel
#define CC_GENERATE_SEQUENCE 118
#define CC_USE_SPEAKER 119

class Arp {
  private:
    // 4D7S display manager
    Expressions* expr;
    // RGB LED manager
    Light* light;

    // Track whether we're in "MIDI mode"
    // ie we've received a MIDI message recently
    bool midiMode = false;
    // Track Start/Stop/Continue messages
    // #TODO could this replace midiMode?
    bool running = false;
    unsigned long lastMidiMessage;

    // Whether we should trigger a sequence regeneration
    bool regenerateQueued = false;
    // Whether we should trigger a sequence "slip"
    bool slipQueued = false;

    // Last time we saw a clock (to determine BPM)
    unsigned long lastClockPulse = 0;
    // When a note that will swing should have played without swing
    unsigned long originalSwungNoteTime = 0;
    // How long to wait before playing the swung note
    unsigned long swingDelay = 0;
    // Which step was getting swung
    int swungStepIndex = -1;

    // MIDI notes that are currently pressed
    byte pressedNotes[MAX_NOTES];
    // Number of notes that are currently pressed
    byte numPressedNotes = 0;
    // MIDI notes that are currently active
    byte activeNotes[MAX_NOTES] = {62, 65, 69, 65};
    // Number of notes that are currently active
    byte numActiveNotes = 4;

    // Note currently being played
    byte currNote = 0;

    // #TODO this is poorly named, it's not really an "interval"
    // it's an index for accessing a note from `activeNotes`
    // so it's a sequence of array indexes modulo'd by `numActiveNotes`
    byte sequenceIntervals[MAX_STEPS_IN_SEQ] = {0, 1, 2, 3};

    // Offset the note at this step (like for octaves)
    // 0=no offset; 12=+1 oct; -24=-2oct
    int8_t sequenceOffset[MAX_STEPS_IN_SEQ] = {0, 0, 0, 0};
    // Time (in clock pulses) that each step in the sequence starts
    uint16_t sequenceStartPositions[MAX_STEPS_IN_SEQ] = {
      PULSES_PER_SIXTEENTH_NOTE * 0,
      PULSES_PER_SIXTEENTH_NOTE * 1,
      PULSES_PER_SIXTEENTH_NOTE * 2,
      PULSES_PER_SIXTEENTH_NOTE * 3
    };

    // Total sequence length in discrete steps
    byte totalSequenceSteps = 4;
    // Total sequence length in pulses
    uint16_t totalSequenceLength = 4 * PULSES_PER_SIXTEENTH_NOTE;

    // Basically: is this an odd or even quarter note?
    bool quarterFlipFlop = false;
    // How many clock pulses through the sequence are we
    unsigned long long pulseCount = 0;

    // MIDI channels; 0-15
    // 255 = do whatever the original MIDI message wanted
    byte midiChannelIn = 255;
    byte midiChannelOut = 255;

    // CC controlled params; all need to be 0-127
    byte ccBaseNoteLength = 0;
    byte ccSequenceLength = 0;
    byte ccOctaveOneUpChance = 10;
    byte ccOctaveOneDownChance = 10;
    byte ccOctaveTwoUpChance = 5;
    byte ccOctaveTwoDownChance = 5;
    byte ccHalfLengthChance = 0;
    byte ccDoubleLengthChance = 5;
    byte ccRatchetChance = 10;
    byte ccRestChance = 5;

    // The chaotic ones
    byte ccFifthChance = 0;
    byte ccRandomIntervalChance = 0;
    byte ccRandomLengthChance = 0;

    // Utilities
    byte ccSwing = 0;
    byte ccUseSpeaker = 0;
    byte ccPanic = 0;
    byte ccGenerate = 0;
    byte ccSlipChance = 10;
    byte ccSlip = 0;

    void reset();
    byte ccRoll();
    int findStepIndexForPulse(uint16_t pulse);
    bool noteInBounds(byte note);
    void handleCommandChange(byte channel, byte cc, byte value);
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
    void slipSequence();
    String padded(String input);
    bool convertCCToBool(byte value);
    String convertCCToString(byte value);
    bool correctInChannel(byte channel);
    void sendNoteOn(byte channel, byte pitch, byte velocity);
    void sendNoteOff(byte channel, byte pitch, byte velocity);
  public:
    Arp(Grandbot* gb);
    void setup();
    bool update(bool buttonPressed);
};

#endif