#include <Grandbot.h>
#include <Expression.h>
#include <LedControl.h>

int sleeping[4] = {
  B00000000,
  B10000001,
  B00000001,
  B00000000
};

int forward[4] = {
  B00000000,
  B01111110,
  B01111110,
  B00000000
};

int blinking[4] = {
  B00000000,
  B00001000,
  B00001000,
  B00000000
};

Expression expressions[] = {
  Expression(sleeping, sleeping),
  Expression(forward, blinking)
};
 
Grandbot::Grandbot(int dataPin, int clockPin, int loadPin, int voicePin)
  : lc(LedControl(dataPin, clockPin, loadPin)), voice(Voice(voicePin)) {
    // Wake up Max7219
    lc.shutdown(0, false);
    // Set the brightness
    lc.setIntensity(0, 14);
    // Only scan 4 digits
    lc.setScanLimit(0, 3);
    // Clear the display
    lc.clearDisplay(0);

    nextBlink = getNextBlink();
    blinkLength = getBlinkLength();
}

int Grandbot::getNextBlink() {
  return millis() + random(2000, 10000);
}

int Grandbot::getBlinkLength() {
  return random(100, 300);
}

void Grandbot::writeExpression(int expr[4]) {
  for (int i = 0; i < 4; i++) {
    lc.setRow(0, i, expr[i]);
  }
}

void Grandbot::setExpression(int expressionIndex) {
    expression = expressionIndex;
}

void Grandbot::update(int light) {
  int now = millis();

  if (state == 0) {
    // Wakeup
    if (light > Grandbot::lightThreshold) {
      state = 1;
      setExpression(1);

      lc.setIntensity(0, 14);
      writeExpression(expressions[expression].getRegular());
      voice.playRandomSequence();

      nextBlink = getNextBlink();
      blinkLength = getBlinkLength();
    }
  } else {
    // Sleep
    if (light < Grandbot::lightThreshold) {
      state = 0;
      setExpression(0);

      lc.setIntensity(0, 1);
      writeExpression(expressions[expression].getRegular());
      voice.sleepy();
    } 
    // Normal
    else {
      if (!isBlinking && now > nextBlink) {
        isBlinking = true;
        writeExpression(expressions[expression].getBlinking());
      }
      
      else if (isBlinking && now > nextBlink + blinkLength) {
        isBlinking = false;
        writeExpression(expressions[expression].getRegular());
        nextBlink = getNextBlink();
        blinkLength = getBlinkLength();
      }
    }
  }
}