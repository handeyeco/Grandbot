#include <Synth.h>

// Initialize MIDI library
MIDI_CREATE_DEFAULT_INSTANCE();

Synth::Synth(Voice* _voice) {
  this->voice = _voice;
}

void Synth::handleNoteOn(byte channel, byte pitch, byte velocity) {
  voice->handleNoteOn(channel, pitch, velocity);
}

void Synth::handleNoteOff(byte channel, byte pitch, byte velocity) {
  voice->handleNoteOff(channel, pitch, velocity);
}

void Synth::setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
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