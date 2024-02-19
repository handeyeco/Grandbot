#include <Synth.h>

// Initialize MIDI library
MIDI_CREATE_DEFAULT_INSTANCE();

Synth::Synth(Expressions* _expr, Light* _light, int voicePin) {
  this->expr = _expr;
  this->light = _light;
  this->voicePin = voicePin;
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

  tone(voicePin, getPitchByNote(note));
  sendNoteOn(channel, note, velocity);
}

void Synth::handleNoteOff(byte channel, byte note, byte velocity) {
  // BUG: for some reason updating the LED is triggering Note Off?
  // Arturia keystep sends a note off velocity of 64?
  if (velocity != 64) {
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

  if (noteIndex >= 0 && numPressedNotes > 0) {
    --numPressedNotes;
  }

  if (numPressedNotes > 0) {
    tone(voicePin, getPitchByNote(pressedNotes[numPressedNotes-1]));
  } else {
    noTone(voicePin);
  }

  sendNoteOff(channel, note, velocity);
}

void Synth::handleClock() {
  ++count;
  if (count >= 20) {
    ++beat;
    count = 0;
    if (beat >= 4) {
      beat = 0;
    }
    expr->midiBeat(beat);
    light->midiBeat(beat);
  }
}

void Synth::handleStartContinue(bool reset) {
  if (!reset) {
    count = 0;
    beat = 0;
  }
  expr->midiBeat(beat);
  light->midiBeat(beat);
}

void Synth::handleStop() {
  count = 0;
  beat = 0;
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
      case midi::NoteOn:
        handleNoteOn(MIDI.getChannel(), MIDI.getData1(), MIDI.getData2());
        break;
      case midi::NoteOff:
        handleNoteOff(MIDI.getChannel(), MIDI.getData1(), MIDI.getData2());
        break;
      case midi::Clock:
        handleClock();
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
      default:
        // if we don't know what MIDI message this is,
        // just pretend like we didn't receive one
        readMidi = false;
        break;
    }
  }

  return readMidi;
}