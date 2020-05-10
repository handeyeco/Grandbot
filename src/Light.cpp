#include <Light.h>

Light::Light(int redPin, int greenPin, int bluePin) {
  red = redPin;
  green = greenPin;
  blue = bluePin;

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
}

void Light::write(int rValue, int gValue, int bValue) {
  analogWrite(red, rValue);
  analogWrite(green, gValue);
  analogWrite(blue, bValue);
}

void Light::update(int state) {
  int r = 0;
  int g = 0;
  int b = 0;

  if (state == 1) {
    r = random(150, 256);
    b = random(150, 256);
  } else if (state == 2) {
    r = random(0, 100);
    g = random(150, 256);
    b = random(0, 100);
  }

  switch(state) {
    // Sleeping
    case 0:
      write(0, 0, 0);
      return;
    // Happy
    case 1:
      write(r, g, b);
      return;
    // Neutral
    case 2:
      write(r, g, b);
      return;
    // Unhappy
    case 3:
      write(255, 0, 0);
      return;
  }
}