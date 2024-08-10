#include <Arp.h>

// Initialize MIDI library
// MIDI_CREATE_DEFAULT_INSTANCE(); // uncomment for #NANO
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI); // uncomment for #NANO_EVERY

Arp::Arp(Grandbot* gb) {
  this->buttons = gb->getButtonManagerPointer();
  this->expr = gb->getExpressionPointer();
  this->light = gb->getLightPointer();
  this->settings = new SettingManager(expr, buttons);

  if (INITIALIZE_ON_START) {
    generateSequence();
  }
}

const byte possibleNoteLengths[] = {
  // used in random note selection
  1, // 16th
  2, // 8th
  4, // quarter
  // not used in random note selection
  8, // half
  16, // whole
  32 // double whole note
};

/**
 * Add a step to the sequence
 *
 * @param {byte} stepIndex - discrete step in the sequence
 * @param {byte} noteInterval - `activeNotes` index
 * @param {int8_t} noteOffset - how much to transpose note (ie octaves)
 * @param {byte} noteLength - how long the step is
 * @param {uint16_t} startPosition - where the step starts (in pulses)
 * @returns {uint16_t} where the step ends (in pulses)
*/
uint16_t Arp::addStep(
  byte stepIndex,
  byte noteInterval,
  int8_t noteOffset,
  byte noteLength,
  uint16_t startPosition
) {
    sequenceIntervals[stepIndex] = noteInterval;
    sequenceOffset[stepIndex] = noteOffset;
    sequenceStartPositions[stepIndex] = startPosition;

    return startPosition + noteLength;
}

/**
 * Dice roll for MIDI CC values
 *
 * @returns {byte} random number between 0-127
*/
byte Arp::ccRoll() {
  return random(128);
}

/**
 * Maps ccBaseNoteLength (0-127) to note lengths
 *
 * @returns {byte} number of 16ths for note
*/
byte Arp::getNoteLength() {
  byte v = settings->baseNoteLength->getValue();
  if (v > 108) {
    // double whole
    return 32;
  }
  else if (v > 90) {
    // whole
    return 16;
  }
  else if (v > 72) {
    // half
    return 8;
  }
  else if (v > 54) {
    // quarter
    return 4;
  }
  else if (v > 36) {
    // 8th
    return 2;
  }
  else if (v > 18) {
    // 16th
    return 1;
  }
  else {
    // random; only choose between
    // 16th, 8th, and quarter
    return possibleNoteLengths[random(3)];
  }
}

/**
 * Maps ccSequenceLength (0-127) to sequence lengths
 *
 * @returns {byte} number between 1-8
*/
byte Arp::getSequenceLength() {
  // Random, 1-8
  if (ccSequenceLength < 15) {
    return random(1, 9);
  } else {
    return map(ccSequenceLength, 15, 127, 1, 8);
  }
}

/**
 * Slip sequence runs through the existing sequence and
 * randomly switches steps (randomness based on ccSlipChance)
*/
void Arp::slipSequence() {
  if (ccSlipChance == 0) return;

  for (byte i = 0; i < totalSequenceSteps - 1; i++) {
    if (ccRoll() < ccSlipChance) {
      byte tmpInterval = sequenceIntervals[i+1];
      int8_t tmpOffset = sequenceOffset[i+1];
      sequenceIntervals[i+1] = sequenceIntervals[i];
      sequenceOffset[i+1] = sequenceOffset[i];
      sequenceIntervals[i] = tmpInterval;
      sequenceOffset[i] = tmpOffset;
    }
  }
}

