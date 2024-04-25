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

void setup() {
  gb.setup();
  arp.setup();
}

void loop() {
  int lightRead = analogRead(LIGHT_SENSOR_PIN);
  bool playRead = digitalRead(PLAY_BUTTON_PIN);
  bool playPress = playRead == LOW && lastPlayRead == HIGH;
  lastPlayRead = playRead;

  if (playPress) {
    arp.playButtonPress();
  }

  if (arp.update()) {
    return;
  }

  if (playPress) {
    gb.play();
  }

  gb.update(lightRead);
  light.update();
  voice.update();
}
