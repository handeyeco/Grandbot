#include <Synth.h>

// Initialize MIDI library
MIDI_CREATE_DEFAULT_INSTANCE();

Synth::Synth(int voicePin) {
  this->voicePin = voicePin;
}

void Synth::sendNoteOn(byte channel, byte note, byte velocity) {
  MIDI.sendNoteOn(note, velocity, channel);
}

void Synth::sendNoteOff(byte channel, byte note, byte velocity) {
  MIDI.sendNoteOff(note, velocity, channel);
}

void Synth::handleNoteOn(byte channel, byte note, byte velocity) {
  tone(voicePin, sNotePitches[note - 23]);
  sendNoteOn(channel, note, velocity);
}

void Synth::handleNoteOff(byte channel, byte note, byte velocity) {
  noTone(voicePin);
  sendNoteOff(channel, note, velocity);
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
      default:
        // if we don't know what MIDI message this is,
        // just pretend like we didn't receive one
        readMidi = false;
        break;
    }
  }

  return readMidi;
}