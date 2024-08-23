#include <Arduino.h>
#include <ButtonManager.h>
#include <Expressions.h>
#include <Grandbot.h>
#include <Light.h>
#include <MIDI.h>
#include <SettingManager.h>
#include <Stepper.h>
#include <pin_defs.h>
#include <pitches.h>

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
#define MAX_STEPS_IN_SEQ 8 * 16 * 2  // uncomment for #NANO_EVERY

// =====================
// MIDI CCs to listen to
// =====================

// TODO these probably need to move to SettingManager with the other CCs
// Special controls
#define CC_SLIP 116
#define CC_PANIC 117  // WARNING we listen to this CC regardless of channel
#define CC_GENERATE_SEQUENCE 118

class Arp {
 private:
  Grandbot* gb;
  SettingManager* settings;
  ButtonManager* buttons;
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
  // When using an internal clock, when we sent last clock pulse
  // TODO are we handling when this is 0 and micros is 0?
  unsigned long lastInternalClockPulseTime = 0;

  unsigned long lastMidiMessage;

  // Whether we should trigger a sequence regeneration
  bool regenerateQueued = false;
  // Whether we should trigger a sequence "slip"
  bool slipQueued = false;

  // Last time we saw a clock (to calculate swing delay time)
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
  byte activeNotes[MAX_NOTES] = {60, 62, 64, 65, 67, 69, 71, 72};
  // Number of notes that are currently active
  byte numActiveNotes = 8;

  // Note currently being played
  byte currNote = 0;

  // #TODO this is poorly named, it's not really an "interval"
  // it's an index for accessing a note from `activeNotes`
  // so it's a sequence of array indexes modulo'd by `numActiveNotes`
  byte sequenceIntervals[MAX_STEPS_IN_SEQ] = {1, 3, 5, 3};

  // Offset the note at this step (like for octaves)
  // 0=no offset; 12=+1 oct; -24=-2oct
  int8_t sequenceOffset[MAX_STEPS_IN_SEQ] = {0, 0, 0, 0};
  // Time (in clock pulses) that each step in the sequence starts
  uint16_t sequenceStartPositions[MAX_STEPS_IN_SEQ] = {
      // WARNING manually changing these values can introduce a silent bug
      // value sum must be >= totalSequenceLength
      PULSES_PER_QUARTER_NOTE * 0, PULSES_PER_QUARTER_NOTE * 1,
      PULSES_PER_QUARTER_NOTE * 2, PULSES_PER_QUARTER_NOTE * 3};

  // Total sequence length in discrete steps
  byte totalSequenceSteps = 4;
  // Total sequence length in pulses
  uint16_t totalSequenceLength = 4 * PULSES_PER_QUARTER_NOTE;

  // Basically: is this an odd or even quarter note?
  bool quarterFlipFlop = false;
  // How many clock pulses through the sequence are we
  unsigned long long pulseCount = 0;

  // Utilities
  byte ccPanic = 0;
  byte ccGenerate = 0;
  byte ccSlip = 0;

  void reset();
  byte ccRoll();
  int findStepIndexForPulse(uint16_t pulse);
  bool noteInBounds(byte note);
  void handleCommandChange(byte channel, byte cc, byte value);
  void handleNoteOn(byte channel, byte pitch, byte velocity);
  void handleNoteOff(byte channel, byte pitch, byte velocity);
  void handleClock(unsigned long now);
  void handleStartContinue(bool reset);
  void handleStop();
  void handleStep(int stepIndex);
  void handleButtons(bool useInternalClock);
  uint16_t addStep(byte stepIndex,
                   byte noteInterval,
                   int8_t noteOffset,
                   byte noteLength,
                   uint16_t startPosition);
  byte getNoteLength();
  byte getSequenceLength();
  void generateSequence();
  void slipSequence();
  String padded(String input);
  String convertCCToString(byte value);
  bool correctInChannel(byte channel);
  int insert(byte arr[], int arrLen, byte value, int capacity);
  void sort(byte arr[], int arrLen);
  void sendNoteOn(byte channel, byte pitch, byte velocity);
  void sendNoteOff(byte channel, byte pitch, byte velocity);
  byte ccToMidiCh(byte cc);
  void panic();

 public:
  Arp(Grandbot* gb);
  void setup();
  bool update();
};

#endif