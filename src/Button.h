#include <Arduino.h>

#ifndef BUTTON_INCL_GUARD
#define BUTTON_INCL_GUARD

#define BUTTON_MASK 0b11000111

/**
 * State manager for an individual button
 */
struct Button {
 private:
  int pin;

 public:
  Button(int pin);
  uint8_t history;
  bool ignoreRelease = false;

  bool pressed;
  bool held;
  bool released;
  void update();
};

#endif