#include <Synth.h>

#define PULSES_PER_QUARTER_NOTE 24
#define PULSES_PER_SIXTEENTH_NOTE 6
#define SIXTEENTH_PER_BAR 16

#define DEBUG_ARP 1

// Initialize MIDI library
MIDI_CREATE_DEFAULT_INSTANCE();

Synth::Synth(LedControl* _lc, Expressions* _expr, Light* _light, int voicePin) {
  this->lc = _lc;
  this->expr = _expr;
  this->light = _light;
  this->voicePin = voicePin;
}

// in 16th notes
const byte possibleSequenceLengths[] = {
  1 * 16, // 1 bar (1 bar * 16(th))
  2 * 16,
  3 * 16,
  4 * 16,
  5 * 16,
  6 * 16,
  7 * 16,
  8 * 16  // 8 bar (8 bar * 16(th)) => 128
};

const byte possibleNoteLengths[] = {
  1, // 16th
  2, // 8th
  4, // quarter
};

void Synth::generateSequence() {
  byte newSequenceLength = 0;
  byte stepIndex = 0;

  // This is the length in number of 16th notes
  // TODO make this use the length of the the array for random
  int fullSequenceLength = possibleSequenceLengths[random(8)];
  byte randomNoteLength = possibleNoteLengths[random(3)];

  while (newSequenceLength < fullSequenceLength) {
    byte randomNoteIndex = random(numPressedNotes);
    // make sure we stay within bounds of the total seq length
    if (newSequenceLength + randomNoteLength > fullSequenceLength) {
      randomNoteLength = fullSequenceLength - newSequenceLength;
    }

    newSequenceLength = newSequenceLength + randomNoteLength;
    sequenceNotes[stepIndex] = pressedNotes[randomNoteIndex];
    sequenceLengths[stepIndex] = randomNoteLength;

    stepIndex++;
  }

  totalSequenceLength = stepIndex;
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

  sendNoteOff(1, sequenceNotes[currNoteIndex], 64);

  // exit if we're maxed out on space for new notes
  if (numPressedNotes + 1 > MAX_NOTES) {
    return;
  }

  for (byte i = 0; i < numPressedNotes; i++) {
    // exit if the note is already pressed
    if (pressedNotes[i] == note) {
      return;
    }
  }

  // append note to end of pressed note array
  ++numPressedNotes;
  pressedNotes[numPressedNotes-1] = note;

  // tone(voicePin, getPitchByNote(note));
  // sendNoteOn(channel, note, velocity);
  generateSequence();
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

  if (noteIndex >= 0 && numPressedNotes > 0) {
    --numPressedNotes;
  }

  // if (numPressedNotes > 0) {
  //   tone(voicePin, getPitchByNote(pressedNotes[numPressedNotes-1]));
  // } else {
  //   noTone(voicePin);
  // }

  // sendNoteOff(channel, note, velocity);
}

void Synth::handleQuarter() {
  if (quarterCount >= 8) {
    quarterCount = 0;
  }

  int even = quarterCount % 2 == 0;
  expr->midiBeat(even);
  light->midiBeat(even);

  quarterCount++;
}

void Synth::handleSixteenth() {
  if (sixteenthCount >= totalSequenceLength) {
    sixteenthCount = 0;
    nextNotePosition = 0;
  }

  if (sixteenthCount == nextNotePosition) {
    // -1 is no current note
    if (currNoteIndex >= 0) {
      sendNoteOff(1, sequenceNotes[currNoteIndex], 64);
    }
    currNoteIndex = (currNoteIndex + 1) % totalSequenceLength;
    sendNoteOn(1, sequenceNotes[currNoteIndex], 100);
    nextNotePosition = nextNotePosition + sequenceLengths[currNoteIndex];
  }

  // if (DEBUG_ARP) {
  //   int v = sixteenthCount;
  //   int ones;  
  //   int tens;  
  //   int hundreds;

  //   ones = v%10;  
  //   v = v/10;  
  //   tens = v%10;  
  //   v = v/10;
  //   hundreds = v; 

  //   lc->clearDisplay(0);
  //   lc->setDigit(0,1,(byte)hundreds,false);
  //   lc->setDigit(0,2,(byte)tens,false); 
  //   lc->setDigit(0,3,(byte)ones,false);
  // }

  ++sixteenthCount;
}

void Synth::handleClock() {
  if (pulseCount % PULSES_PER_SIXTEENTH_NOTE == 0) {
    handleSixteenth();
  }

  if (pulseCount % PULSES_PER_QUARTER_NOTE == 0) {
    handleQuarter();
    // pulseCount = 0;
  }

  ++pulseCount;
}

void Synth::resetCounts() {
  pulseCount = 0;
  sixteenthCount = 0;
  quarterCount = 0;
  currNoteIndex = -1;
  nextNotePosition = 0;
}

void Synth::handleStartContinue(bool reset) {
  lc->clearDisplay(0);

  if (reset) {
    resetCounts();
  }

  int even = quarterCount % 2;
  expr->midiBeat(!even);
  light->midiBeat(!even);
}

void Synth::handleStop() {
  sendNoteOff(1, sequenceNotes[currNoteIndex], 64);
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