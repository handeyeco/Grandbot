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
  // holy fuck I hate C++ why can't I pass a fucking array
  nameDisplay[0] = firstDisplayChar;
  nameDisplay[1] = secondDisplayChar;
}

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