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
  unsigned long thresh = lastPlayTime + ignoreThresh;

  if (now > thresh) {
    lastPlayTime = now;
    esteem = max(0, esteem - 1);
    updateMood();
  }
}

void Grandbot::updateMood() {
  int last = mood;

  int nextMood;
  if (esteem > 8) {
    nextMood = 1;
  } else if (esteem < 2) {
    nextMood = 3;
  } else {
    nextMood = 2;
  }
  mood = nextMood;

  setExpression();

  if (last != nextMood) {
    voice.emote(mood, esteem);
  }
}

void Grandbot::setExpression() {
  expression = Expressions::getExpression(mood);
  writeExpression();
  light.setColor(mood);
}

void Grandbot::sleep() {
  lc.setIntensity(0, 0);
  int lastMood = mood;
  mood = 0;
  setExpression();

  // So we don't play a sound
  // if we reset at night
  if (lastMood >= 0) {
    voice.emote(mood, esteem);
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
  if (mood < 1) {
    return;
  }

  unsigned long now = millis();
  unsigned long thresh = lastPlayTime + playThresh;

  if (now > thresh) {
    lastPlayTime = now;
    esteem = min(9, esteem + 1);
  }

  updateMood();
  voice.emote(mood, esteem);
}

void Grandbot::update(int lightReading) {
  unsigned long now = millis();
  boolean awake = lightReading > wakeThresh;
  boolean asleep = lightReading < sleepThresh;
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

  light.update(mood);
  voice.update();
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