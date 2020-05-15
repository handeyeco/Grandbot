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

void Grandbot::updateEsteem() {
  unsigned long now = millis();
  unsigned long thresh = lastPlayTime + 21600000LL;

  if (now > thresh) {
    lastPlayTime = now;
    esteem = max(0, esteem - 1);
    updateMood();
  }
}

void Grandbot::updateMood() {
  int last = mood;

  int next;
  if (esteem > 7) {
    next = 1;
  } else if (esteem < 4) {
    next = 3;
  } else {
    next = 2;
  }
  mood = next;

  setExpression();

  if (last != next) {
    voice.emote(mood);
  }
}

void Grandbot::setExpression() {
  expression = Expressions::getExpression(mood);
  writeExpression();
  light.update(mood);
}

void Grandbot::sleep() {
  lc.setIntensity(0, 0);
  int lastMood = mood;
  mood = 0;
  setExpression();

  // So we don't play a sound
  // if we reset at night
  if (lastMood >= 0) {
    voice.emote(mood);
  }
}

void Grandbot::wakeup() {
  // So he doesn't wake up angry
  lastPlayTime = millis();
  nextBlink = getNextBlink();
  blinkLength = getBlinkLength();

  lc.setIntensity(0, 14);
  updateMood();
}

void Grandbot::play() {
  unsigned long now = millis();
  unsigned long thresh = lastPlayTime + 3600000LL;

  if (now > thresh) {
    lastPlayTime = now;
    esteem = min(9, esteem + 1);
  }

  updateMood();
  voice.emote(mood);
}

void Grandbot::update(int light) {
  unsigned long now = millis();
  boolean awake = light > Grandbot::wakeThreshold;
  boolean asleep = light < Grandbot::sleepThreshold;
  // debug(now);

  if (mood == 0) {
    // Wakeup
    if (awake) {
      wakeup();
    }
  } else if (mood > 0) {
    // Sleep
    if (asleep) {
      sleep();
    } 
    // Normal
    else {
      if (now > nextExpressionChange) {
        updateEsteem();
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