#include <Arduino.h>
#include <Grandbot.h>
#include <Arp.h>

Grandbot gb = Grandbot();
Arp arp = Arp(&gb);

void setup() {
  gb.setup();
  arp.setup();

  Serial.begin(9600);
}

void loop() {
  bool buttonPressed = gb.readButton();
  if (!arp.update(buttonPressed)) {
    gb.update(buttonPressed);
  }
}

// NO ARP OPTION
// =============
// #include <Arduino.h>
// #include <Grandbot.h>

// Grandbot gb = Grandbot();

// void setup() {
//   gb.setup();
// }

// void loop() {
//   bool buttonPressed = gb.readButton();
//   gb.update(buttonPressed);
// }