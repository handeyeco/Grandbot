#include <Arduino.h>

#ifndef STEPPER_INCL_GUARD
#define STEPPER_INCL_GUARD

struct Stepper {
  byte static getSteppedIndex(
    byte value,
    byte steps
  ) {
    float step = 127.0f / steps;

    for (byte i = 0; i < steps; i++) {
      float thresh = (i + 1) * step;
      if (value <= thresh) {
        return i;
      }
    }

    return 255;
  }

  byte static stepIndex(
    byte value,
    byte steps,
    bool up
  ) {
    float step = 127.0f / steps;
    float halfstep = step / 2.0f;
    byte index = getSteppedIndex(value, steps);

    if (!up && index <= 0) {
      return halfstep;
    } else if (up && index >= steps - 1) {
      return (step * (steps - 1)) + halfstep;
    }

    byte nextIndex = index + (up ? 1 : -1);
    return (nextIndex * step) + halfstep;
  }

  byte static stepFloor(
    byte index,
    byte steps
  ) {
    float step = 127.0f / steps;
    return (index - 1) * step;
  }
};

#endif