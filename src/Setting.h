#include <Arduino.h>
#include <Stepper.h>

#ifndef SETTING_INCL_GUARD
#define SETTING_INCL_GUARD

/**
 * An individual setting
 */
struct Setting {
 private:
  // current value
  byte value = 0;
  // what's the "doing nothing interesting" state
  byte zeroValue = 0;
  // smart defaults to get started making sequences
  // TODO: consider getting rid of this now that reset == zeroValue
  byte defaultValue = 0;
  // settings are stored between 0-127, but they might be broken into finite
  // steps
  byte numOfOptions = 0;
  // value transform (map name/value to display)
  void (&valueTransform)(Setting& self, byte output[4]);
  // handle setting value with buttons
  byte (&stepTransform)(Setting& self, byte value, bool stepUp, bool shift);
  // callback to randomize setting
  byte (&randomizeValue)();

 public:
  Setting(byte defaultValue,
          byte zeroValue,
          byte midiCC,
          byte firstDisplayChar,
          byte secondDisplayChar,
          byte _numOfOptions,
          void (&valueTransform)(Setting& self, byte output[4]),
          byte (&stepTransform)(Setting& self, byte value, bool stepUp, bool shift),
          byte (&randomizeValue)(),
          bool usesColon = true);

  static bool convertCCToBool(byte cc);

  // display name
  byte(nameDisplay)[2] = {};
  void getDisplay(byte output[4]);
  // whether to show colon when showing name/value
  bool usesColon;

  // if this responds to MIDI CC,
  // which MIDI CC number (255 = none)
  byte midiCC;

  byte getValue();
  void setValue(byte nextValue);
  byte stepIndex(byte value, bool up, byte stride = 1);
  byte getSteppedIndex();
  void step(bool stepUp, bool shift);
  void randomize();
  void reset();
  bool roll();
  bool getValueAsBool();
};

#endif