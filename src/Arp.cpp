#include <Arp.h>

// Initialize MIDI library
MIDI_CREATE_DEFAULT_INSTANCE();

byte CHAR_A = B01110111;
byte CHAR_B = B00011111;
byte CHAR_D = B00111101;
byte CHAR_E = B01001111;
byte CHAR_I = B00110000;
byte CHAR_L = B00001110;
byte CHAR_N = B00010101;
byte CHAR_O = B00011101;
byte CHAR_R = B00000101;
byte CHAR_S = B01011011;
byte CHAR_T = B00001111;

byte CHAR_0 = B01111110;
byte CHAR_1 = B00110000;
byte CHAR_2 = B01101101;
byte CHAR_3 = B01111001;
byte CHAR_4 = B00110011;
byte CHAR_5 = B01011011;
byte CHAR_6 = B01011111;
byte CHAR_7 = B01110000;
byte CHAR_8 = B01111111;
byte CHAR_9 = B01111011;

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

byte ccRoll() {
  return random(127);
}

byte Arp::getNoteLength() {
    // Random, 16, 8, 4
    if (ccBaseNoteLength > 96) {
      // quarter
      return 4;
    }
    else if (ccBaseNoteLength > 64) {
      // 8th
      return 2;
    }
    else if (ccBaseNoteLength > 32) {
      // 16th
      return 1;
    }
    else {
      // random
      return possibleNoteLengths[random(3)];
    }
}

byte Arp::getSequenceLength() {
  // Random, 1-8
  if (ccSequenceLength < 15) {
    return random(1, 9);
  } else {
    return map(ccSequenceLength, 15, 127, 1, 8);
  }
}

