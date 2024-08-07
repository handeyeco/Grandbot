#include <Arduino.h>
#include <LedControl.h>

#ifndef SETTING_INCL_GUARD
#define SETTING_INCL_GUARD


/**
 * An individual setting
*/
struct Setting {
  private:
    // start / reset value
    byte defaultValue = 0;
    // display name
    byte (&ccDisplay)[2];
    // value transform (map value to display)
    void (&valueTransform)(byte value, byte output[2]);
  public:
    Setting(
      byte defaultValue,
      byte midiCC,
      byte (&ccDisplay)[2],
      void (&valueTransform)(byte value, byte output[2]));
    void getDisplay(byte output[4]);

    // if this responds to MIDI CC,
    // which MIDI CC number (255 = none)
    byte midiCC;
    // current value
    byte value = 0;
};

#endif