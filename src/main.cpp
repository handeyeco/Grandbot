#include <Arduino.h>
#include <Grandbot.h>

// Arduino Pins
int dataPin = 12;
int clockPin = 11;
int loadPin = 10;

Grandbot gb = Grandbot(dataPin, clockPin, loadPin);

void setup() {

}

void loop() {
  gb.update();
}
