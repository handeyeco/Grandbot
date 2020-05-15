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

void Light::update(int mood) {
  int r = random(0, 256);
  int g = random(0, 256);
  int b = random(0, 256);

  switch(mood) {
    // Sleeping
    case 0:
      write(0, 0, 0);
      return;
    // Happy
    case 1:
      write(r, g, 255);
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