/**
 * Generate a new sequence
*/
void Arp::generateSequence() {
  // This is the length in number of pulses
  // (random number of bars between 1-8)
  uint16_t newTotalSequenceLength = getSequenceLength() * PULSES_PER_BAR;

  // This is the base length of the note in pulses
  byte baseNoteLength = getNoteLength() * PULSES_PER_SIXTEENTH_NOTE;

  // Accumulator for steps in pulses
  uint16_t newSequenceLength = 0;

  // Accumulator for discrete steps
  byte stepIndex = 0;

  // Try to fill the sequence length (pulses)
  // while keeping within max number of discrete steps.
  // For long sequences of 16th notes with a lot of ratchets or half steps
  // this could result in number of steps not filling the sequence
  // (which makes the end of the sequence one long note)
  // #TODO handle this better
  while (newSequenceLength < newTotalSequenceLength && stepIndex < MAX_STEPS_IN_SEQ) {
    // Due to adding note length variation
    // this particular step might not stay the same
    // length as other steps
    byte noteLength = baseNoteLength;
    // used for transposition variation
    int8_t noteOffset = 0;

    // which index in the activeNotes array to play
    // (gets wrapped by numActiveNotes)
    // #TODO this name is confusing
    byte randomNoteInterval = random(MAX_NOTES);

    // Random transposition; since they affect the same value (offset)
    // there's some interplay between them with a bias
    // towards single octave intervals
    if (
      settings->octaveOneUpChance->getValue() ||
      settings->octaveOneDownChance->getValue() ||
      ccOctaveTwoUpChance ||
      ccOctaveTwoDownChance ||
      ccFifthChance ||
      ccRandomIntervalChance
    ) {
      if (ccRoll() < settings->octaveOneUpChance->getValue()) {
          // one oct up
          noteOffset = 12;
      } else if (ccRoll() < settings->octaveOneDownChance->getValue()) {
          // one oct down
          noteOffset = -12;
      } else if (ccRoll() < ccOctaveTwoUpChance) {
          // two oct up
          noteOffset = 24;
      } else if (ccRoll() < ccOctaveTwoDownChance) {
          // two oct down
          noteOffset = -24;
      } else if (ccRoll() < ccFifthChance) {
          // fifth up
          noteOffset = 7;
      } else if (ccRoll() < ccRandomIntervalChance) {
          // chaos between two octaves (exclusive)
          noteOffset = random(-11, 12);
      }
    }

    // Random note length; this is different than
    // random base note length (affects all notes)
    // this is random for a single step
    if (ccRandomLengthChance) {
      if (ccRoll() < ccRandomLengthChance) {
        // only chose between 16th, 8th, and quarter during random selection
        noteLength = possibleNoteLengths[random(3)] * PULSES_PER_SIXTEENTH_NOTE;
      }
    }

    // Note length variation; since they affect the same value (length)
    // there's some interplay between them with a bias
    // towards ratchets
    if (ccRatchetChance || ccHalfLengthChance || ccDoubleLengthChance || ccRunChance) {
      if (ccRoll() < ccRatchetChance) {
        noteLength = noteLength / 2;
        newSequenceLength = addStep(
          stepIndex,
          randomNoteInterval,
          noteOffset,
          noteLength,
          newSequenceLength
        );
        stepIndex++;
      }
      else if ((stepIndex + 4 < MAX_STEPS_IN_SEQ) && (ccRoll() < ccRunChance)) {
        for (int i = 0; i < 4; i++) {
          newSequenceLength = addStep(
            stepIndex,
            i % MAX_NOTES,
            noteOffset,
            PULSES_PER_SIXTEENTH_NOTE / 2,
            newSequenceLength
          );
          stepIndex++;
        }
      }
      else if (ccRoll() < ccHalfLengthChance) {
        noteLength = noteLength / 2;
      }
      else if (ccRoll() < ccDoubleLengthChance) {
        noteLength = noteLength * 2;
      }
    }

    if (ccRestChance) {
      // Random rest
      if (ccRoll() < ccRestChance) {
        // use 255 to indicate rest
        // TODO fix this, it's hacky
        randomNoteInterval = 255;
      }
    }

    // take all this variation and add a step to the sequence
    newSequenceLength = addStep(
      stepIndex,
      randomNoteInterval,
      noteOffset,
      noteLength,
      newSequenceLength
    );
    stepIndex++;
  }

  totalSequenceSteps = stepIndex;
  totalSequenceLength = newTotalSequenceLength;
}

