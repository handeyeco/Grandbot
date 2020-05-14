#include <Grandbot.h>
 
Grandbot::Grandbot(int dataPin, int clockPin, int loadPin, int voicePin, int redPin, int greenPin, int bluePin)
  : lc(LedControl(dataPin, clockPin, loadPin)), voice(Voice(voicePin)), light(Light(redPin, greenPin, bluePin)) {
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
  Expression expr = *expression;
  byte* data;
  if (isBlinking) {
    data = expr.getBlinking();
  } else {
    data = expr.getRegular();
  }

  for (int i = 0; i < 4; i++) {
    lc.setRow(0, i, data[i]);
  }
}

void Grandbot::setState(int next) {
  int lastState = state;

  state = next;
  setExpression();

  if (lastState > -1 && lastState != state) {
    light.update(state);
    voice.emote(state);
  }
}

void Grandbot::setExpression() {
  expression = Expressions::getExpression(state);
  writeExpression();
}

void Grandbot::sleep() {
  lc.setIntensity(0, 1);
  setState(0);
}

void Grandbot::wakeup() {
  lc.setIntensity(0, 14);
  setState(1);
  nextBlink = getNextBlink();
  blinkLength = getBlinkLength();
}

void Grandbot::play() {
  // voice.feedback();

  // Keep state between 1-3
  setState(((state + 1) % 3) + 1);
}

void Grandbot::update(int light) {
  unsigned long now = millis();
  boolean awake = light > Grandbot::wakeThreshold;
  boolean asleep = light < Grandbot::sleepThreshold;
  // debug(now);

  if (state == 0) {
    // Wakeup
    if (awake) {
      wakeup();
    }
  } else if (state > 0) {
    // Sleep
    if (asleep) {
      sleep();
    } 
    // Normal
    else {
      if (now > nextExpressionChange) {
        setExpression();
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
    sleep();
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