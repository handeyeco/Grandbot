#include <Arduino.h>

#ifndef SEQ_STEP_INCL_GUARD
#define SEQ_STEP_INCL_GUARD

struct SequenceStep {
  public:
    SequenceStep();
    SequenceStep(byte _noteIndex, uint16_t _noteStartPosition);
    void reset();
    byte noteIndex = 0;
    // Position in sequence in pulses
    uint16_t noteStartPosition = 0;
};

#endif