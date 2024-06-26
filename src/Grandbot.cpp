#include <Grandbot.h>
 
Grandbot::Grandbot()
  : voice(), light(), lc(SERIAL_DATA_PIN, SERIAL_CLOCK_PIN, SERIAL_LOAD_PIN, 1), expr(&lc, &light)
{}


Expressions* Grandbot::getExpressionPointer(){
  return &expr;
}

Light* Grandbot::getLightPointer(){
  return &light;
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

  expr.setExpression(mood);
  light.setColor(mood);

  if (last != nextMood) {
    light.setMood(nextMood);
    // make a mood sound when changing between moods,
    // it's a call for attention
    voice.emote(mood, esteem);
  }
}

/**
 * Sleep is a low activity state;
 * no mood, expression, or LED changes
*/
void Grandbot::sleep() {
  lc.setIntensity(0, 0);
  int lastMood = mood;
  mood = 0;
  expr.setExpression(mood);
  light.setMood(0);
  light.setColor(mood);

  // So we don't play a sound
  // if we reset at night
  if (lastMood >= 0) {
    voice.emote(mood, esteem);
  }
}

/**
 * Return back to an active state
*/
void Grandbot::wakeup() {
  // So he doesn't wake up angry
  lastPlayTime = millis();

  lc.setIntensity(0, 14);
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
  voice.emote(mood, esteem);
}

/**
 * Setup to be called during the Arduino setup stage.
*/
void Grandbot::setup() {
  pinMode(PLAY_BUTTON_PIN, INPUT_PULLUP);
  randomSeed(analogRead(RANDOM_PIN));

  // Wake up Max7219
  lc.shutdown(0, false);
  // Set the brightness
  lc.setIntensity(0, 14);
  // Only scan 4 digits
  lc.setScanLimit(0, 4);
  // Clear the display
  lc.clearDisplay(0);

  expr.init();
}

/**
 * Reads the button a returns if it's been pressed
 * since last read
 * 
 * @returns {bool} whether button has been pressed
*/
bool Grandbot::readButton() {
  bool read = digitalRead(PLAY_BUTTON_PIN);
  bool pressed = read == LOW && lastButtonRead == HIGH;
  lastButtonRead = read;
  return pressed;
}

/**
 * Update to be called during the Arduino update cycle.
 * Triggers sleep/wake and handles esteem drift timing
*/
void Grandbot::update(bool buttonPressed) {
  if (buttonPressed) {
    play();
  }
  
  unsigned long now = millis();
  int lightRead = analogRead(LIGHT_SENSOR_PIN);
  bool awake = lightRead > wakeThresh;
  bool asleep = lightRead < sleepThresh;

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

      expr.update(mood);
    }
  } else {
    sleep();
  }

  light.update();
  voice.update();
}