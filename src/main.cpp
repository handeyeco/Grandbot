#include <Arduino.h>
#include <pin_defs.h>
#include <Grandbot.h>
#include <MIDI.h>
#include <LedControl.h>
#include <Voice.h>
#include <Light.h>
#include <Arp.h>

// MIDI and Serial don't play along with
// one another, so keep MIDI off by default
#define midiEnabled 1

// RGB LED control
Light light = Light(RGB_R_PIN, RGB_G_PIN, RGB_B_PIN);
// 4D7S display control
LedControl lc = LedControl(SERIAL_DATA_PIN, SERIAL_CLOCK_PIN, SERIAL_LOAD_PIN, 1);
// Expression manager
Expressions expr = Expressions(&lc, &light);
// MIDI/Arp control
Arp arp = Arp(&expr, &light, BUZZER_PIN);
// Buzzer control
Voice voice = Voice(BUZZER_PIN);
Grandbot gb = Grandbot(&expr, &lc, &voice, &light);

bool lastPlayRead = HIGH;

bool midiMode = false;
unsigned long lastMidiMessage;

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
  expr.init();

  if (midiEnabled) {
    arp.setup();
  } else {
    Serial.begin(9600);
  }
}

void loop() {
  unsigned long now = millis();

  int lightRead = analogRead(LIGHT_SENSOR_PIN);
  bool playRead = digitalRead(PLAY_BUTTON_PIN);
  bool playPress = playRead == LOW && lastPlayRead == HIGH;
  lastPlayRead = playRead;

  if (playPress) {
    arp.playButtonPress();
  }

  if (arp.update()) {
    midiMode = true;
    lastMidiMessage = now;
  }

  if (midiMode) {
    if (now - lastMidiMessage > 1 * 1000) {
      midiMode = false;
    }

    return;
  }

  if (playPress) {
    gb.play();
  }

  gb.update(lightRead);
  light.update();
  voice.update();
}
