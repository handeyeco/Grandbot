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
    bool up,
    byte stride = 1
  ) {
    float step = 127.0f / steps;
    float halfstep = step / 2.0f;
    int dirStride = stride * (up ? 1 : -1);

    int index = getSteppedIndex(value, steps);
    int nextIndex = index + dirStride;
    nextIndex = min(nextIndex, steps - 1);
    nextIndex = max(nextIndex, 0);

    float result = (nextIndex * step) + halfstep;
    float rounded = round(result);
    rounded = min(rounded, 127);
    rounded = max(rounded, 0);

    return rounded;
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