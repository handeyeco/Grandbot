#include <Arp.h>

// Initialize MIDI library
MIDI_CREATE_DEFAULT_INSTANCE();

Arp::Arp(Expressions* _expr, Light* _light, int voicePin) {
  this->expr = _expr;
  this->light = _light;
  this->voicePin = voicePin;
}

const byte possibleNoteLengths[] = {
  1, // 16th
  2, // 8th
  4, // quarter
};

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

void Arp::generateSequence() {
  // This is the length in number of pulses
  // (random number of bars between 1-8)
  uint16_t newTotalSequenceLength = random(1, 9) * PULSES_PER_BAR;

  // This is the length of the note in pulses
  byte randomNoteLength = possibleNoteLengths[random(3)] * PULSES_PER_SIXTEENTH_NOTE;

  // Accumulator for steps in pulses
  uint16_t newSequenceLength = 0;

  // Accumulator for discrete steps
  byte stepIndex = 0;

  while (newSequenceLength < newTotalSequenceLength) {
    // Due to adding chaos variation
    // this particular step might not stay the same
    // length as other steps
    byte noteLength = randomNoteLength;
    // used for octaves
    int8_t noteOffset = 0;

    byte randomNoteInterval = random(MAX_NOTES);
    byte diceRoll = random(256);

    // Random octave
    if (diceRoll < 10) {
      byte offsetDiceRoll = random(100);

      // TODO this could be a switch,
      // it just wasn't working as expected
      if (offsetDiceRoll < 10) {
          // two oct down
          noteOffset = -24;
      } else if (offsetDiceRoll < 20) {
          // two oct up
          noteOffset = 24;
      } else if (offsetDiceRoll < 50) {
          // one oct down
          noteOffset = -12;
      } else {
          // one oct up
          noteOffset = 12;
      }
    }

    // Random ratchet (don't do this for 16ths)
    else if (diceRoll < 20 && noteLength > PULSES_PER_SIXTEENTH_NOTE) {
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

    // Double note length
    else if (diceRoll < 30) {
      noteLength = noteLength * 2;
    }

    // Random rest
    else if (diceRoll < 40) {
      // use 255 to indicate rest
      // TODO fix this, it's hacky
      randomNoteInterval = 255;
    }

    // Make sure we stay within bounds of the total seq length
    // if (newSequenceLength + noteLength > newTotalSequenceLength) {
    //   noteLength = newTotalSequenceLength - newSequenceLength;
    // }

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

void Arp::sendNoteOn(byte channel, byte note, byte velocity) {
  MIDI.sendNoteOn(note, velocity, channel);
}

void Arp::sendNoteOff(byte channel, byte note, byte velocity) {
  MIDI.sendNoteOff(note, velocity, channel);
}

void Arp::handleNoteOn(byte channel, byte note, byte velocity) {
  if (velocity == 0) {
    handleNoteOff(channel, note, velocity);
    return;
  }

  if (numPressedNotes == 0) {
    numActiveNotes = 0;
  }

  // Exit if we're maxed out on space for new notes
  if (numPressedNotes + 1 > MAX_NOTES || numActiveNotes + 1 > MAX_NOTES) {
    return;
  }

  for (byte i = 0; i < numActiveNotes; i++) {
    // exit if the note is already active
    if (activeNotes[i] == note) {
      return;
    }
  }

  // append note to end of pressed note array
  ++numPressedNotes;
  pressedNotes[numPressedNotes-1] = note;

  ++numActiveNotes;
  activeNotes[numPressedNotes-1] = note;
}

void Arp::handleNoteOff(byte channel, byte note, byte velocity) {
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

// Find a step that starts on this pulse
// (returns -1 if there isn't one)
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

// keep note between MIDI note ranges of
// 23 (B0) and 110 (D8)
bool Arp::noteInBounds(byte note) {
  return note >= 23 && note <= 110;
}

void Arp::handleStep(int stepIndex) {
  // 0 is no current note
  if (currNote > 0) {
    sendNoteOff(1, currNote, 64);
  }

  byte stepInterval = sequenceIntervals[stepIndex];

  // use 255 to indicate rest
  // TODO fix this, it's hacky
  if (stepInterval == 255) {
    currNote = 0;
    return;
  }

  byte newNoteIndex =  stepInterval % numActiveNotes;
  byte nextNote = activeNotes[newNoteIndex];

  // check to make sure we can safely apply note/octave offset
  if (sequenceOffset[stepIndex] && noteInBounds(nextNote + sequenceOffset[stepIndex])) {
    nextNote = nextNote + sequenceOffset[stepIndex];
  }

  currNote = nextNote;
  sendNoteOn(1, currNote, 100);
}

void Arp::handleClock() {
  // If the button has been pressed and we hit the start of a new bar
  // regenerate sequence
  if (regenerateQueued && pulseCount % PULSES_PER_BAR == 0) {
    regenerateQueued = false;
    generateSequence();
    pulseCount = 0;
  }

  if (pulseCount >= totalSequenceLength) {
    pulseCount = 0;
  }

  if (totalSequenceLength > 0) {
    // TODO Serious room for optimization here
    // there's a better way than running through each step
    // each clock cycle
    int stepIndex = findStepIndexForPulse(pulseCount);
    if (stepIndex > -1) {
      handleStep(stepIndex);
    }

    if (pulseCount % PULSES_PER_QUARTER_NOTE == 0) {
      quarterFlipFlop = !quarterFlipFlop;

      expr->midiBeat(quarterFlipFlop);
      light->midiBeat(quarterFlipFlop);
    }
  }

  ++pulseCount;
}

void Arp::reset() {
  pulseCount = 0;
  currNote = 0;
}

void Arp::handleStartContinue(bool resetSeq) {
  if (resetSeq) {
    reset();
  }

  expr->midiBeat(quarterFlipFlop);
  light->midiBeat(quarterFlipFlop);
}

void Arp::handleStop() {
  sendNoteOff(1, currNote, 64);
}

void Arp::setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
}

bool Arp::update() {
  bool readMidi = false;

  if (MIDI.read()) {
    readMidi = true;
    switch(MIDI.getType()) {
      case midi::Clock:
        handleClock();
        break;
      case midi::NoteOn:
        handleNoteOn(MIDI.getChannel(), MIDI.getData1(), MIDI.getData2());
        break;
      case midi::NoteOff:
        handleNoteOff(MIDI.getChannel(), MIDI.getData1(), MIDI.getData2());
        break;
      case midi::Start:
        handleStartContinue(true);
        handleClock();
        break;
      case midi::Continue:
        handleStartContinue(false);
        handleClock();
        break;
      case midi::Stop:
        handleStop();
        break;
      default:
        // if we don't know what MIDI message this is,
        // just pretend like we didn't receive one
        readMidi = false;
        break;
    }
  }

  return readMidi;
}

void Arp::playButtonPress() {
  regenerateQueued = true;
}