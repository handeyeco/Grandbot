#include <ButtonManager.h>
#include <Expression.h>
#include <Expressions.h>
#include <LedControl.h>
#include <Light.h>
#include <Voice.h>
#include <pin_defs.h>

#ifndef GRANDBOT_INCL_GUARD
#define GRANDBOT_INCL_GUARD

class Grandbot {
 private:
  ButtonManager buttons;
  // The voice manager
  Voice voice;
  // The RGB LED manager
  Light light;
  // The Expressions manager
  Expressions expr;

  void sleep();
  void wakeup();

  // Sleep/Wake thresholds; they're different because otherwise
  // slight variations in readings cause jitter
  // how bright it needs to be to switch from sleep to waking
  static const int wakeThresh = 200;
  // how dark it needs to be to switch from waking to sleep
  static const int sleepThresh = 100;

  // Emotional state of Grandbot
  // #TODO make byte
  // -1 = uninitialized
  //  0 = sleeping
  //  1 = happy
  //  2 = neutral
  //  3 = unhappy
  int mood = -1;

  // Esteem is a more fine measurement of mood
  // between 0 (unhappiest) and 9 (happiest)
  byte esteem = 8;

  void updateMood();
  // Last time someone pushed Grandbot's button
  unsigned long lastPlayTime = 0;

  // #TODO I think these could be ints
  // How long (ms) between interactions before an interaction is considered
  // unique this keeps people from mashing the button continuously to improve
  // mood
  static const unsigned long playThresh = 30000LL;
  // How long Grandbot has to be ignored before his esteem drops
  static const unsigned long ignoreThresh = 14200000LL;

 public:
  Grandbot();
  void play();
  void setup();
  void read();
  void update();
  ButtonManager* getButtonManagerPointer();
  Expressions* getExpressionPointer();
  Light* getLightPointer();
};

#endif