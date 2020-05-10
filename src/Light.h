#include <Arduino.h>

#ifndef LIGHT_INCL_GUARD
#define LIGHT_INCL_GUARD

class Light {
  private:
    int red;
    int green;
    int blue;

    void write(int rValue, int gValue, int bValue);
  public:
    Light(int redPin, int greenPin, int bluePin);
    void update(int state);
};

#endif