void Arp::generateSequence() {
  // This is the length in number of pulses
  // (random number of bars between 1-8)
  uint16_t newTotalSequenceLength = getSequenceLength() * PULSES_PER_BAR;

  // This is the length of the note in pulses
  byte baseNoteLength = getNoteLength() * PULSES_PER_SIXTEENTH_NOTE;

  // Accumulator for steps in pulses
  uint16_t newSequenceLength = 0;

  // Accumulator for discrete steps
  byte stepIndex = 0;

  while (newSequenceLength < newTotalSequenceLength) {
    // Due to adding chaos variation
    // this particular step might not stay the same
    // length as other steps
    byte noteLength = baseNoteLength;
    // used for octaves
    int8_t noteOffset = 0;

    byte randomNoteInterval = random(MAX_NOTES);

    // Random octave; since they affect the same value (offset)
    // there's some interplay between them with a bias
    // towards single octave intervals
    if (ccOctaveOneUpChance || ccOctaveOneDownChance || ccOctaveTwoUpChance || ccOctaveTwoDownChance) {
      byte octOneUpRoll = ccRoll();
      byte octOneDownRoll = ccRoll();
      byte octTwoUpRoll = ccRoll();
      byte octTwoDownRoll = ccRoll();

      if (octOneUpRoll < ccOctaveOneUpChance) {
          // one oct up
          noteOffset = 12;
      } else if (octOneDownRoll < ccOctaveOneDownChance) {
          // one oct down
          noteOffset = -12;
      } else if (octTwoUpRoll < ccOctaveTwoUpChance) {
          // two oct up
          noteOffset = 24;
      } else if (octTwoDownRoll < ccOctaveTwoDownChance) {
          // two oct down
          noteOffset = -24;
      }
    }

    // Random note length; since they affect the same value (length)
    // there's some interplay between them with a bias
    // towards ratchets
    if (ccRatchetChance || ccDoubleLengthChance) {
      byte ratchetRoll = ccRoll();
      byte doubleRoll = ccRoll();
      
      if (noteLength > PULSES_PER_SIXTEENTH_NOTE && ratchetRoll < ccRatchetChance) {
        noteLength = noteLength / 2;
        newSequenceLength = addStep(
          stepIndex,
          randomNoteInterval,
          noteOffset,
          noteLength,
          newSequenceLength
        );
        stepIndex++;
      } else if (doubleRoll < ccDoubleLengthChance) {
        noteLength = noteLength * 2;
      }
    }

    if (ccRestChance) {
      byte restRoll = ccRoll();
      // Random rest
      if (restRoll < ccRestChance) {
        // use 255 to indicate rest
        // TODO fix this, it's hacky
        randomNoteInterval = 255;
      }
    }

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
  byte movedChannel = midiChannelOut == 255 ? channel : midiChannelOut;
  MIDI.sendNoteOn(note, velocity, movedChannel);
}

void Arp::sendNoteOff(byte channel, byte note, byte velocity) {
  byte movedChannel = midiChannelOut == 255 ? channel : midiChannelOut;
  MIDI.sendNoteOff(note, velocity, movedChannel);
}

void Arp::handleNoteOn(byte channel, byte note, byte velocity) {
  if (!correctInChannel(channel)) {
    return;
  }

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
  if (!correctInChannel(channel)) {
    return;
  }

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

String Arp::padded(String input) {
  int len = input.length();
  if (len < 2) {
    String padded = " ";
    padded.concat(input);
    return padded;
  }
  return input;
}

String Arp::convertCCToString(byte value) {
  byte mappedValue = map(value, 0, 127, 0, 99);
  String valueStr = String(mappedValue);
  return padded(valueStr);
}

bool Arp::correctInChannel(byte channel) {
  // 255 is the magic number to represent "all channels are okay"
  if (midiChannelIn == 255 || channel == midiChannelIn) {
    return true;
  }

  return false;
};

void Arp::handleMidiChannelChange(byte channel, byte cc, byte value) {
  byte ccDisplay[2];
  byte mapped = map(value, 0, 127, 1, 16);
  String valueStr = String(mapped);
  String paddedStr = padded(valueStr);
  char valDisplay[2] = {paddedStr[0], paddedStr[1]};

  // no matter what channel we get this message on
  // we'll change the incoming MIDI channel
  // @HACK will likely lead to weird things
  if (cc == CC_CHANNEL_IN) {
    midiChannelIn = mapped;
    ccDisplay[0] = CHAR_I;
    ccDisplay[1] = CHAR_N;
    expr->control(ccDisplay, valDisplay);
  } else if (cc == CC_CHANNEL_OUT && correctInChannel(channel)) {
    midiChannelOut = mapped;
    ccDisplay[0] = CHAR_O;
    ccDisplay[1] = CHAR_T;
    expr->control(ccDisplay, valDisplay);
  }
}

void Arp::handleControlChange(byte channel, byte cc, byte value) {
  String valueStr = convertCCToString(value);
  byte ccDisplay[2];
  char valDisplay[2] = {valueStr[0], valueStr[1]};

  if (cc == CC_OCTAVE_ONE_UP) {
    ccOctaveOneUpChance = value;
    ccDisplay[0] = CHAR_O;
    ccDisplay[1] = B01000000;
  }
  else if (cc == CC_OCTAVE_ONE_DOWN) {
    ccOctaveOneDownChance = value;
    ccDisplay[0] = CHAR_O;
    ccDisplay[1] = B00001000;
  }
  else if (cc == CC_OCTAVE_TWO_UP) {
    ccOctaveTwoUpChance = value;
    ccDisplay[0] = CHAR_O;
    ccDisplay[1] = B01000001;
  }
  else if (cc == CC_OCTAVE_TWO_DOWN) {
    ccOctaveTwoDownChance = value;
    ccDisplay[0] = CHAR_O;
    ccDisplay[1] = B00001001;
  }
  else if (cc == CC_DOUBLE_LENGTH) {
    ccDoubleLengthChance = value;
    ccDisplay[0] = CHAR_D;
    ccDisplay[1] = CHAR_B;
  }
  else if (cc == CC_RATCHET) {
    ccRatchetChance = value;
    ccDisplay[0] = CHAR_R;
    ccDisplay[1] = CHAR_A;
  }
  else if (cc == CC_REST) {
    ccRestChance = value;
    ccDisplay[0] = CHAR_R;
    ccDisplay[1] = CHAR_E;
  }
  else if (cc == CC_BASE_NOTE_LENGTH) {
    ccBaseNoteLength = value;
    ccDisplay[0] = CHAR_N;
    ccDisplay[1] = CHAR_L;

    String valStr = "  ";
    // Random, 16, 8, 4
    if (value > 96) {
      // quarter
      valStr = " 4";
    }
    else if (value > 64) {
      // 8th
      valStr = " 8";
    }
    else if (value > 32) {
      // 16th
      valStr = "16";
    }
    else {
      // random
      valStr = " A";
    }

    valDisplay[0] = valStr[0];
    valDisplay[1] = valStr[1];
  }
  else if (cc == CC_SEQUENCE_LENGTH) {
    ccSequenceLength = value;
    ccDisplay[0] = CHAR_S;
    ccDisplay[1] = CHAR_L;

    // Random, 1-8
    if (value < 15) {
      String valStr = " A";
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

  expr->control(ccDisplay, valDisplay);
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
      case midi::ControlChange: {
        byte channel = MIDI.getChannel();
        byte cc = MIDI.getData1();
        if (cc == CC_CHANNEL_IN || cc == CC_CHANNEL_OUT) {
          handleMidiChannelChange(channel, cc, MIDI.getData2());
        } else if (correctInChannel(channel)) {
          handleControlChange(channel, cc, MIDI.getData2());
        }
      } break;
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