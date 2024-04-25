#include <Grandbot.h>
 
Grandbot::Grandbot(Expressions* _expr, LedControl* _lc, Voice* _voice, Light* _light) {
  this->expr = _expr;
  this->lc = _lc;
  this->voice = _voice;
  this->light = _light;
}

/**
 * Determine mood based on esteem (a finer resolution mood);
 * triggers a new expression and new LED color
*/
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

  expr->setExpression(mood);
  light->setColor(mood);

  if (last != nextMood) {
    light->setMood(nextMood);
    // make a mood sound when changing between moods,
    // it's a call for attention
    voice->emote(mood, esteem);
  }
}

/**
 * Sleep is a low activity state;
 * no mood, expression, or LED changes
*/
void Grandbot::sleep() {
  lc->setIntensity(0, 0);
  int lastMood = mood;
  mood = 0;
  expr->setExpression(mood);
  light->setMood(0);
  light->setColor(mood);

  // So we don't play a sound
  // if we reset at night
  if (lastMood >= 0) {
    voice->emote(mood, esteem);
  }
}

/**
 * Return back to an active state
*/
void Grandbot::wakeup() {
  // So he doesn't wake up angry
  lastPlayTime = millis();

  lc->setIntensity(0, 14);
  updateMood();
}

/**
 * When Grandbot is interacted with,
 * it boosts his esteem and he makes sound
*/
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

/**
 * Update to be called during the Arduino update cycle.
 * Triggers sleep/wake and handles esteem drift timing
 *
 * @param {int} lightReading - the last reading from the light sensor
*/
void Grandbot::update(int lightReading) {
  unsigned long now = millis();
  bool awake = lightReading > wakeThresh;
  bool asleep = lightReading < sleepThresh;

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
      if (now - lastPlayTime > ignoreThresh) {
        lastPlayTime = now;
        esteem = max(0, esteem - 1);
        updateMood();
      }

      expr->update(mood);
    }
  } else {
    sleep();
  }
}