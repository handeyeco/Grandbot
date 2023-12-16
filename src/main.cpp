#include <Arduino.h>
#include <Grandbot.h>

// Arduino Pins
#define dataPin 12
#define clockPin 11
#define loadPin 10
#define voicePin 3
#define playPin 2
#define lightPin A0

#define redPin 5
#define greenPin 6
#define bluePin 9

// Leave floating
#define randomPin A5

#define demoMode 0

Grandbot gb = Grandbot(dataPin, clockPin, loadPin, voicePin, redPin, greenPin, bluePin);

int lastPlayState = 0;

void setup() {
  pinMode(playPin, INPUT);

  randomSeed(analogRead(randomPin));

  Serial.begin(9600);
}

void loop() {
  int light = analogRead(lightPin);
  int playing = digitalRead(playPin);

  if (demoMode) {
    Serial.print("Light: ");
    Serial.print(light);
    Serial.print(" Button: ");
    Serial.println(playing);
    gb.demo();
    return;
  }

  if (playing && lastPlayState == 0) {
    gb.play();
    lastPlayState = !lastPlayState;
  } else if (!playing && lastPlayState == 1) {
    lastPlayState = !lastPlayState;
  }

  gb.update(light);
}