/**
 * Sends a note on MIDI message and optionally
 * sends note to the buzzer
 * #TODO do we need velocity?
 *
 * @param {byte} channel - MIDI channel to send to
 * @param {byte} note - MIDI note to send
 * @param {byte} velocity - MIDI velocity to send
*/
void Arp::sendNoteOn(byte channel, byte note, byte velocity) {
  // if midiChannelOut is 255, we use whatever channel was provided,
  // otherwise use the channel set in midiChannelOut
  byte outCh = ccToMidiCh(settings->midiChannelOut->getValue());
  byte movedChannel = outCh == 255 ? channel : outCh;
  MIDI.sendNoteOn(note, velocity, movedChannel);

  // if the speaker is set to be on, play note on buzzer
  if (convertCCToBool(settings->useSpeaker->getValue())) {
    tone(BUZZER_PIN, getPitchByNote(note));
  }
}

/**
 * Sends a note off MIDI message and quiets the buzzer
 * #TODO do we need velocity?
 *
 * @param {byte} channel - MIDI channel to send to
 * @param {byte} note - MIDI note to send
 * @param {byte} velocity - MIDI velocity to send
*/
void Arp::sendNoteOff(byte channel, byte note, byte velocity) {
  // if midiChannelOut is 255, we use whatever channel was provided,
  // otherwise use the channel set in midiChannelOut
  byte outCh = ccToMidiCh(settings->midiChannelOut->getValue());
  byte movedChannel = outCh == 255 ? channel : outCh;
  MIDI.sendNoteOff(note, velocity, movedChannel);
  noTone(BUZZER_PIN);
}

byte Arp::ccToMidiCh(byte cc) {
  if (cc == 255) return cc;

  return map(cc, 0, 127, 0, 16);
}

int Arp::insert(byte arr[], int arrLen, byte value, int capacity) { 
    // cap at capacity
    if (arrLen >= capacity) {
      return arrLen;
    }

    // deduplicate
    for (int i = 0; i < arrLen; i++) {
      if (arr[i] == value) {
        return arrLen;
      }
    }
  
    arr[arrLen] = value;
    return arrLen + 1; 
} 


void Arp::sort(byte arr[], int arrLen) {
  for (int i = 1; i < arrLen; i++) {
    for (int j = i; j > 0 && arr[j-1] > arr[j]; j--) {
      byte tmp = arr[j-1];
      arr[j-1] = arr[j];
      arr[j] = tmp;
    }
  }
}

/**
 * Handle a MIDI note on message
 *
 * @param {byte} channel - MIDI channel received on
 * @param {byte} note - MIDI note received
 * @param {byte} velocity - MIDI velocity received
*/
void Arp::handleNoteOn(byte channel, byte note, byte velocity) {
  // if we're not listening on this channel, ignore message
  if (!correctInChannel(channel)) {
    return;
  }

  // sometimes things send note on with a velocity of 0
  // to mean note off
  if (velocity == 0) {
    handleNoteOff(channel, note, velocity);
    return;
  }

  // reset active notes when going from 0 to 1 pressed note
  if (numPressedNotes == 0) {
    numActiveNotes = 0;
  }

  // exit if we're maxed out on space for new notes
  if (numPressedNotes + 1 > MAX_NOTES || numActiveNotes + 1 > MAX_NOTES) {
    return;
  }

  numPressedNotes = insert(pressedNotes, numPressedNotes, note, MAX_NOTES);
  numActiveNotes = insert(activeNotes, numActiveNotes, note, MAX_NOTES);

  if (convertCCToBool(settings->sort->getValue())) {
    sort(pressedNotes, numPressedNotes);
    sort(activeNotes, numActiveNotes);
  }
}

