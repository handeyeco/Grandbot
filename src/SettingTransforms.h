#include <Arduino.h>
#include <ExpressionSets.h>
#include <Setting.h>
#include <Stepper.h>

#ifndef SETTING_TRANSFORMS_INCL_GUARD
#define SETTING_TRANSFORMS_INCL_GUARD

/**
 * Tranformers for manipulating setting values
 *
 * value transforms: taking a name/value and converting it to a display. Ex:
 *    - "swing: 127" would get mapped to "SG:67"
 *    - "baseNoteLength: 127" would get mapped to "nL:2-"
 *    - "bpm: 127" would get mapped to "b200"
 * step transforms: mapping 0-127 CC to discrete steps (for buttons). Ex:
 *    - for on/off settings, we want to step between 0 (off) and 127 (off)
 * immediately
 *    - for other settings, we might want to move by 1 or move by 10 when shift
 * is held
 */
namespace SettingTransforms {
// helpers
void populateName(Setting &self, byte output[4]);

// shared
void ccValueTransform(Setting &self, byte output[4]);
byte ccStepTransform(byte value, bool stepUp, bool shift);
void onOffValueTransform(Setting &self, byte output[4]);
byte onOffStepTransform(byte value, bool stepUp, bool shift);

// specialized
// TODO a lot of these could be consolidated
void clockValueTransform(Setting &self, byte output[4]);
void bpmValueTransform(Setting &self, byte output[4]);
byte bpmStepTransform(byte value, bool stepUp, bool shift);
void swingValueTransform(Setting &self, byte output[4]);
byte swingStepTransform(byte value, bool stepUp, bool shift);
void midiChValueTransform(Setting &self, byte output[4]);
byte midiChStepTransform(byte value, bool stepUp, bool shift);
void noteLengthValueTransform(Setting &self, byte output[4]);
byte noteLenthStepTransform(byte value, bool stepUp, bool shift);
void sequenceLengthValueTransform(Setting &self, byte output[4]);
byte sequenceLenthStepTransform(byte value, bool stepUp, bool shift);
void collapseNotesValueTransform(Setting &self, byte output[4]);
byte collapseNotesStepTransform(byte value, bool stepUp, bool shift);
}; // namespace SettingTransforms

#endif