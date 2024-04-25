#include <Arduino.h>
#include <pin_defs.h>
#include <Grandbot.h>
#include <LedControl.h>
#include <Voice.h>
#include <Light.h>
#include <Arp.h>

// Main Grandbot class
Grandbot gb = Grandbot();
// MIDI/Arp control
Arp arp = Arp(&gb);

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
}
