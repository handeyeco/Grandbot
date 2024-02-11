#include <Light.h>

Light::Light(int rPin, int gPin, int bPin) {
  redPin = rPin;
  greenPin = gPin;
  bluePin = bPin;

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void Light::write(int rVal, int gVal, int bVal) {
  analogWrite(redPin, rVal);
  analogWrite(greenPin, gVal);
  analogWrite(bluePin, bVal);
}

void Light::setColor(int mood) {
  Serial.println(mood);
  prevR = nextR;
  prevG = nextG;
  prevB = nextB;

  if (mood == 0) {
    nextR = 0;
    nextG = 0;
    nextB = 0;
  } else if (mood == 3) {
    nextR = 255;
    nextG = 0;
    nextB = 0;
  } else {
    nextR = random(0, 256);
    nextG = random(0, 256);
    nextB = random(0, 256);
  }

  animating = true;
  startTween = millis();
}

void Light::demo() {
  unsigned long now = millis();

  if (now - lastDemoChange > tweenLength) {
    prevR = nextR;
    prevG = nextG;
    prevB = nextB;

    if (prevR > 0) {
      nextR = 0;
      nextG = 255;
    }

    if (prevG > 0) {
      nextG = 0;
      nextB = 255;
    }
    
    if (prevB > 0) {
      nextB = 0;
      nextR = 255;
    }

    write(nextR, nextG, nextB);

    lastDemoChange = millis();
  }
}

void Light::setMood(int mood) {
  this->mood = mood;
}

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

  int r = prevR + (progress * rDelta);
  int g = prevG + (progress * gDelta);
  int b = prevB + (progress * bDelta);

  write(r, g, b);
}