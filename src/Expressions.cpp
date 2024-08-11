#include <Expressions.h>

Expressions::Expressions(LedControl* _lc, Light* _light) {
  this->lc = _lc;
  this->light = _light;
}

void Expressions::init() {
  handleChangeBlinkState();
  handleChangeExpressionState(1);
  lastExpressionChange = millis();
}

/**
 * Switch blink states, setting a random timeout
 * for when the next blink change will happen
*/
void Expressions::handleChangeBlinkState() {
  lastBlinkChange = millis();
  isBlinking = !isBlinking;
  if (isBlinking) {
    blinkDelay = random(100, 300);
  } else {
    blinkDelay = random(2000, 10000);
  }
}

/**
 * Change expression, setting a random timeout for
 * when the next expression change should happen
 * 
 * @param {int} mood - enum for emotional state as defined in Grandbot.h
*/
void Expressions::handleChangeExpressionState(int mood) {
  setExpression(mood);
  expressionChangeDelay = random(10000, 100000);
}

/**
 * Takes a pointer to an array of bytes and sends those
 * bytes to LedControl to be sent to the display
 * 
 * @param {byte*} data - pointer to an array of bytes to be written
*/
void Expressions::writeToDisplay(byte* data, bool delayUpdate = true, bool colon = false) {
  // Skip if we're currently displaying text
  // (for the Arp)
  if (delayUpdate && (isShowingControl() || inMenu)) {
    return;
  }

  for (int i = 0; i < 4; i++) {
    lc->setRow(0, i, data[i]);
  }

  // Turn the colon off (if it was turned on by `control`)
  lc->setRow(0, 4, colon ? B10000000 : B00000000);
}

/**
 * Writes the active Expression to the display
*/
void Expressions::writeExpression(bool delayUpdate = true) {
  Expression expr = *expression;
  byte* data;
  if (isBlinking) {
    data = expr.getBlinking();
  } else {
    data = expr.getRegular();
  }

  writeToDisplay(data, delayUpdate);
}

/**
 * Based on mood, set a random expression as the active one
 * and display it
 * 
 * @param {int} mood - enum for emotional state as defined in Grandbot.h
*/
void Expressions::setExpression(int mood) {
  expression = getExpression(mood);
  writeExpression();
}

/**
 * Based on mood, get and return a random expression
 * 
 * @param {int} mood - enum for emotional state as defined in Grandbot.h
 * @returns random expression based on mood
*/
Expression *Expressions::getExpression(int mood)
{
  int i = 0;

  switch (mood)
  {
  // Sleeping
  case 0:
    i = random(0, ExpressionSets::sleepingLength);
    return &ExpressionSets::sleepingExpressions[i];
  // Happy
  case 1:
    i = random(0, ExpressionSets::happyLength);
    return &ExpressionSets::happyExpressions[i];
  // Neutral
  case 2:
    i = random(0, ExpressionSets::neutralLength);
    return &ExpressionSets::neutralExpressions[i];
  // Unhappy
  case 3:
    i = random(0, ExpressionSets::unhappyLength);
    return &ExpressionSets::unhappyExpressions[i];
  default:
    return &ExpressionSets::unhappyExpressions[0];
  }
}

/**
 * Update to be called during the Arduino update loop,
 * times expression and blinking changes
 * 
 * @param {int} mood - enum for emotional state as defined in Grandbot.h
*/
void Expressions::update(int mood) {
  unsigned long now = millis();

  // Sleep is an inactive state, so we don't need to do things
  if (mood > 0) {
      if (now - lastExpressionChange > expressionChangeDelay) {
        lastExpressionChange = now;
        handleChangeExpressionState(mood);
        light->setColor(mood);
      }

      if (now - lastBlinkChange > blinkDelay) {
        handleChangeBlinkState();
        writeExpression();
      }
  }
}

// =========
// ARP STUFF
// =========

/**
 * Handles the dance Grandbot does when there's a MIDI clock
 * 
 * @param {bool} even - whether we're on an even or odd quarter note
*/
void Expressions::midiBeat(bool even) {
  Expression e = ExpressionSets::midiBeatExpressions[even];
  writeToDisplay(e.getRegular());
}

/**
 * Shows the last control text for 1s
 * 
 * @returns {bool} whether we're actively showing control text
*/
bool Expressions::isShowingControl() {
  if (lastControlChange == 0) return false;
  unsigned long now = millis();
  return now - lastControlChange < 1000;
}

void Expressions::writeText(byte* digits, bool colon = true) {
  lastControlChange = millis();
  
  writeToDisplay(digits, false, colon);
}

bool Expressions::isUnsupportedChar(char c) {
  return c == 'r' || c == 'R';
}


void Expressions::setMenu(bool menu) {
  inMenu = menu;

  if (!menu) {
    writeExpression(false);
  }
}