/**
 * Handle a MIDI note off message
 *
 * @param {byte} channel - MIDI channel received on
 * @param {byte} note - MIDI note received
 * @param {byte} velocity - MIDI velocity received
*/
void Arp::handleNoteOff(byte channel, byte note, byte velocity) {
  // if we're not listening on this channel, ignore message
  if (!correctInChannel(channel)) {
    return;
  }

  // remove note from pressed note array, not from active note array
  // (active note array only resets when going from 0 to 1 pressed notes)
  byte noteIndex = -1;
  for (byte i = 0; i < numPressedNotes; i++) {
    if (pressedNotes[i] == note) {
      noteIndex = i;
    }
    
    // MAX_NOTES - 2 because at index 14 we can move index 15 over
    // but at index 15 there's nothing to the right to move over
    if (noteIndex >= 0 && i >= noteIndex && i < MAX_NOTES - 2) {
      pressedNotes[i] = pressedNotes[i+1];
    }
  }

  if (noteIndex > -1 && numPressedNotes > 0) {
    --numPressedNotes;
  }
}

/**
 * Find step index for a given MIDI clock pulse
 *
 * @param {uint16_t} pulse - clock pulse step should be on
 * @returns {int} step index (or -1 if not found)
*/
int Arp::findStepIndexForPulse(uint16_t pulse) {
  for (int i = 0; i < totalSequenceSteps; ++i) {
    if (pulse == sequenceStartPositions[i]) {
      return i;
    }

    // steps are in sequential order, so if we haven't
    // found the right step we aren't going to find it
    if (pulse < sequenceStartPositions[i]) {
      return -1;
    }
  }

  return -1;
}

/**
 * Is note within MIDI note range
 *
 * @param {byte} note - MIDI note to check
 * @returns {bool} if note is between 23 (B0) and 110 (D8)
*/
bool Arp::noteInBounds(byte note) {
  return note >= 23 && note <= 110;
}

/**
 * Pads a string with a leading space if it's under 2 chars long
 *
 * @param {String} input - unpadded string
 * @returns {String} padded string
*/
String Arp::padded(String input) {
  int len = input.length();
  if (len < 2) {
    String padded = " ";
    padded.concat(input);
    return padded;
  }
  return input;
}

/**
 * Converts a MIDI CC value (0-127) to a boolean
 *
 * @param {byte} value - CC value
 * @returns {boolean} if CC is above threshold
*/
bool Arp::convertCCToBool(byte value) {
  return value > 64;
}

/**
 * Converts a MIDI CC value (0-127) to a 0-99 string
 *
 * @param {byte} value - CC value
 * @returns {String} padded and mapped string representation
*/
String Arp::convertCCToString(byte value) {
  byte mappedValue = map(value, 0, 127, 0, 99);
  String valueStr = String(mappedValue);
  return padded(valueStr);
}

/**
 * Checks if we care about a specific MIDI channel
 *
 * @param {byte} channel - MIDI channel in question
 * @returns {bool} if we care about that channel
*/
bool Arp::correctInChannel(byte channel) {
  // 255 is the magic number to represent "all channels are okay"
  // otherwise we only care about the channel set in midiChannelIn
  byte inCh = ccToMidiCh(settings->midiChannelIn->getValue());
  if (inCh == 255 || channel == inCh) {
    return true;
  }

  return false;
};

