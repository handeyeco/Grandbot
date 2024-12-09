#include <Setting.h>

Setting::Setting(byte _defaultValue,
                 byte _zeroValue,
                 byte _midiCC,
                 byte firstDisplayChar,
                 byte secondDisplayChar,
                 byte _numOfOptions,
                 void (&_valueTransform)(Setting& self, byte output[4]),
                 byte (&_stepTransform)(Setting& self, byte value, bool stepUp, bool shift),
                 byte (&_randomizeValue)(),
                 bool _usesColon)
    : value(_defaultValue),
      zeroValue(_zeroValue),
      defaultValue(_defaultValue),
      midiCC(_midiCC),
      numOfOptions(_numOfOptions),
      valueTransform(_valueTransform),
      stepTransform(_stepTransform),
      randomizeValue(_randomizeValue),
      usesColon(_usesColon) {
  // TODO ideally this would be passed in as an array,
  // but I couldn't figure it out because C++ is big brain programming
  nameDisplay[0] = firstDisplayChar;
  nameDisplay[1] = secondDisplayChar;
}

/**
 * Converts a MIDI CC value (0-127) to a boolean
 *
 * @param {byte} value - CC value
 * @returns {boolean} if CC is above threshold
 */
bool Setting::convertCCToBool(byte cc) {
  // MIDI CC is 0-127, so is the CC greater than half?
  return cc > 64;
}

/**
 * Gets an array of bytes that is sent to the 4D7S
 * to be displayed when viewing/updating a setting
 *
 * @param {byte[]} output - MIDI channel in question
 */
void Setting::getDisplay(byte output[4]) {
  valueTransform(*this, output);
}

byte Setting::getValue() {
  return value;
}

bool Setting::getValueAsBool() {
  return convertCCToBool(value);
};

void Setting::setValue(byte nextValue) {
  value = nextValue;
}

byte Setting::stepIndex(byte value, bool up, byte stride = 1) {
  return Stepper::stepIndex(value, numOfOptions, up, stride);
}

byte Setting::getSteppedIndex() {
  // 0 = no stepping
  if (numOfOptions == 0)
    return value;
  Stepper::getSteppedIndex(value, numOfOptions);
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
  if ((stepUp && value >= 127) || (!stepUp && value <= 0))
    return;

  setValue(stepTransform(*this, value, stepUp, shift));
};

void Setting::randomize() {
  setValue(randomizeValue());
}

void Setting::reset() {
  setValue(zeroValue);
}

/**
 * Dice roll for chance values
 *
 * @returns {bool} whether the roll was successful
 */
bool Setting::roll() {
  if (getValue() == 0)
    return false;
  return random(127) < getValue();
}