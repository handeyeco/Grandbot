#include <Arduino.h>
#include <Grandbot.h>

// Arduino Pins
int dataPin = 12;
int clockPin = 11;
int loadPin = 10;
int voicePin = 3;
int playPin = 2;
int lightPin = A0;

int red = 5;
int green = 6;
int blue = 9;

// Leave floating
int randomPin = A5;

Grandbot gb = Grandbot(dataPin, clockPin, loadPin, voicePin, red, green, blue);

int lastPlayState = 0;

void setup() {
  pinMode(playPin, INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  Serial.begin(9600);
  randomSeed(analogRead(randomPin));

  analogWrite(red, 10);
  analogWrite(green, 20);
  analogWrite(blue, 40);
}

void loop() {
  int light = analogRead(lightPin);

  int playing = digitalRead(playPin);
  if (playing && lastPlayState == 0) {
    gb.play();
    lastPlayState = !lastPlayState;
  } else if (!playing && lastPlayState == 1) {
    lastPlayState = !lastPlayState;
  }

  gb.update(light);
}