/**
 * Handle changing MIDI CC values for "command" signals
 *
 * NOTE: this mostly ignores messages that are on the wrong channel,
 * except for Panic signals
 * - #TODO move panic to AllNotesOff CC
 * - #TODO probably could be merged with handleControlChange
 *
 * @param {byte} channel - MIDI channel received on
 * @param {byte} cc - MIDI CC received
 * @param {byte} value - MIDI value received
*/
void Arp::handleCommandChange(byte channel, byte cc, byte value) {
  byte ccDisplay[2];
  char valDisplay[2];
  bool isOn = convertCCToBool(value);

  // try to shut it all down
  // (no matter what channel we get this message on
  // we'll trigger a panic.
  // @HACK will likely lead to weird things)
  if (cc == CC_PANIC) {
    bool wasOff = !convertCCToBool(ccPanic);
    ccPanic = value;
    if (wasOff && isOn) {
      noTone(BUZZER_PIN);

      // I don't know why I did it this way
      ccDisplay[0] = CHAR_A;
      ccDisplay[1] = CHAR_H;
      valDisplay[0] = 'h';
      valDisplay[1] = 'h';
      expr->control(ccDisplay, valDisplay);

      for (byte ch = 0; ch < 16; ch++) {
        for (byte note = 0; note < 128; note++) {
          MIDI.sendNoteOff(note, 64, ch+1);
        }
      }
    }
  }

  if (!correctInChannel(channel)) return;

  // Queue a new sequence
  if (cc == CC_GENERATE_SEQUENCE) {
    bool wasOff = !convertCCToBool(ccGenerate);
    ccGenerate = value;
    if (wasOff && isOn) {
      regenerateQueued = true;
    }
  }
  // Queue a sequence slip
  else if (cc == CC_SLIP) {
    bool wasOff = !convertCCToBool(ccSlip);
    ccSlip = value;
    if (wasOff && isOn) {
      slipQueued = true;
    }
  }
  // Note swing
  else if (cc == CC_SWING) {
    ccSwing = value;
    ccDisplay[0] = CHAR_S;
    ccDisplay[1] = CHAR_G;
    // Swing is between 50%-67%
    // 50%, 16th is halfway between 8ths
    // 67%, 16th is 2/3 between 8ths
    byte mapped = map(value, 0, 127, 50, 67);
    String valueStr = String(mapped);
    valDisplay[0] = valueStr[0];
    valDisplay[1] = valueStr[1];
    expr->control(ccDisplay, valDisplay);
  }
}

/**
 * Handle changing MIDI CC values for "control" signals
 *
 * @param {byte} channel - MIDI channel received on
 * @param {byte} cc - MIDI CC received
 * @param {byte} value - MIDI value received
*/
void Arp::handleControlChange(byte channel, byte cc, byte value) {
  String valueStr = convertCCToString(value);
  byte ccDisplay[2];
  char valDisplay[2] = {valueStr[0], valueStr[1]};

  // Chance a step will be transposed two octaves up
  if (cc == CC_OCTAVE_TWO_UP) {
    ccOctaveTwoUpChance = value;
    ccDisplay[0] = B01100011;
    ccDisplay[1] = B01000001;
  }
  // Chance a step will be transposed two octaves down
  else if (cc == CC_OCTAVE_TWO_DOWN) {
    ccOctaveTwoDownChance = value;
    ccDisplay[0] = B00011101;
    ccDisplay[1] = B00001001;
  }
  // Chance a step will be transposed a fifth down
  else if (cc == CC_FIFTH_UP) {
    ccFifthChance = value;
    ccDisplay[0] = CHAR_F;
    ccDisplay[1] = CHAR_T;
  }
  // Chance a step will be transposed by a random interval
  else if (cc == CC_RANDOM_INTERVAL) {
    ccRandomIntervalChance = value;
    ccDisplay[0] = CHAR_R;
    ccDisplay[1] = CHAR_N;
  }
  // Chance a step will be have a random length
  else if (cc == CC_RANDOM_LENGTH) {
    ccRandomLengthChance = value;
    ccDisplay[0] = CHAR_R;
    ccDisplay[1] = CHAR_L;
  }
  // Chance a step will be have half the base length
  else if (cc == CC_HALF_LENGTH) {
    ccHalfLengthChance = value;
    ccDisplay[0] = CHAR_H;
    ccDisplay[1] = CHAR_L;
  }
  // Chance a step will be have double the base length
  else if (cc == CC_DOUBLE_LENGTH) {
    ccDoubleLengthChance = value;
    ccDisplay[0] = CHAR_D;
    ccDisplay[1] = CHAR_L;
  }
  // Chance a step will be a ratchet
  else if (cc == CC_RATCHET) {
    ccRatchetChance = value;
    ccDisplay[0] = CHAR_R;
    ccDisplay[1] = CHAR_A;
  }
  // Chance a step will be a rest
  else if (cc == CC_REST) {
    ccRestChance = value;
    ccDisplay[0] = CHAR_R;
    ccDisplay[1] = CHAR_E;
  }
  // Chance a step will be a run
  else if (cc == CC_RUN) {
    ccRunChance = value;
    ccDisplay[0] = CHAR_R;
    ccDisplay[1] = CHAR_U;
  }
  // Chance a step will slip (be swapped with an adjacent step)
  else if (cc == CC_SLIP_CHANCE) {
    ccSlipChance = value;
    ccDisplay[0] = CHAR_S;
    ccDisplay[1] = CHAR_C;
  }
  // Set the base sequence length (in bars)
  else if (cc == CC_SEQUENCE_LENGTH) {
    ccSequenceLength = value;
    ccDisplay[0] = CHAR_S;
    ccDisplay[1] = CHAR_L;

    // Random, 1-8
    if (value < 15) {
      String valStr = "rA";
      valDisplay[0] = valStr[0];
      valDisplay[1] = valStr[1];
    } else {
      byte mapped = map(value, 15, 127, 1, 8);
      String mappedStr = String(mapped);
      valDisplay[0] = ' ';
      valDisplay[1] = mappedStr[0];
    }
  }
  else {
    return;
  }

  // display the change on the 4D7S
  expr->control(ccDisplay, valDisplay);
}

