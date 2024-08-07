#include <Setting.h>

byte defaultDisplay[2] = {};
void defaultValueTransform(byte value, byte output[2]) {}

Setting::Setting() :
  nameDisplay(defaultDisplay),
  valueTransform(defaultValueTransform) {}

Setting::Setting(
  byte _defaultValue,
  byte _midiCC,
  byte (&_nameDisplay)[2],
  void (&_valueTransform)(byte value, byte output[2])) : 
    defaultValue(_defaultValue),
    midiCC(_midiCC),
    nameDisplay(_nameDisplay),
    valueTransform(_valueTransform)
{
  value = _defaultValue;
}

void Setting::getDisplay(byte output[4]) {
  output[0] = nameDisplay[0];
  output[1] = nameDisplay[1];

  byte valueBytes[2] = {};
  valueTransform(value, valueBytes);
  output[2] = valueBytes[0];
  output[3] = valueBytes[1];
}