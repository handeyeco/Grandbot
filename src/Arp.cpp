#include <Arp.h>

// Initialize MIDI library
// MIDI_CREATE_DEFAULT_INSTANCE(); // uncomment for #NANO
MIDI_CREATE_INSTANCE(HardwareSerial,
                     Serial1,
                     MIDI);  // uncomment for #NANO_EVERY

Arp::Arp(Grandbot* gb) {
  this->gb = gb;
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
    1,  // 16th
    2,  // 8th
    4,  // quarter
    // not used in random note selection
    8,   // half
    16,  // whole
    32   // double whole note
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
uint16_t Arp::addStep(byte stepIndex,
                      byte noteInterval,
                      int8_t noteOffset,
                      byte noteLength,
                      uint16_t startPosition,
                      bool legato) {
  sequenceIntervals[stepIndex] = noteInterval;
  sequenceOffset[stepIndex] = noteOffset;
  sequenceStartPositions[stepIndex] = startPosition;
  sequenceStepLegato[stepIndex] = legato;

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
  byte value = settings->baseNoteLength->getValue();
  byte index = Stepper::getSteppedIndex(value, 7);

  if (index == 0) {
    // random; only choose between
    // 16th, 8th, and quarter
    return possibleNoteLengths[random(3)];
  } else {
    return possibleNoteLengths[index - 1];
  }
}

/**
 * Maps ccSequenceLength (0-127) to sequence lengths
 *
 * @returns {byte} number between 1-8
 */
byte Arp::getSequenceLength() {
  byte ccSequenceLength = settings->sequenceLength->getValue();
  byte index = Stepper::getSteppedIndex(ccSequenceLength, 9);

  // Random
  if (index == 0) {
    return random(1, 9);
  }
  // 1-8 bars
  else {
    return index;
  }
}

/**
 * Handles queueing slip and enabling drift
 *
 * @param {bool} setDrift - whether to enable drift
 */
void Arp::queueSlip(bool setDrift) {
  // having slip and regenerate happen at
  // the same time doesn't make sense
  if (regenerateQueued) {
    regenerateQueued = false;
  }

  drift = setDrift;

  // if the arp is running, delay slip;
  // otherwise do it right away
  if (running) {
    slipQueued = true;
    expr->setLed(4, 1, true);
  } else {
    slipSequence();
  }
}

/**
 * Slip sequence runs through the existing sequence and
 * randomly switches steps (randomness based on ccSlipChance)
 */
void Arp::slipSequence() {
  // no point slipping/drifting if there's no slipChance
  if (!settings->slipChance->getValue()) {
    slipQueued = false;
    drift = false;
    return;
  }

  // drift is endless slip, so only unqueue
  // slip if drift is not enabled
  if (!drift) {
    slipQueued = false;
  }

  for (byte i = 0; i < totalSequenceSteps - 1; i++) {
    if (settings->slipChance->roll()) {
      byte tmpInterval = sequenceIntervals[i + 1];
      int8_t tmpOffset = sequenceOffset[i + 1];
      bool tmpLegato = sequenceStepLegato[i + 1];
      sequenceIntervals[i + 1] = sequenceIntervals[i];
      sequenceOffset[i + 1] = sequenceOffset[i];
      sequenceStepLegato[i + 1] = sequenceStepLegato[i];
      sequenceIntervals[i] = tmpInterval;
      sequenceOffset[i] = tmpOffset;
      sequenceStepLegato[i] = tmpLegato;
    }
  }
}

/**
 * Handles queueing sequence regeneration
 */
void Arp::queueRegenerate() {
  // having slip and regenerate happen at
  // the same time doesn't make sense
  if (!drift && slipQueued) {
    slipQueued = false;
  }

  // if the arp is running, delay generation;
  // otherwise do it right away
  if (running) {
    regenerateQueued = true;
    expr->setLed(4, 1, true);
  } else {
    generateSequence();
  }
}

/**
 * Generate a new sequence
 */
void Arp::generateSequence() {
  regenerateQueued = false;

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
  while (newSequenceLength < newTotalSequenceLength &&
         stepIndex < MAX_STEPS_IN_SEQ) {
    // Due to adding note length variation
    // this particular step might not stay the same
    // length as other steps
    byte noteLength = baseNoteLength;
    // used for transposition variation
    int8_t noteOffset = 0;
    bool legato = settings->legatoChance->roll();

    // which index in the activeNotes array to play
    // (gets wrapped by numActiveNotes)
    // #TODO this name is confusing
    byte randomNoteInterval = random(MAX_NOTES);

    // Random transposition; since they affect the same value (offset)
    // there's some interplay between them with a bias
    // towards single octave intervals
    if (settings->octaveOneUpChance->getValue() ||
        settings->octaveOneDownChance->getValue() ||
        settings->octaveTwoUpChance->getValue() ||
        settings->octaveTwoDownChance->getValue() ||
        settings->fifthChance->getValue() ||
        settings->randomNoteChance->getValue()) {
      if (settings->octaveOneUpChance->roll()) {
        // one oct up
        noteOffset = 12;
      } else if (settings->octaveOneDownChance->roll()) {
        // one oct down
        noteOffset = -12;
      } else if (settings->octaveTwoUpChance->roll()) {
        // two oct up
        noteOffset = 24;
      } else if (settings->octaveTwoDownChance->roll()) {
        // two oct down
        noteOffset = -24;
      } else if (settings->fifthChance->roll()) {
        // fifth up
        noteOffset = 7;
      } else if (settings->randomNoteChance->roll()) {
        // chaos between two octaves (exclusive)
        noteOffset = random(-11, 12);
      }
    }

    // Random note length; this is different than
    // random base note length (affects all notes)
    // this is random for a single step
    if (settings->randomLengthChance->getValue()) {
      if (settings->randomLengthChance->roll()) {
        // only chose between 16th, 8th, and quarter during random selection
        noteLength = possibleNoteLengths[random(3)] * PULSES_PER_SIXTEENTH_NOTE;
      }
    }

    // this needs to happen before ratchets, or else 100% rest chance still has notes
    if (settings->restChance->getValue()) {
      // Random rest
      if (settings->restChance->roll()) {
        // use 255 to indicate rest
        // TODO fix this, it's hacky
        randomNoteInterval = 255;
        // no point in sliding from a note to a rest
        legato = false;
      }
    }

    // Note length variation; since they affect the same value (length)
    // there's some interplay between them with a bias
    // towards ratchets
    if (settings->ratchetChance->getValue() ||
        settings->halfLengthChance->getValue() ||
        settings->doubleLengthChance->getValue() ||
        settings->runChance->getValue()) {
      if (settings->ratchetChance->roll()) {
        // TODO: do we need to handle legato differently for ratchets?
        noteLength = noteLength / 2;
        newSequenceLength = addStep(stepIndex, randomNoteInterval, noteOffset,
                                    noteLength, newSequenceLength, legato);
        stepIndex++;
      } else if ((stepIndex + 4 < MAX_STEPS_IN_SEQ) &&
                 (settings->runChance->roll())) {
        for (int i = 0; i < 4; i++) {
          newSequenceLength =
              addStep(stepIndex, i % MAX_NOTES, noteOffset,
                      PULSES_PER_SIXTEENTH_NOTE / 2, newSequenceLength, legato);
          stepIndex++;
        }
      } else if (settings->halfLengthChance->roll()) {
        noteLength = noteLength / 2;
      } else if (settings->doubleLengthChance->roll()) {
        noteLength = noteLength * 2;
      }
    }

    // take all this variation and add a step to the sequence
    newSequenceLength = addStep(stepIndex, randomNoteInterval, noteOffset,
                                noteLength, newSequenceLength, legato);
    stepIndex++;
  }

  byte collapseIndex =
      Stepper::getSteppedIndex(settings->collapseNotes->getValue(), 3);
  // move rests to the end
  if (collapseIndex == 1) {
    byte count = 0;
    for (byte i = 0; i < stepIndex; i++) {
      if (sequenceIntervals[i] != 255) {
        byte tmp = sequenceIntervals[i];
        sequenceIntervals[i] = sequenceIntervals[count];
        sequenceIntervals[count] = tmp;
        count++;
      }
    }
  }
  // move rests to the beginning
  else if (collapseIndex == 2) {
    int end = -1;
    for (int i = stepIndex - 1; i >= 0; i--) {
      if (sequenceIntervals[i] == 255) {
        end = i;
        break;
      }
    }

    for (int i = end - 1; i >= 0; i--) {
      if (sequenceIntervals[i] != 255) {
        byte tmp = sequenceIntervals[i];
        sequenceIntervals[i] = sequenceIntervals[end];
        sequenceIntervals[end] = tmp;
        end--;
      }
    }
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
  // if midiChannelOut is 0, we use whatever channel was provided,
  // otherwise use the channel set in midiChannelOut
  byte outCh = ccToMidiCh(settings->midiChannelOut->getValue());
  byte movedChannel = outCh == 0 ? channel : outCh;
  MIDI.sendNoteOn(note, velocity, movedChannel);

  // if the speaker is set to be on, play note on buzzer
  if (settings->useSpeaker->getValueAsBool()) {
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
  // note 0 means no current note, so nothing to do
  if (note == 0) {
    return;
  }

  // if midiChannelOut is 0, we use whatever channel was provided,
  // otherwise use the channel set in midiChannelOut
  byte outCh = ccToMidiCh(settings->midiChannelOut->getValue());
  byte movedChannel = outCh == 0 ? channel : outCh;
  MIDI.sendNoteOff(note, velocity, movedChannel);
  noTone(BUZZER_PIN);
}

byte Arp::ccToMidiCh(byte value) {
  return Stepper::getSteppedIndex(value, 17);
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
    for (int j = i; j > 0 && arr[j - 1] > arr[j]; j--) {
      byte tmp = arr[j - 1];
      arr[j - 1] = arr[j];
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

  if (settings->sort->getValueAsBool()) {
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
      pressedNotes[i] = pressedNotes[i + 1];
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
  // 0 is the magic number to represent "all channels are okay"
  // otherwise we only care about the channel set in midiChannelIn
  byte inCh = ccToMidiCh(settings->midiChannelIn->getValue());
  if (inCh == 0 || channel == inCh) {
    return true;
  }

  return false;
};

/**
 * Goes through every note on every channel
 * sending a "note off" message
 *
 * TODO should it also send CC123 "all notes off"?
 * 
 * TODO it should also stop the internal clock
 */
void Arp::panic() {
  byte fullDisplay[4];
  fullDisplay[0] = CHAR_A;
  fullDisplay[1] = CHAR_H;
  fullDisplay[2] = CHAR_H;
  fullDisplay[3] = CHAR_H;
  expr->writeText(fullDisplay, false);

  noTone(BUZZER_PIN);

  for (byte ch = 0; ch < 16; ch++) {
    for (byte note = 0; note < 128; note++) {
      MIDI.sendNoteOff(note, 64, ch + 1);
    }
  }
}

/**
 * Handle changing MIDI CC values for "command" signals
 *
 * NOTE: this mostly ignores messages that are on the wrong channel,
 * except for Panic signals
 *
 * @param {byte} channel - MIDI channel received on
 * @param {byte} cc - MIDI CC received
 * @param {byte} value - MIDI value received
 */
void Arp::handleCommandChange(byte channel, byte cc, byte value) {
  bool isOn = Setting::convertCCToBool(value);

  // try to shut it all down
  // (no matter what channel we get this message on
  // we'll trigger a panic)
  // @HACK will likely lead to weird things)
  if (cc == CC_PANIC) {
    bool wasOff = !Setting::convertCCToBool(ccPanic);
    ccPanic = value;
    if (wasOff && isOn) {
      panic();
    }
  }

  if (!correctInChannel(channel))
    return;

  // Queue a new sequence
  if (cc == CC_GENERATE_SEQUENCE) {
    bool wasOff = !Setting::convertCCToBool(ccGenerate);
    ccGenerate = value;
    if (wasOff && isOn) {
      queueRegenerate();
    }
  }
  // Queue a sequence slip
  else if (cc == CC_SLIP) {
    bool wasOff = !Setting::convertCCToBool(ccSlip);
    ccSlip = value;
    if (wasOff && isOn) {
      queueSlip(false);
    }
  }
  // Toggle drift mode
  else if (cc == CC_DRIFT) {
    bool wasOff = !Setting::convertCCToBool(ccDrift);
    ccDrift = value;
    if (wasOff && isOn) {
      if (drift) {
        drift = false;
        slipQueued = false;
      } else {
        queueSlip(true);
      }
    }
  }
  // Randomize generation parameters
  else if (cc == CC_RANDOMIZE_CHANCES) {
    bool wasOff = !Setting::convertCCToBool(ccRandomChances);
    ccRandomChances = value;
    if (wasOff && isOn) {
      settings->randomize();
    }
  }
  // Reset generation parameters to defaults
  else if (cc == CC_RESET_CHANCES) {
    bool wasOff = !Setting::convertCCToBool(ccResetChances);
    ccResetChances = value;
    if (wasOff && isOn) {
      settings->reset();
    }
  }
}

/**
 * Handle changing step in sequence
 *
 * @param {int} stepIndex - index of step in sequence
 */
void Arp::handleStep(int stepIndex) {
  byte stepInterval = sequenceIntervals[stepIndex];
  bool stepLegato = sequenceStepLegato[stepIndex];

  if (legatoNote != 0) {
    sendNoteOff(1, legatoNote, 64);
  }

  legatoNote = stepLegato ? currNote : 0;

  // use 255 to indicate rest
  // #TODO fix this, it's hacky
  // (also currNote = 0 is no note and stepInterval = 255 is rest,
  // which is confusing)
  if (stepInterval == 255) {
    // if the next note is a rest and latch is enabled,
    // do nothing
    if (settings->latch->getValueAsBool()) {
      return;
    }
    // if the next note is a rest and latch is disabled,
    // send note off and rest
    else {
      // TODO: why do we have an "all" MIDI out setting if
      // we just default to channel 1?
      sendNoteOff(1, currNote, 64);
      currNote = 0;
      return;
    }
  }
  // if the next note is not a rest, send note off for the current note
  // unless we're holding it for legato
  else if (!stepLegato) {
    // TODO: why do we have an "all" MIDI out setting if
    // we just default to channel 1?
    sendNoteOff(1, currNote, 64);
  }

  // This is where the magic happens!
  // `sequenceIntervals` keeps a sequence of array indexes that are set randomly
  //  and `activeNotes` keeps a set of notes that have been played by the user.
  // The index wraps around the total number of active notes
  // to determine which note to play.
  byte newNoteIndex = stepInterval % numActiveNotes;
  byte nextNote = activeNotes[newNoteIndex];

  // check to make sure we can safely apply note/octave offset
  if (sequenceOffset[stepIndex] &&
      noteInBounds(nextNote + sequenceOffset[stepIndex])) {
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
  // End of the sequence
  if (pulseCount >= totalSequenceLength) {
    pulseCount = 0;

    // Regenerate sequence if queued
    if (regenerateQueued) {
      generateSequence();
    }

    // If we triggered a slip, update the sequence
    if (slipQueued) {
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
      byte ccSwing = settings->swing->getValue();
      if (ccSwing > 0 &&
          (pulseCount + PULSES_PER_SIXTEENTH_NOTE) % PULSES_PER_EIGHTH_NOTE ==
              0) {
        // Swing is between 0 and 2 clock pulses in length, so we need to know
        // how long that is
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
      // TODO, need to find a better way to handle screen state
      bool showQueueDot =
          !settings->inMenu() && (regenerateQueued || slipQueued);
      expr->midiBeat(quarterFlipFlop, showQueueDot);
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
 *
 * TODO should this play the current note when continuing?
 */
void Arp::handleStartContinue(bool resetSeq) {
  if (resetSeq || settings->clock->getValueAsBool()) {
    reset();
  }

  running = true;
  lastInternalClockPulseTime = millis();

  // trigger dance / light show again
  // TODO, need to find a better way to handle screen state
  bool showQueueDot = !settings->inMenu() && (regenerateQueued || slipQueued);
  expr->midiBeat(quarterFlipFlop, showQueueDot);
  light->midiBeat(quarterFlipFlop);
}

/**
 * Handle stop MIDI signal
 */
void Arp::handleStop() {
  running = false;
  regenerateQueued = false;
  drift = false;
  slipQueued = false;

  sendNoteOff(1, currNote, 64);
  sendNoteOff(1, legatoNote, 64);
}

/**
 * Initialize Arp
 */
void Arp::setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
}

/**
 * Go through and respond to button presses
 *
 * @param {bool} useInternalClock - whether or not we're using an internal
 * clock/transport
 */
void Arp::handleButtons(bool useInternalClock) {
  if (!(buttons->anyPressed || buttons->anyReleased || buttons->anyHeld)) {
    return;
  }

  // Handle Grandbot Controller buttons
  // Two right buttons are a shortcut for the menu
  if (buttons->combo(buttons->forward, buttons->backward)) {
    settings->toggleMenu();
    return;
  } else if (!settings->inMenu()) {
    // Forward + Left is a shortcut for panic
    if (buttons->combo(buttons->forward, buttons->left)) {
      panic();
      return;
    }
    // Toggle drift mode
    else if (buttons->combo(buttons->forward, buttons->down)) {
      if (drift) {
        drift = false;
        slipQueued = false;
      } else {
        queueSlip(true);
      }
      return;
    }
    // Randomize sequence settings
    else if (buttons->combo(buttons->forward, buttons->up)) {
      settings->randomize();
      return;
    }
    // Reset sequence settings
    else if (buttons->combo(buttons->forward, buttons->right)) {
      settings->reset();
      return;
    }
    // Up || Play triggers sequence generation
    else if (buttons->play.released || buttons->up.released) {
      queueRegenerate();
      return;
    }
    // Down triggers sequence slip
    else if (buttons->down.released) {
      queueSlip(false);
      return;
    }
    // Left triggers GB play sequence (unrelated to the Arp)
    else if (buttons->left.released) {
      gb->play();
      return;
    }
    // Play / Pause
    else if (useInternalClock && buttons->forward.released && !running) {
      handleStartContinue(true);
      MIDI.sendStart();
      return;
    }
    // Stop
    else if (useInternalClock && buttons->backward.released && running) {
      handleStop();
      MIDI.sendStop();
      return;
    }
  }
}

/**
 * Update to be called during the Arduino update cycle.
 * Reads MIDI messages and tries to handle them.
 *
 * @returns {bool} whether a MIDI message was read
 */
bool Arp::update() {
  // TODO might be best to switch this all to micros
  unsigned long now = millis();
  bool readMidi = false;

  bool useInternalClock = settings->clock->getValueAsBool();

  handleButtons(useInternalClock);

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

  // calculate internal clock pulse if internal clock enabled
  if (useInternalClock && running) {
    unsigned long nowMicros = micros();

    // TODO this could be optimized by only calculating values on BPM change
    byte bpm = settings->bpm->getValue() + BPM_OFFSET;
    unsigned long timeBetweenInternalClockPulses =
        60000000L / (PULSES_PER_QUARTER_NOTE * bpm);
    if (nowMicros - lastInternalClockPulseTime >
        timeBetweenInternalClockPulses) {
      lastInternalClockPulseTime = nowMicros;
      handleClock(now);
      MIDI.sendClock();
    }
  }

  if (MIDI.read()) {
    readMidi = true;
    switch (MIDI.getType()) {
      case midi::Clock:
        if (running && !useInternalClock) {
          handleClock(now);
          MIDI.sendClock();
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
        MIDI.sendStart();
        break;
      case midi::Continue:
        handleStartContinue(false);
        MIDI.sendContinue();
        break;
      case midi::Stop:
        handleStop();
        MIDI.sendStop();
        break;
      case midi::ControlChange: {
        byte channel = MIDI.getChannel();
        byte cc = MIDI.getData1();
        byte value = MIDI.getData2();

        // #TODO, these three callbacks could be merged
        // MIDI setup
        if (settings->usesCC(cc)) {
          // TODO filter in channel
          settings->handleCC(cc, value);
          return;
        }
        // Special controls
        else {
          handleCommandChange(channel, cc, value);
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
  } else if (midiMode) {
    if (now - lastMidiMessage > 1000) {
      midiMode = false;
    }
  }

  return midiMode || (useInternalClock && running);
}