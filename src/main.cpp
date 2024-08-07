#include <Arduino.h>
#include <ButtonManager.h>
#include <Grandbot.h>
#include <Arp.h>
#include <Expressions.h>
#include <Setting.h>
#include <SettingManager.h>

Grandbot gb = Grandbot();
Arp arp = Arp(&gb);

void setup() {
  gb.setup();
  arp.setup();

  Serial.begin(9600);

  // SettingManager sm = SettingManager();

  // Expressions* expr = gb.getExpressionPointer();
  // expr->setting(*sm.ccOctaveOneUpChance);
}

void loop() {
  gb.getButtonManagerPointer()->read();
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
//   bool buttonPressed = gb.readButton();
//   gb.update(buttonPressed);
// }