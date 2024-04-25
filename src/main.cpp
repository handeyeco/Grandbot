#include <Arduino.h>
#include <Grandbot.h>
#include <Arp.h>

// Main Grandbot class
Grandbot gb = Grandbot();
// MIDI/Arp control
Arp arp = Arp(&gb);

void setup() {
  gb.setup();
  arp.setup();
}

void loop() {
  bool buttonPressed = gb.readButton();
  if (!arp.update(buttonPressed)) {
    gb.update(buttonPressed);
  }
}
