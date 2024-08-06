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
    bool pressed;
    bool released;
};

#endif