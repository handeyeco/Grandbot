#include <SettingTransforms.h>

/**
 * Most settings are displayed as `nn:vv`, so this gets the name part sorted
 */
void SettingTransforms::populateName(Setting& self, byte output[4]) {
  output[0] = self.nameDisplay[0];
  output[1] = self.nameDisplay[1];
}

/**
 * Default value transform
 * Since MIDI CC is 0-127 but we only have two digits for the value we map 0-127
 * to 0-99
 */
void SettingTransforms::ccValueTransform(Setting& self, byte output[4]) {
  populateName(self, output);
  byte index = Stepper::getSteppedIndex(self.getValue(), 100);

  if (index < 10) {
    output[2] = CHAR_BLANK;
    output[3] = ExpressionSets::convertNumberToByte(index);
    return;
  }

  output[2] = ExpressionSets::convertNumberToByte((index / 10) % 10);
  output[3] = ExpressionSets::convertNumberToByte(index % 10);
}

/**
 * Default step transform
 * Since MIDI CC gets mapped from 0-127 to 0-99,
 * we also want to step by more than 1
 * (so for example: we don't need to press up twice to get to the next displayed
 * value)
 */
byte SettingTransforms::ccStepTransform(byte value, bool stepUp, bool shift) {
  return Stepper::stepIndex(value, 100, stepUp, shift ? 10 : 1);
}

/**
 * Value transform for on/off settings
 */
void SettingTransforms::onOffValueTransform(Setting& self, byte output[4]) {
  populateName(self, output);
  output[2] = CHAR_O;

  if (self.getValue() < 64) {
    output[3] = CHAR_F;
  } else {
    output[3] = CHAR_N;
  }
}

/**
 * Step transform for on/off settings
 */
byte SettingTransforms::onOffStepTransform(byte value,
                                           bool stepUp,
                                           bool shift) {
  return stepUp ? 127 : 0;
}

/**
 * Value transform for MIDI clock
 * (since MIDI 0-127 gets mapped to external / internal)
 */
void SettingTransforms::clockValueTransform(Setting& self, byte output[4]) {
  populateName(self, output);
  // external clock
  if (self.getValue() < 64) {
    output[2] = CHAR_E;
    output[3] = CHAR_T;
  }
  // internal clock
  else {
    output[2] = CHAR_I;
    output[3] = CHAR_N;
  }
}

/**
 * Value transform for BPM
 * (since MIDI 0-127 gets mapped to 73-200)
 */
void SettingTransforms::bpmValueTransform(Setting& self, byte output[4]) {
  output[0] = CHAR_B;

  // TODO: this should use BPM_OFFSET
  byte mapped = self.getValue() + 73;

  if (mapped < 100) {
    output[1] = CHAR_BLANK;
    output[2] = ExpressionSets::convertNumberToByte((mapped / 10) % 10);
    output[3] = ExpressionSets::convertNumberToByte(mapped % 10);
    return;
  }

  output[1] = ExpressionSets::convertNumberToByte((mapped / 100) % 10);
  output[2] = ExpressionSets::convertNumberToByte((mapped / 10) % 10);
  output[3] = ExpressionSets::convertNumberToByte(mapped % 10);
}

/**
 * Step transfor for BPM
 */
byte SettingTransforms::bpmStepTransform(byte value, bool stepUp, bool shift) {
  int step = (shift ? 10 : 1) * (stepUp ? 1 : -1);
  int next = value + step;
  next = max(0, next);
  next = min(127, next);

  return next;
}

/**
 * Value transform for swing
 * (since MIDI 0-127 gets mapped to 50-67)
 */
void SettingTransforms::swingValueTransform(Setting& self, byte output[4]) {
  populateName(self, output);
  byte index = Stepper::getSteppedIndex(self.getValue(), 18);
  byte mapped = index + 50;

  output[2] = ExpressionSets::convertNumberToByte((mapped / 10) % 10);
  output[3] = ExpressionSets::convertNumberToByte(mapped % 10);
}

/**
 * Step transfor for swing
 *
 * TODO consolidate these transformers that are basically doing the same thing
 */
byte SettingTransforms::swingStepTransform(byte value,
                                           bool stepUp,
                                           bool shift) {
  if (shift) {
    return stepUp ? 127 : 0;
  }

  return Stepper::stepIndex(value, 18, stepUp);
}

/**
 * Value transform for MIDI channel
 * (since MIDI 0-127 gets mapped to all, 1-16)
 */
