#include <Grandbot.h>
#include <Expression.h>
#include <Expressions.h>
#include <LedControl.h>
 
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
    nextExpressionChange = getNextExpressionChange();
}

unsigned long Grandbot::getNextBlink() {
  return millis() + random(2000, 10000);
}

int Grandbot::getBlinkLength() {
  return random(100, 300);
}

unsigned long Grandbot::getNextExpressionChange() {
  return millis() + random(10000, 100000);
}

void Grandbot::writeExpression() {
  int* expr;
  if (isBlinking) {
    expr = Expressions::expressions[exprIndex].getBlinking();
  } else {
    expr = Expressions::expressions[exprIndex].getRegular();
  }

  for (int i = 0; i < 4; i++) {
    lc.setRow(0, i, expr[i]);
  }
}

void Grandbot::setExpression(int expressionIndex) {
    exprIndex = expressionIndex;
    writeExpression();
}

void Grandbot::sleep() {
  state = 0;
  int nextExpr = Expressions::getSleepyIndex();
  setExpression(nextExpr);
  lc.setIntensity(0, 1);
}

void Grandbot::wakeup() {
  state = 1;
  int nextExpr = Expressions::getHappyIndex();
  setExpression(nextExpr);
  lc.setIntensity(0, 14);
  nextBlink = getNextBlink();
  blinkLength = getBlinkLength();
}

void Grandbot::update(int light) {
  unsigned long now = millis();
  boolean lightOn = light > Grandbot::lightThreshold;
  // debug(now);

  if (state == 0) {
    // Wakeup
    if (lightOn) {
      wakeup();
      voice.playRandomSequence();
    }
  } else if (state > 0) {
    // Sleep
    if (!lightOn) {
      sleep();
      voice.sleepy();
    } 
    // Normal
    else {
      if (now > nextExpressionChange) {
        int nextExpr = Expressions::getNeutralIndex();
        setExpression(nextExpr);
        nextExpressionChange = getNextExpressionChange();
      }

      if (!isBlinking && now > nextBlink) {
        isBlinking = true;
        writeExpression();
      }
      
      else if (isBlinking && now > nextBlink + blinkLength) {
        isBlinking = false;
        writeExpression();
        nextBlink = getNextBlink();
        blinkLength = getBlinkLength();
      }
    }
  } else {
    // Initialize (no sound)
    if (lightOn) {
      wakeup();
    } else {
      sleep();
    }
  }
}

void Grandbot::debug(unsigned long now) {
  Serial.print("now: ");
  Serial.print(now);
  Serial.print(" isBlinking: ");
  Serial.print(isBlinking);
  Serial.print(" nextBlink: ");
  Serial.print(nextBlink);
  Serial.print(" blinkLength: ");
  Serial.print(blinkLength);
  Serial.print(" nextExpressionChange: ");
  Serial.println(nextExpressionChange);
}