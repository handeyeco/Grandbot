#include <Arduino.h>
#include <pin_defs.h>
#include <Grandbot.h>
#include <MIDI.h>
#include <LedControl.h>
#include <Voice.h>
#include <Light.h>
#include <Synth.h>

// Set this to 1 and upload to check that
// everything is wired up as expected
#define demoMode 0

// MIDI and Serial don't play along with
// one another, so keep MIDI off by default
#define midiEnabled 1

// 4D7S display control
LedControl lc = LedControl(SERIAL_DATA_PIN, SERIAL_CLOCK_PIN, SERIAL_LOAD_PIN, 1);
// Buzzer control
Voice voice = Voice(BUZZER_PIN);
// MIDI/Synth control
Synth synth = Synth(&voice);
// RGB LED control
Light light = Light(RGB_R_PIN, RGB_G_PIN, RGB_B_PIN);
Grandbot gb = Grandbot(&lc, &voice, &light);

int lastPlayRead = HIGH;

bool midiMode = false;
int lastMidiMessage;

void setupLedControl() {
  // Wake up Max7219
  lc.shutdown(0, false);
  // Set the brightness
  lc.setIntensity(0, 14);
  // Only scan 4 digits
  lc.setScanLimit(0, 4);
  // Clear the display
  lc.clearDisplay(0);
}

void setup() {
  pinMode(PLAY_BUTTON_PIN, INPUT_PULLUP);
  randomSeed(analogRead(RANDOM_PIN));

  setupLedControl();

  if (midiEnabled) {
    synth.setup();
  } else {
    Serial.begin(9600);
  }
}

void loop() {
  int lightRead = analogRead(LIGHT_SENSOR_PIN);
  int playRead = digitalRead(PLAY_BUTTON_PIN);
  int now = millis();

  if (synth.update()) {
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
      Serial.print(lightRead);
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

  gb.update(lightRead);
  light.update();
  voice.update();
}
