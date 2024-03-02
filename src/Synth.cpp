#include <Synth.h>

// Initialize MIDI library
MIDI_CREATE_DEFAULT_INSTANCE();

Synth::Synth(LedControl* _lc, Expressions* _expr, Light* _light, int voicePin) {
  this->lc = _lc;
  this->expr = _expr;
  this->light = _light;
  this->voicePin = voicePin;
}

const byte possibleNoteLengths[] = {
  1, // 16th
  2, // 8th
  4, // quarter
};

void Synth::generateSequence() {
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
    byte randomNoteInterval = random(MAX_NOTES);

    // Make sure we stay within bounds of the total seq length
    byte noteLength = randomNoteLength;
    // if (newSequenceLength + noteLength > newTotalSequenceLength) {
    //   noteLength = newTotalSequenceLength - newSequenceLength;
    // }

    sequenceIntervals[stepIndex] = randomNoteInterval;
    sequenceStartPositions[stepIndex] = newSequenceLength;

    newSequenceLength = newSequenceLength + noteLength;

    stepIndex++;
  }

  totalSequenceSteps = stepIndex;
  totalSequenceLength = newTotalSequenceLength;
}

void Synth::sendNoteOn(byte channel, byte note, byte velocity) {
  MIDI.sendNoteOn(note, velocity, channel);
}

void Synth::sendNoteOff(byte channel, byte note, byte velocity) {
  MIDI.sendNoteOff(note, velocity, channel);
}

void Synth::handleNoteOn(byte channel, byte note, byte velocity) {
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

void Synth::handleNoteOff(byte channel, byte note, byte velocity) {
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
int Synth::findStepIndexForPulse(uint16_t pulse) {
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

void Synth::handleStep(int stepIndex) {
  // -1 is no current note
  if (currNote > -1) {
    sendNoteOff(1, currNote, 64);
  }

  int newNoteIndex = sequenceIntervals[stepIndex] % numActiveNotes;
  currNote = activeNotes[newNoteIndex];

  sendNoteOn(1, currNote, 100);
}

void Synth::handleClock() {
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

void Synth::reset() {
  pulseCount = 0;
  currNote = -1;
}

void Synth::handleStartContinue(bool resetSeq) {
  lc->clearDisplay(0);

  if (resetSeq) {
    reset();
  }

  expr->midiBeat(quarterFlipFlop);
  light->midiBeat(quarterFlipFlop);
}

void Synth::handleStop() {
  sendNoteOff(1, currNote, 64);
}

void Synth::setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
}

bool Synth::update() {
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

void Synth::playButtonPress() {
  generateSequence();
}