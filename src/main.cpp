#include <Arduino.h>
#include <Grandbot.h>

// Arduino Pins
int dataPin = 12;
int clockPin = 11;
int loadPin = 10;
int voicePin = 3;
int lightPin = A0;

// Leave floating
int randomPin = A5;

Grandbot gb = Grandbot(dataPin, clockPin, loadPin, voicePin);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(randomPin));
}

void loop() {
  int light = analogRead(lightPin);
  gb.update(light);
}
