#include <Arduino.h>

#ifndef LIGHT_INCL_GUARD
#define LIGHT_INCL_GUARD

class Light {
  private:
    byte redPin;
    byte greenPin;
    byte bluePin;

    int mood;

    // Use all this for color transition
    byte nextR = 0;
    byte nextG = 0;
    byte nextB = 0;
    byte prevR = 0;
    byte prevG = 0;
    byte prevB = 0;
    int rDelta;
    int gDelta;
    int bDelta;

    boolean animating = false;
    static const int tweenLength = 1000;
    unsigned long startTween;

    void write(byte rVal, byte gVal, byte bVal);
  public:
    Light(byte rPin, byte gPin, byte bPin);
    void update();
    void setColor(int mood);
    void setMood(int mood);
    void midiBeat(bool beat);
};

#endif