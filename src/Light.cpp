#include <Light.h>

Light::Light(byte rPin, byte gPin, byte bPin) {
  redPin = rPin;
  greenPin = gPin;
  bluePin = bPin;

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

/**
 * Write color to LED
 *
 * @param {byte} rVal - red color value
 * @param {byte} gVal - green color value
 * @param {byte} bVal - blue color value
*/
void Light::write(byte rVal, byte gVal, byte bVal) {
  analogWrite(redPin, rVal);
  analogWrite(greenPin, gVal);
  analogWrite(bluePin, bVal);
}

/**
 * Set color to animate to
 *
 * @param {int} mood - enum for emotional state as defined in Grandbot.h
*/
void Light::setColor(int mood) {
  prevR = nextR;
  prevG = nextG;
  prevB = nextB;

  // turn off for sleep
  if (mood == 0) {
    nextR = 0;
    nextG = 0;
    nextB = 0;
  }
  // only show red when unhappen
  else if (mood == 3) {
    nextR = 255;
    nextG = 0;
    nextB = 0;
  }
  // otherwise a random color
  else {
    nextR = random(0, 256);
    nextG = random(0, 256);
    nextB = random(0, 256);
  }

  // trigger animation
  animating = true;
  startTween = millis();
}

/**
 * Store a local copy of mood
 * #TODO remove redundancy 
 *
 * @param {int} mood - enum for emotional state as defined in Grandbot.h
*/
void Light::setMood(int mood) {
  this->mood = mood;
}

/**
 * Light show (for Arp)
 * flips between cyan and magenta without animation
 * #TODO move this
 *
 * @param {bool} even - whether we're on an odd/even quarter note
*/
void Light::midiBeat(bool even) {
  byte r = even ? 0 : 255;
  byte g = even ? 255 : 0;
  byte b = even ? 255 : 255;
  write(r, g, b);
}

/**
 * Update to be called during the Arduino update cycle.
 * Handles animating the LED.
*/
void Light::update() {
  if (!animating) return;

  unsigned long now = millis();
  if (now - startTween > tweenLength) {
    if (mood == 1) {
      // Keep swirling the lights if he's happy
      setColor(mood);
    } else {
      // Otherwise stop when the target color is reached
      write(nextR, nextG, nextB);
      animating = false;
      return;
    }
  }

  int elapsed = now - startTween;
  float progress = min((float) elapsed / (float) tweenLength, 1);

  int rDelta = nextR - prevR;
  int gDelta = nextG - prevG;
  int bDelta = nextB - prevB;

  byte r = prevR + (progress * rDelta);
  byte g = prevG + (progress * gDelta);
  byte b = prevB + (progress * bDelta);

  write(r, g, b);
}