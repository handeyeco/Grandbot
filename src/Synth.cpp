#include <Synth.h>

// Initialize MIDI library
MIDI_CREATE_DEFAULT_INSTANCE();

Synth::Synth(LedControl* _lc, Light* _light, int voicePin) {
  this->lc = _lc;
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
  tone(voicePin, getPitchByNote(note));
  sendNoteOn(channel, note, velocity);
}

void Synth::handleNoteOff(byte channel, byte note, byte velocity) {
  noTone(voicePin);
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
    lc->clearDisplay(0);
    lc->setDigit(0,beat,beat+1,false);
  }
}

void Synth::handleStart() {
  count = 0;
  beat = 0;
  lc->clearDisplay(0);
  lc->setDigit(0,0,1,false);
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
        handleStart();
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