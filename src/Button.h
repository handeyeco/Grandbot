#include <Arduino.h>

#ifndef BUTTON_INCL_GUARD
#define BUTTON_INCL_GUARD


/**
 * State manager for an individual button
*/
struct Button {
  private:
    int pin;
    bool prevPressed;
  public:
    Button(int pin);
    void read();
    
    // did it just get pressed
    bool pressed;
    // did it just get released
    bool released;
    // are we between a press and a release
    bool held;
    // disable release after a combo or press event
    bool ignoreRelease;
};

#endif