/**
 * Handle changing step in sequence
 *
 * @param {int} stepIndex - index of step in sequence
*/
void Arp::handleStep(int stepIndex) {
  // Turn current note off if there is one
  // (0 means there is no note)
  if (currNote > 0) {
    sendNoteOff(1, currNote, 64);
  }

  byte stepInterval = sequenceIntervals[stepIndex];

  // use 255 to indicate rest
  // #TODO fix this, it's hacky
  if (stepInterval == 255) {
    currNote = 0;
    return;
  }

  // This is where the magic happens!
  // `sequenceIntervals` keeps a sequence of array indexes that are set randomly
  //  and `activeNotes` keeps a set of notes that have been played by the user.
  // The index wraps around the total number of active notes
  // to determine which note to play.
  byte newNoteIndex =  stepInterval % numActiveNotes;
  byte nextNote = activeNotes[newNoteIndex];

  // check to make sure we can safely apply note/octave offset
  if (sequenceOffset[stepIndex] && noteInBounds(nextNote + sequenceOffset[stepIndex])) {
    nextNote = nextNote + sequenceOffset[stepIndex];
  }

  currNote = nextNote;
  // #TODO could we do something more interesting with velocity
  sendNoteOn(1, currNote, 100);
}

/**
 * Handle a new MIDI clock pulse
*/
void Arp::handleClock(unsigned long now) {
  // If we hit the start of a new bar
  // and the button has been pressed, regenerate sequence
  if (pulseCount % PULSES_PER_BAR == 0 && regenerateQueued) {
    regenerateQueued = false;
    generateSequence();
    pulseCount = 0;
  }

  // End of the sequence
  if (pulseCount >= totalSequenceLength) {
    pulseCount = 0;

    // If we triggered a slip, update the sequence
    if (slipQueued) {
      slipQueued = false;
      slipSequence();
    }
  }

  if (totalSequenceLength > 0) {
    // TODO Serious room for optimization here
    // there's a better way than running through each step
    // each clock cycle
    int stepIndex = findStepIndexForPulse(pulseCount);
    if (stepIndex > -1) {
      // This is all to handle swing if we need to.
      // First we check if we're on even (2 of 2) 16ths
      if (ccSwing > 0 && (pulseCount + PULSES_PER_SIXTEENTH_NOTE) % PULSES_PER_EIGHTH_NOTE == 0) {
        // Swing is between 0 and 2 clock pulses in length, so we need to know how long that is
        unsigned long twoClockPulses = (now - lastClockPulse) * 2;
        swingDelay = map(ccSwing, 0, 127, 0, twoClockPulses);
        // Stash what the note was and when it should have played
        swungStepIndex = stepIndex;
        originalSwungNoteTime = now;
      } else {
        handleStep(stepIndex);
      }
    }

    if (pulseCount % PULSES_PER_QUARTER_NOTE == 0) {
      quarterFlipFlop = !quarterFlipFlop;

      // dance
      expr->midiBeat(quarterFlipFlop);
      // light show
      light->midiBeat(quarterFlipFlop);
    }
  }

  ++pulseCount;
  lastClockPulse = now;
}

