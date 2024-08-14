#include <Setting.h>

Setting::Setting(
  byte _defaultValue,
  byte _midiCC,
  byte firstDisplayChar,
  byte secondDisplayChar,
  void (&_valueTransform)(byte value, byte output[2]),
  byte (&_stepTransform)(byte value, bool stepUp, bool shift)) :
    value(_defaultValue), 
    defaultValue(_defaultValue),
    midiCC(_midiCC),
    valueTransform(_valueTransform),
    stepTransform(_stepTransform)
{
  // TODO ideally this would be passed in as an array,
  // but I couldn't figure it out because C++ is big brain programming
  nameDisplay[0] = firstDisplayChar;
  nameDisplay[1] = secondDisplayChar;
}

/**
 * Gets an array of bytes that is sent to the 4D7S
 * to be displayed when viewing/updating a setting
 *
 * @param {byte[]} output - MIDI channel in question
*/
void Setting::getDisplay(byte output[4]) {
  output[0] = nameDisplay[0];
  output[1] = nameDisplay[1];

  byte valueBytes[2] = {};
  valueTransform(value, valueBytes);
  output[2] = valueBytes[0];
  output[3] = valueBytes[1];
}

byte Setting::getValue() {
  return value;
}

void Setting::setValue(byte nextValue) {
  value = nextValue;
}

/**
 * Update the setting using buttons (rather than MIDI CC).
 * Since not all settings are linear, we transform them
 * to something that makes sense to the specific setting.
 *
 * @param {bool} stepUp - whether we're stepping up (true) or down (false)
 * @param {bool} shift - whether the shift (forward) buttons was held
*/
void Setting::step(bool stepUp, bool shift) {
  if ((stepUp && value >= 127) || (!stepUp && value <= 0)) return;

  setValue(stepTransform(value, stepUp, shift));
};

/**
 * Dice roll for chance values
 *
 * @returns {bool} whether the roll was successful
*/
bool Setting::roll() {
  return random(128) < getValue();
}