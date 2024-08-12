#include <Arduino.h>
#include <ExpressionSets.h>
#include <Stepper.h>

#ifndef SETTING_TRANSFORMS_INCL_GUARD
#define SETTING_TRANSFORMS_INCL_GUARD


/**
 * Tranformers for manipulating setting values
 * 
 * value transforms: taking a value and converting it to a display
 * step transforms: mapping 0-127 CC to discrete steps (for buttons)
*/
namespace SettingTransforms {
  // shared
  void ccValueTransform(byte value, byte output[2]);
  byte ccStepTransform(byte value, bool stepUp, bool shift);
  void onOffValueTransform(byte value, byte output[2]);
  byte onOffStepTransform(byte value, bool stepUp, bool shift);

  // specialized
  void swingValueTransform(byte value, byte output[2]);
  byte swingStepTransform(byte value, bool stepUp, bool shift);
  void midiChValueTransform(byte value, byte output[2]);
  byte midiChStepTransform(byte value, bool stepUp, bool shift);
  void noteLengthValueTransform(byte value, byte output[2]);
  byte noteLenthStepTransform(byte value, bool stepUp, bool shift);
  void sequenceLengthValueTransform(byte value, byte output[2]);
  byte sequenceLenthStepTransform(byte value, bool stepUp, bool shift);
};

#endif