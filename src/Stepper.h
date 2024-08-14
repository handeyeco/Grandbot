#include <Arduino.h>

#ifndef STEPPER_INCL_GUARD
#define STEPPER_INCL_GUARD

/**
 * Helper to help with mapping MIDI CC (0-127) to discrete steps
 */
struct Stepper {
  /**
   * Map a MIDI CC to an index value based
   * the number of steps required. Ex:
   *    - getSteppedIndex(0, 2) == 0
   *    - getSteppedIndex(60, 2) == 0
   *    - getSteppedIndex(70, 2) == 1
   *    - getSteppedIndex(127, 2) == 1
   *    - getSteppedIndex(0, 4) == 0
   *    - getSteppedIndex(60, 4) == 1
   *    - getSteppedIndex(70, 4) == 2
   *    - getSteppedIndex(127, 4) == 3
   * 
   * @param {byte} value - the MIDI CC value
   * @param {byte} steps - the number of discrete steps to map
   * @return {byte} the index of the value mapped to the number of steps
   */
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


  /**
   * Move up/down through discrete steps in a MIDI CC range.
   * Finds the current index, determines a delta, finds the new index,
   * and maps the new index to 0-127
   * 
   * @param {byte} value - the MIDI CC value
   * @param {byte} steps - total number of discrete steps
   * @param {bool} up - movement direction: increment (true), decrement (false)
   * @param {byte} stride - number of steps to move
   * @return {byte} the new value after stepping/clamping
   */
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
};

#endif