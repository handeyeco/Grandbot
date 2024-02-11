#include <Grandbot.h>
 
Grandbot::Grandbot(Expressions* _expr, LedControl* _lc, Voice* _voice, Light* _light) {
  this->expr = _expr;
  this->lc = _lc;
  this->voice = _voice;
  this->light = _light;
  handleChangeBlinkState();
  handleChangeExpressionState();
}

void Grandbot::handleChangeBlinkState() {
  lastBlinkChange = millis();
  isBlinking = !isBlinking;
  if (isBlinking) {
    blinkDelay = random(100, 300);
  } else {
    blinkDelay = random(2000, 10000);
  }
}

void Grandbot::handleChangeExpressionState() {
  setExpression();
  lastExpressionChange = millis();
  expressionChangeDelay = random(10000, 100000);
}

void Grandbot::writeLedControlData(byte* data) {
  for (int i = 0; i < 4; i++) {
    lc->setRow(0, i, data[i]);
  }
}

void Grandbot::writeExpression() {
  Expression expr = *expression;
  byte* data;
  if (isBlinking) {
    data = expr.getBlinking();
  } else {
    data = expr.getRegular();
  }

  writeLedControlData(data);
}

void Grandbot::updateEsteem() {
  unsigned long now = millis();

  if (now - lastPlayTime > ignoreThresh) {
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
    light->setMood(nextMood);
    voice->emote(mood, esteem);
  }
}

void Grandbot::setExpression() {
  expression = expr->getExpression(mood);
  writeExpression();
  light->setColor(mood);
}

void Grandbot::demo() {
  light->demo();

  unsigned long now = millis();
  if (now - lastExpressionChange > 1000) {
    // rotate through 4D7S segments (there are 8)
    demoSegmentIndex = (demoSegmentIndex + 1) % 8;
    byte* demoData = expr->getDemo(demoSegmentIndex);
    for (int i = 0; i < 4; i++) {
      lc->setRow(0, i, *demoData);
    }

    voice->demo();

    lastExpressionChange = millis();
  }
}

void Grandbot::sleep() {
  lc->setIntensity(0, 0);
  int lastMood = mood;
  mood = 0;
  setExpression();
  light->setMood(0);

  // So we don't play a sound
  // if we reset at night
  if (lastMood >= 0) {
    voice->emote(mood, esteem);
  }
}

void Grandbot::wakeup() {
  // So he doesn't wake up angry
  lastPlayTime = millis();
  handleChangeBlinkState();

  lc->setIntensity(0, 14);
  updateMood();
}

void Grandbot::play() {
  if (mood < 1) {
    return;
  }

  unsigned long now = millis();

  if (now - lastPlayTime > playThresh) {
    lastPlayTime = now;
    esteem = min(9, esteem + 1);
  }

  updateMood();
  voice->emote(mood, esteem);
}

void Grandbot::update(int lightReading) {
  unsigned long now = millis();
  boolean awake = lightReading > wakeThresh;
  boolean asleep = lightReading < sleepThresh;

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
      if (now - lastExpressionChange > expressionChangeDelay) {
        updateEsteem();
        handleChangeExpressionState();
      }

      if (now - lastBlinkChange > blinkDelay) {
        handleChangeBlinkState();
        writeExpression();
      }
    }
  } else {
    sleep();
  }
}