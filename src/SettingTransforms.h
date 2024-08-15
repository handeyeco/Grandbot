#include <Arduino.h>
#include <ExpressionSets.h>
#include <Stepper.h>

#ifndef SETTING_TRANSFORMS_INCL_GUARD
#define SETTING_TRANSFORMS_INCL_GUARD


/**
 * Tranformers for manipulating setting values
 * 
 * value transforms: taking a value and converting it to a display. Ex:
 *    - "swing: 127" would get mapped to "SG:67"
 *    - "baseNoteLength: 127" would get mapped to "nL:2-"
 * step transforms: mapping 0-127 CC to discrete steps (for buttons). Ex:
 *    - for on/off settings, we want to step between 0 (off) and 127 (off) immediately
 *    - for other settings, we might want to move by 1 or move by 10 when shift is held
*/
namespace SettingTransforms {
  // shared
  void ccValueTransform(byte value, byte output[2]);
  byte ccStepTransform(byte value, bool stepUp, bool shift);
  void onOffValueTransform(byte value, byte output[2]);
  byte onOffStepTransform(byte value, bool stepUp, bool shift);

  // specialized
  // TODO a lot of these could be consolidated
  void swingValueTransform(byte value, byte output[2]);
  byte swingStepTransform(byte value, bool stepUp, bool shift);
  void midiChValueTransform(byte value, byte output[2]);
  byte midiChStepTransform(byte value, bool stepUp, bool shift);
  void noteLengthValueTransform(byte value, byte output[2]);
  byte noteLenthStepTransform(byte value, bool stepUp, bool shift);
  void sequenceLengthValueTransform(byte value, byte output[2]);
  byte sequenceLenthStepTransform(byte value, bool stepUp, bool shift);
  void collapseNotesValueTransform(byte value, byte output[2]);
  byte collapseNotesStepTransform(byte value, bool stepUp, bool shift);
};

#endif