/**
 * Reset sequence progress
*/
void Arp::reset() {
  pulseCount = 0;
  currNote = 0;
  swungStepIndex = -1;
}

/**
 * Handle start and continue MIDI signals
 *
 * @param {bool} resetSeq - whether to reset progress
*/
void Arp::handleStartContinue(bool resetSeq) {
  if (resetSeq) {
    reset();
  }

  running = true;

  // trigger dance / light show again
  expr->midiBeat(quarterFlipFlop);
  light->midiBeat(quarterFlipFlop);
}

/**
 * Handle stop MIDI signal
*/
void Arp::handleStop() {
  running = false;
  sendNoteOff(1, currNote, 64);
}

/**
 * Initialize Arp
*/
void Arp::setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
}

/**
 * Update to be called during the Arduino update cycle.
 * Reads MIDI messages and tries to handle them.
 *
 * @returns {bool} whether a MIDI message was read
*/
bool Arp::update() {
  unsigned long now = millis();

  if (buttons->play.released) {
    regenerateQueued = true;
  }

  if (buttons->combo(buttons->forward, buttons->backward)) {
    settings->toggleMenu();
  }

  if (settings->inMenu()) {
    settings->updateMenu();
  }

  // Handle swung notes (they don't always land on a clock pulse)
  // #TODO what happens if we stop/continue in the middle of a swung note?
  // the originalSwungNoteTime will be off
  if (swungStepIndex > -1) {
    if (now - originalSwungNoteTime > swingDelay) {
        handleStep(swungStepIndex);
        swungStepIndex = -1;
    }
  }

  bool readMidi = false;

  if (MIDI.read()) {
    readMidi = true;
    switch(MIDI.getType()) {
      case midi::Clock:
        if (running) {
          handleClock(now);
        } else {
          readMidi = false;
        }
        break;
      case midi::NoteOn:
        handleNoteOn(MIDI.getChannel(), MIDI.getData1(), MIDI.getData2());
        break;
      case midi::NoteOff:
        handleNoteOff(MIDI.getChannel(), MIDI.getData1(), MIDI.getData2());
        break;
      case midi::Start:
        handleStartContinue(true);
        break;
      case midi::Continue:
        handleStartContinue(false);
        break;
      case midi::Stop:
        handleStop();
        break;
      case midi::ControlChange: {
        byte channel = MIDI.getChannel();
        byte cc = MIDI.getData1();
        byte value = MIDI.getData2();

        
        // #TODO, these three callbacks could be merged
        // MIDI setup
        if (settings->usesCC(cc)) {
          settings->handleCC(cc, value);
          return;
        }
        // Special controls
        else if (cc >= 102) {
          handleCommandChange(channel, cc, value);
        }
        // Sequence params
        else if (correctInChannel(channel)) {
          handleControlChange(channel, cc, value);
        }
      } break;
      default:
        // if we don't know what MIDI message this is,
        // just pretend like we didn't receive one
        readMidi = false;
        break;
    }
  }

  if (readMidi) {
    midiMode = true;
    lastMidiMessage = now;
  }
  else if (midiMode) {
    if (now - lastMidiMessage > 1000) {
      midiMode = false;
    }
  }

  return midiMode;
}