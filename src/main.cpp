#include <Arduino.h>
#include <ButtonManager.h>
#include <Grandbot.h>
#include <Arp.h>
#include <Expressions.h>
#include <Setting.h>

Grandbot gb = Grandbot();
Arp arp = Arp(&gb);

void defaultValueTransform(byte value, byte output[2]) {
  output[0] = CHAR_F;
  output[1] = CHAR_F;
}

void setup() {
  gb.setup();
  arp.setup();

  Serial.begin(9600);

  byte ccDisplay[2] = {CHAR_O, CHAR_O};
  Setting s = Setting(0, 255, ccDisplay, defaultValueTransform);

  byte fullDisplay[4] = {};
  s.getDisplay(fullDisplay);
  for (int i = 0; i < 4; i++) {
    Serial.println(fullDisplay[i]);
  }

  Expressions* expr = gb.getExpressionPointer();
  expr->setting(s);
}

void loop() {
  // gb.getButtonManagerPointer()->read();
  // if (!arp.update()) {
  //   gb.update();
  // }
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