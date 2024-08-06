#include <Arduino.h>
#include <ButtonManager.h>
#include <Grandbot.h>
#include <Arp.h>

ButtonManager buttons = ButtonManager();
Grandbot gb = Grandbot();
Arp arp = Arp(&gb);

void setup() {
  gb.setup();
  arp.setup();

  Serial.begin(9600);
}

void loop() {
  buttons.read();
  bool playButtonPressed = buttons.play.pressed;
  if (!arp.update(playButtonPressed)) {
    gb.update(playButtonPressed);
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