#include <SettingTransforms.h>

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

byte SettingTransforms::ccStepTransform(byte value, bool stepUp, bool shift) {
  return Stepper::stepIndex(value, 100, stepUp, shift ? 10 : 1);
}

void SettingTransforms::swingValueTransform(byte value, byte output[2]) {
  byte index = Stepper::getSteppedIndex(value, 18);
  byte mapped = index + 50;

  output[0] = ExpressionSets::convertNumberToByte((mapped / 10) % 10);
  output[1] = ExpressionSets::convertNumberToByte(mapped % 10);
}

byte SettingTransforms::swingStepTransform(byte value, bool stepUp, bool shift) {
  if (shift) {
    return stepUp ? 127 : 0;
  }

  return Stepper::stepIndex(value, 18, stepUp);
}

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

byte SettingTransforms::midiChStepTransform(byte value, bool stepUp, bool shift) {
  if (shift) {
    return stepUp ? 127 : 0;
  }

  return Stepper::stepIndex(value, 17, stepUp);
}

void SettingTransforms::onOffValueTransform(byte value, byte output[2]) {
  output[0] = CHAR_O;

  if (value < 64) {
    output[1] = CHAR_F;
  } else {
    output[1] = CHAR_N;
  }
}

byte SettingTransforms::onOffStepTransform(byte value, bool stepUp, bool shift) {
  return stepUp ? 127 : 0;
}

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

byte SettingTransforms::noteLenthStepTransform(byte value, bool stepUp, bool shift) {
  if (shift) {
    return stepUp ? 127 : 0;
  }

  return Stepper::stepIndex(value, 7, stepUp);
}

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

byte SettingTransforms::sequenceLenthStepTransform(byte value, bool stepUp, bool shift) {
  if (shift) {
    return stepUp ? 127 : 0;
  }

  return Stepper::stepIndex(value, 9, stepUp);
}