void SettingTransforms::midiChValueTransform(Setting& self, byte output[4]) {
  populateName(self, output);
  byte index = Stepper::getSteppedIndex(self.getValue(), 17);

  // we use channel 0 to mean "no channel transform"
  if (index == 0) {
    output[2] = CHAR_A;
    output[3] = CHAR_L;
    return;
  }

  if (index < 10) {
    output[2] = CHAR_BLANK;
    output[3] = ExpressionSets::convertNumberToByte(index);
    return;
  }

  output[2] = ExpressionSets::convertNumberToByte((index / 10) % 10);
  output[3] = ExpressionSets::convertNumberToByte(index % 10);
}

/**
 * Step transform for MIDI channel
 *
 * TODO consolidate these transformers that are basically doing the same thing
 */
byte SettingTransforms::midiChStepTransform(byte value,
                                            bool stepUp,
                                            bool shift) {
  if (shift) {
    return stepUp ? 127 : 0;
  }

  return Stepper::stepIndex(value, 17, stepUp);
}

/**
 * Value transform for note length
 * (since MIDI 0-127 gets mapped to various different note lengths)
 */
void SettingTransforms::noteLengthValueTransform(Setting& self,
                                                 byte output[4]) {
  populateName(self, output);
  byte index = Stepper::getSteppedIndex(self.getValue(), 7);

  if (index == 6) {
    // double whole
    output[2] = CHAR_2;
    output[3] = CHAR_DASH;
  } else if (index == 5) {
    // whole
    output[2] = CHAR_1;
    output[3] = CHAR_DASH;
  } else if (index == 4) {
    // half
    output[2] = CHAR_H;
    output[3] = CHAR_A;
  } else if (index == 3) {
    // quarter
    output[2] = CHAR_BLANK;
    output[3] = CHAR_1;
  } else if (index == 2) {
    // 8th
    output[2] = CHAR_BLANK;
    output[3] = CHAR_8;
  } else if (index == 1) {
    // 16th
    output[2] = CHAR_1;
    output[3] = CHAR_6;
  } else {
    // random
    output[2] = CHAR_R;
    output[3] = CHAR_A;
  }
}

/**
 * Step transform for note length
 *
 * TODO consolidate these transformers that are basically doing the same thing
 */
byte SettingTransforms::noteLenthStepTransform(byte value,
                                               bool stepUp,
                                               bool shift) {
  if (shift) {
    return stepUp ? 127 : 0;
  }

  return Stepper::stepIndex(value, 7, stepUp);
}

/**
 * Value transform for sequence length
 * (since MIDI 0-127 gets mapped to random, 1-8)
 */
void SettingTransforms::sequenceLengthValueTransform(Setting& self,
                                                     byte output[4]) {
  populateName(self, output);
  byte index = Stepper::getSteppedIndex(self.getValue(), 9);

  // Random
  if (index == 0) {
    output[2] = CHAR_R;
    output[3] = CHAR_A;
    return;
  } else {
    output[2] = CHAR_BLANK;
    output[3] = ExpressionSets::convertNumberToByte(index);
  }
}

/**
 * Step transform for sequence length
 *
 * TODO consolidate these transformers that are basically doing the same thing
 */
byte SettingTransforms::sequenceLenthStepTransform(byte value,
                                                   bool stepUp,
                                                   bool shift) {
  if (shift) {
    return stepUp ? 127 : 0;
  }

  return Stepper::stepIndex(value, 9, stepUp);
}

/**
 * Value transform for collapse notes
 * (since MIDI 0-127 gets mapped to none, start, or end)
 */
void SettingTransforms::collapseNotesValueTransform(Setting& self,
                                                    byte output[4]) {
  populateName(self, output);
  byte index = Stepper::getSteppedIndex(self.getValue(), 3);

  // No collapse
  if (index == 0) {
    output[2] = CHAR_N;
    output[3] = CHAR_O;
  }
  // Play notes at the start
  else if (index == 1) {
    output[2] = CHAR_S;
    output[3] = CHAR_T;
  }
  // Play notes at the end
  else {
    output[2] = CHAR_E;
    output[3] = CHAR_N;
  }
}

/**
 * Step transform for collapse notes
 *
 * TODO consolidate these transformers that are basically doing the same thing
 */
byte SettingTransforms::collapseNotesStepTransform(byte value,
                                                   bool stepUp,
                                                   bool shift) {
  if (shift) {
    return stepUp ? 127 : 0;
  }

  return Stepper::stepIndex(value, 3, stepUp);
}