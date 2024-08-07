#include <Setting.h>

Setting::Setting(
      byte _defaultValue,
      byte _midiCC,
      byte (&_ccDisplay)[2],
      void (&_valueTransform)(byte value, byte output[2])) : 
      defaultValue(_defaultValue), midiCC(_midiCC), ccDisplay(_ccDisplay), valueTransform(_valueTransform)
{
  value = _defaultValue;
  Serial.println(ccDisplay[0]);
  Serial.println(ccDisplay[1]);
}

void Setting::getDisplay(byte output[4]) {
  output[0] = ccDisplay[0];
  output[1] = ccDisplay[1];

  byte valueBytes[2] = {};
  valueTransform(value, valueBytes);
  output[2] = valueBytes[0];
  output[3] = valueBytes[1];
}