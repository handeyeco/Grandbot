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
  void read();

  // used to debounce
  uint8_t history;
  // did it just get pressed
  bool pressed;
  // did it just get released
  bool released;
  // are we between a press and a release
  bool held;
  // disable release after a combo or press event
  bool ignoreRelease;
};

#endif