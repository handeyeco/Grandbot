#include <Arduino.h>

#ifndef SETTING_INCL_GUARD
#define SETTING_INCL_GUARD


/**
 * An individual setting
*/
struct Setting {
  private:
    // current value
    byte value = 0;
    // start / reset value
    byte defaultValue = 0;
    // value transform (map value to display)
    void (&valueTransform)(Setting &self, byte output[4]);
    // handle setting value with buttons
    byte (&stepTransform)(byte value, bool stepUp, bool shift);
  public:
    Setting(
      byte defaultValue,
      byte midiCC,
      byte firstDisplayChar,
      byte secondDisplayChar,
      void (&valueTransform)(Setting &self, byte output[4]),
      byte (&setValueStepped)(byte value, bool stepUp, bool shift),
      bool usesColon = true);

    // display name
    byte (nameDisplay)[2] = {};
    void getDisplay(byte output[4]);
    bool usesColon;

    // if this responds to MIDI CC,
    // which MIDI CC number (255 = none)
    byte midiCC;

    byte getValue();
    void setValue(byte nextValue);
    void step(bool stepUp, bool shift);
    bool roll();
};

#endif