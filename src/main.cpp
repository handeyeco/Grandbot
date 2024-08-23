#include <Arduino.h>
#include <Arp.h>
#include <ButtonManager.h>
#include <Expressions.h>
#include <Grandbot.h>
#include <Setting.h>
#include <SettingManager.h>

Grandbot gb = Grandbot();
Arp arp = Arp(&gb);

void setup() {
  gb.setup();
  arp.setup();
}

void loop() {
  gb.read();
  if (!arp.update()) {
    gb.update();
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
//   gb.read();
//   gb.update();
// }