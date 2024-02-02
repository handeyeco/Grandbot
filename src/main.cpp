#include <Arduino.h>
#include <Grandbot.h>

// Arduino Pins
#define dataPin 11
#define clockPin 13
#define loadPin 10

#define voicePin 3
#define playPin 2
#define lightPin A0

#define redPin 5
#define greenPin 6
#define bluePin 9

// Leave floating
#define randomPin A5

// Set this to 1 and upload to check that
// everything is wired up as expected
#define demoMode 0

Grandbot gb = Grandbot(dataPin, clockPin, loadPin, voicePin, redPin, greenPin, bluePin);

int lastPlayRead = HIGH;

void setup() {
  pinMode(playPin, INPUT_PULLUP);

  randomSeed(analogRead(randomPin));

  Serial.begin(9600);
}

void loop() {
  int light = analogRead(lightPin);
  int playRead = digitalRead(playPin);

  if (demoMode) {
    Serial.print("Light: ");
    Serial.print(light);
    Serial.print(" Button: ");
    Serial.println(playRead);
    gb.demo();
    return;
  }

  // play button is active LOW
  if (playRead == LOW && lastPlayRead == HIGH) {
    gb.play();
  }
  lastPlayRead = playRead;

  gb.update(light);
}
