#include <Arduino.h>
#include <Grandbot.h>

// Arduino Pins
int dataPin = 12;
int clockPin = 11;
int loadPin = 10;
int voicePin = 3;

Grandbot gb = Grandbot(dataPin, clockPin, loadPin, voicePin);

void setup() {}

void loop() {
  int light = analogRead(A0);
  gb.update(light);
}
