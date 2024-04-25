#include <Arduino.h>

#ifndef LIGHT_INCL_GUARD
#define LIGHT_INCL_GUARD

class Light {
  private:
    byte redPin;
    byte greenPin;
    byte bluePin;

    // copy of the mood
    // #TODO remove redundancy
    int mood;

    // In order to tween colors,
    // we need to keep track of the
    // prev/next states
    byte nextR = 0;
    byte nextG = 0;
    byte nextB = 0;
    byte prevR = 0;
    byte prevG = 0;
    byte prevB = 0;

    // Whether we're actively animating
    bool animating = false;
    // Animation time
    static const int tweenLength = 1000;
    // Time we started current animation
    unsigned long startTween;

    void write(byte rVal, byte gVal, byte bVal);
  public:
    Light(byte rPin, byte gPin, byte bPin);
    void update();
    void setColor(int mood);
    void setMood(int mood);

    // ARP STUFF #TODO move this
    void midiBeat(bool beat);
};

#endif