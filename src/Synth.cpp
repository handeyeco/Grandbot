#include <Synth.h>

// Initialize MIDI library
MIDI_CREATE_DEFAULT_INSTANCE();

Synth::Synth(LedControl* _lc, Expressions* _expr, Light* _light, int voicePin) {
  this->lc = _lc;
  this->expr = _expr;
  this->light = _light;
  this->voicePin = voicePin;

  SequenceStep* step0 = new SequenceStep(0, PULSES_PER_QUARTER_NOTE * 0);
  SequenceStep* step1 = new SequenceStep(1, PULSES_PER_QUARTER_NOTE * 1);
  SequenceStep* step2 = new SequenceStep(2, PULSES_PER_QUARTER_NOTE * 2);
  SequenceStep* step3 = new SequenceStep(3, PULSES_PER_QUARTER_NOTE * 3);
  sequenceSteps[0] = step0;
  sequenceSteps[1] = step1;
  sequenceSteps[2] = step2;
  sequenceSteps[3] = step3;
}

const byte possibleNoteLengths[] = {
  1, // 16th
  2, // 8th
  4, // quarter
};

void Synth::generateSequence() {
  // This is the length in number of pulses
  // (random number of bars between 1-8)
  totalSequenceLength = random(1, 9) * PULSES_PER_BAR;

  // This is the length of the note in pulses
  byte randomNoteLength = possibleNoteLengths[random(3)] * PULSES_PER_SIXTEENTH_NOTE;

  // Accumulator for steps (in pulses)
  uint16_t newSequenceLength = 0;

  // Discrete steps
  byte stepIndex = 0;

  while (newSequenceLength < totalSequenceLength) {
    byte randomNoteInterval = random(MAX_NOTES);

    // Make sure we stay within bounds of the total seq length
    byte noteLength = randomNoteLength;
    // if (newSequenceLength + noteLength > totalSequenceLength) {
    //   noteLength = totalSequenceLength - newSequenceLength;
    // }

    SequenceStep* step = new SequenceStep(randomNoteInterval, newSequenceLength);
    newSequenceLength = newSequenceLength + noteLength;
    sequenceSteps[stepIndex] = step;

    stepIndex++;
  }

  totalSequenceSteps = stepIndex;
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
  for (int i = 0; i < MAX_STEPS_IN_SEQ; ++i) {
    if (pulse == sequenceSteps[i]->noteStartPosition) {
      return i;
    }

    // steps are in sequential order, so if we haven't
    // found the right step we aren't going to find it
    if (pulse < sequenceSteps[i]->noteStartPosition) {
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

  SequenceStep* step = sequenceSteps[stepIndex];
  int newNoteIndex = step->noteIndex % numActiveNotes;
  currNote = activeNotes[newNoteIndex];

  sendNoteOn(1, currNote, 100);
}

void Synth::handleClock() {
  if (pulseCount > totalSequenceLength) {
    pulseCount = 0;
  }

  if (totalSequenceLength > 0) {
    if (pulseCount % PULSES_PER_QUARTER_NOTE == 0) {
      quarterFlipFlop = !quarterFlipFlop;

      expr->midiBeat(quarterFlipFlop);
      light->midiBeat(quarterFlipFlop);
    }

    // TODO Serious room for optimization here
    // there's a better way than running through each step
    // each clock cycle
    int stepIndex = findStepIndexForPulse(pulseCount);
    if (stepIndex > -1) {
      handleStep(stepIndex);
    }
  }

  ++pulseCount;
}

void Synth::resetCounts() {
  pulseCount = 0;
  currNote = -1;
}

void Synth::handleStartContinue(bool reset) {
  lc->clearDisplay(0);

  if (reset) {
    resetCounts();
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