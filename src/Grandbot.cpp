#include <Grandbot.h>
#include <Expression.h>
#include <LedControl.h>

int forward[4] = {
  B00000000,
  B01111110,
  B01111110,
  B00000000
};

int forwardEyebrows[4] = {
  B00000000,
  B01011101,
  B01011101,
  B00000000
};

int blinking[4] = {
  B00000000,
  B00001000,
  B00001000,
  B00000000
};

Expression expressions[] = {
  Expression(forward, blinking),
  Expression(forwardEyebrows, blinking)
};
 
Grandbot::Grandbot(int dataPin, int clockPin, int loadPin)
  : lc(LedControl(dataPin, clockPin, loadPin)) {
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

void Grandbot::update() {
  int now = millis();
  
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