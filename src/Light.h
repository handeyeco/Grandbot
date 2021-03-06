#include <Arduino.h>

#ifndef LIGHT_INCL_GUARD
#define LIGHT_INCL_GUARD

class Light {
  private:
    int redPin;
    int greenPin;
    int bluePin;

    // Use all this for color transition
    int nextR = 0;
    int nextG = 0;
    int nextB = 0;
    int prevR = 0;
    int prevG = 0;
    int prevB = 0;
    int rDelta;
    int gDelta;
    int bDelta;

    boolean animating = false;
    static const int tweenLength = 1000;
    unsigned long startTween;

    void write(int rVal, int gVal, int bVal);
  public:
    Light(int rPin, int gPin, int bPin);
    void update(int mood);
    void setColor(int mood);
};

#endif