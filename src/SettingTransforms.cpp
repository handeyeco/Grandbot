#include <SettingTransforms.h>

/**
 * Default value transform
 * Since MIDI CC is 0-127 but we only have two digits for the value we map 0-127 to 0-99
 */
void SettingTransforms::ccValueTransform(byte value, byte output[2]) {
  byte index = Stepper::getSteppedIndex(value, 100);

  if (index < 10) {
    output[0] = CHAR_BLANK;
    output[1] = ExpressionSets::convertNumberToByte(index);
    return;
  }

  output[0] = ExpressionSets::convertNumberToByte((index / 10) % 10);
  output[1] = ExpressionSets::convertNumberToByte(index % 10);
}

/**
 * Default step transform
 * Since MIDI CC gets mapped from 0-127 to 0-99,
 * we also want to step by more than 1
 * (so for example: we don't need to press up twice to get to the next displayed value)
 */
byte SettingTransforms::ccStepTransform(byte value, bool stepUp, bool shift) {
  return Stepper::stepIndex(value, 100, stepUp, shift ? 10 : 1);
}

/**
 * Value transform for on/off settings
 */
void SettingTransforms::onOffValueTransform(byte value, byte output[2]) {
  output[0] = CHAR_O;

  if (value < 64) {
    output[1] = CHAR_F;
  } else {
    output[1] = CHAR_N;
  }
}

/**
 * Step transform for on/off settings
 */
byte SettingTransforms::onOffStepTransform(byte value, bool stepUp, bool shift) {
  return stepUp ? 127 : 0;
}

/**
 * Value transform for swing
 * (since MIDI 0-127 gets mapped to 50-67)
 */
void SettingTransforms::swingValueTransform(byte value, byte output[2]) {
  byte index = Stepper::getSteppedIndex(value, 18);
  byte mapped = index + 50;

  output[0] = ExpressionSets::convertNumberToByte((mapped / 10) % 10);
  output[1] = ExpressionSets::convertNumberToByte(mapped % 10);
}

/**
 * Step transfor for swing
 * 
 * TODO consolidate these transformers that are basically doing the same thing
 */
byte SettingTransforms::swingStepTransform(byte value, bool stepUp, bool shift) {
  if (shift) {
    return stepUp ? 127 : 0;
  }

  return Stepper::stepIndex(value, 18, stepUp);
}

/**
 * Value transform for MIDI channel
 * (since MIDI 0-127 gets mapped to all, 1-16)
 */
void SettingTransforms::midiChValueTransform(byte value, byte output[2]) {
  byte index = Stepper::getSteppedIndex(value, 17);
  
  // we use channel 0 to mean "no channel transform"
  if (index == 0) {
    output[0] = CHAR_A;
    output[1] = CHAR_L;
    return;
  }

  if (index < 10) {
    output[0] = CHAR_BLANK;
    output[1] = ExpressionSets::convertNumberToByte(index);
    return;
  }

  output[0] = ExpressionSets::convertNumberToByte((index / 10) % 10);
  output[1] = ExpressionSets::convertNumberToByte(index % 10);
}

/**
 * Step transform for MIDI channel
 * 
 * TODO consolidate these transformers that are basically doing the same thing
 */
byte SettingTransforms::midiChStepTransform(byte value, bool stepUp, bool shift) {
  if (shift) {
    return stepUp ? 127 : 0;
  }

  return Stepper::stepIndex(value, 17, stepUp);
}

/**
 * Value transform for note length
 * (since MIDI 0-127 gets mapped to various different note lengths)
 */
void SettingTransforms::noteLengthValueTransform(byte value, byte output[2]) {
  byte index = Stepper::getSteppedIndex(value, 7);

  if (index == 6) {
    // double whole
    output[0] = CHAR_2;
    output[1] = CHAR_DASH;
  }
  else if (index == 5) {
    // whole
    output[0] = CHAR_1;
    output[1] = CHAR_DASH;
  }
  else if (index == 4) {
    // half
    output[0] = CHAR_H;
    output[1] = CHAR_A;
  }
  else if (index == 3) {
    // quarter
    output[0] = CHAR_BLANK;
    output[1] = CHAR_1;
  }
  else if (index == 2) {
    // 8th
    output[0] = CHAR_BLANK;
    output[1] = CHAR_8;
  }
  else if (index == 1) {
    // 16th
    output[0] = CHAR_1;
    output[1] = CHAR_6;
  }
  else {
    // random
    output[0] = CHAR_R;
    output[1] = CHAR_A;
  }
}

/**
 * Step transform for note length
 * 
 * TODO consolidate these transformers that are basically doing the same thing
 */
byte SettingTransforms::noteLenthStepTransform(byte value, bool stepUp, bool shift) {
  if (shift) {
    return stepUp ? 127 : 0;
  }

  return Stepper::stepIndex(value, 7, stepUp);
}

/**
 * Value transform for sequence length
 * (since MIDI 0-127 gets mapped to random, 1-8)
 */
void SettingTransforms::sequenceLengthValueTransform(byte value, byte output[2]) {
  byte index = Stepper::getSteppedIndex(value, 9);

  // Random
  if (index == 0) {
    output[0] = CHAR_R;
    output[1] = CHAR_A;
    return;
  }
  else {
    output[0] = CHAR_BLANK;
    output[1] = ExpressionSets::convertNumberToByte(index);
  }
}

/**
 * Step transform for sequence length
 * 
 * TODO consolidate these transformers that are basically doing the same thing
 */
byte SettingTransforms::sequenceLenthStepTransform(byte value, bool stepUp, bool shift) {
  if (shift) {
    return stepUp ? 127 : 0;
  }

  return Stepper::stepIndex(value, 9, stepUp);
}

/**
 * Value transform for collapse notes
 * (since MIDI 0-127 gets mapped to none, start, or end)
 */
void SettingTransforms::collapseNotesValueTransform(byte value, byte output[2]) {
  byte index = Stepper::getSteppedIndex(value, 3);

  // No collapse
  if (index == 0) {
    output[0] = CHAR_N;
    output[1] = CHAR_O;
  }
  // Play notes at the start
  else if (index == 1) {
    output[0] = CHAR_S;
    output[1] = CHAR_T;
  }
  // Play notes at the end
  else {
    output[0] = CHAR_E;
    output[1] = CHAR_N;
  }
}

/**
 * Step transform for collapse notes
 * 
 * TODO consolidate these transformers that are basically doing the same thing
 */
byte SettingTransforms::collapseNotesStepTransform(byte value, bool stepUp, bool shift) {
  if (shift) {
    return stepUp ? 127 : 0;
  }

  return Stepper::stepIndex(value, 3, stepUp);
}