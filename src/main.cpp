#include <Arduino.h>
#include <Grandbot.h>
#include <Arp.h>

Grandbot gb = Grandbot();
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
