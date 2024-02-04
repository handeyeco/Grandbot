#include <Arduino.h>
#include <Grandbot.h>
#include <MIDI.h>

// Arduino Pins
#define dataPin 11
#define clockPin 13
#define loadPin 10

#define voicePin 3
#define playPin 2
#define lightPin A0

#define redPin 5
#define greenPin 6
#define bluePin 9

// Leave floating
#define randomPin A5

// Set this to 1 and upload to check that
// everything is wired up as expected
#define demoMode 0

// MIDI and Serial don't play along with
// one another, so keep MIDI off by default
#define midiEnabled 1

MIDI_CREATE_DEFAULT_INSTANCE();
Grandbot gb = Grandbot(dataPin, clockPin, loadPin, voicePin, redPin, greenPin, bluePin);

int lastPlayRead = HIGH;

bool midiMode = false;
int lastMidiMessage;


void handleNoteOn(byte channel, byte pitch, byte velocity) {
  gb.handleNoteOn(channel, pitch, velocity);
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
  gb.handleNoteOff(channel, pitch, velocity);
}

void setup() {
  pinMode(playPin, INPUT_PULLUP);

  randomSeed(analogRead(randomPin));

  if (midiEnabled) {
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.begin(MIDI_CHANNEL_OMNI);
  } else {
    Serial.begin(9600);
  }
}

void loop() {
  int light = analogRead(lightPin);
  int playRead = digitalRead(playPin);
  int now = millis();

  if (MIDI.read()) {
    midiMode = true;
    lastMidiMessage = millis();
  }

  if (midiMode) {
    if (now - lastMidiMessage > 30 * 1000) {
      midiMode = false;
    }

    return;
  }

  if (demoMode) {
    if (!midiEnabled) {
      Serial.print("Light: ");
      Serial.print(light);
      Serial.print(" Button: ");
      Serial.println(playRead);
    }
    gb.demo();
    return;
  }

  // play button is active LOW
  if (playRead == LOW && lastPlayRead == HIGH) {
    gb.play();
  }
  lastPlayRead = playRead;

  gb.update(light);
}
