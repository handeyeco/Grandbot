#include <SequenceStep.h>

SequenceStep::SequenceStep() {}

SequenceStep::SequenceStep(byte _noteIndex, uint16_t _noteStartPosition) {
  noteIndex = _noteIndex;
  noteStartPosition = _noteStartPosition;
}

void SequenceStep::reset() {
  noteIndex = 0;
  noteStartPosition = 0;
}