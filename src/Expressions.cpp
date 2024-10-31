#include <Expressions.h>

Expressions::Expressions(Light* _light) : lc(SERIAL_DATA_PIN, SERIAL_CLOCK_PIN, SERIAL_LOAD_PIN, 1) {
  this->light = _light;
}

void Expressions::init() {
  // Wake up Max7219
  lc.shutdown(0, false);
  // Set the brightness
  lc.setIntensity(0, currIntensity);
  // Only scan 4 digits
  lc.setScanLimit(0, 4);
  // Clear the display
  lc.clearDisplay(0);

  handleChangeBlinkState();
  handleChangeExpressionState(1);
  lastExpressionChange = millis();
}

/**
 * Increase the intensity of the MAX7219 to the max
 * without writing to it needlessly
 */
void Expressions::maxIntensity() {
  if (currIntensity != 14) {
    currIntensity = 14;
    lc.setIntensity(0, currIntensity);
  }
}

/**
 * decrease the intensity of the MAX7219 to the min
 * without writing to it needlessly
 */
void Expressions::minIntensity() {
  if (currIntensity != 0) {
    currIntensity = 0;
    lc.setIntensity(0, currIntensity);
  }
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
 * @param {bool} delayUpdate - whether update should interupt current display
 * (like setting changes)
 * @param {bool} colon - whether or not to light the colon on the display
 */
void Expressions::writeToDisplay(byte* data,
                                 bool delayUpdate = true,
                                 bool colon = false) {
  // Skip if we're currently displaying text
  // (for the Arp)
  if (delayUpdate && (isShowingControl() || inMenu)) {
    return;
  }

  for (int i = 0; i < 4; i++) {
    lc.setRow(0, i, data[i]);
  }

  // Set colon
  lc.setRow(0, 4, colon ? B10000000 : B00000000);
}

/**
 * Writes the active Expression to the display
 *
 * @param {bool} delayUpdate - whether update should interupt current display
 * (like setting changes)
 */
void Expressions::writeExpression(bool delayUpdate = true) {
  Expression expr = *expression;
  byte* data;
  if (isBlinking) {
    data = expr.getBlinking();
  } else {
    data = expr.getRegular();
  }

  if (expr.isSleep()) {
    // sleep
    minIntensity();
  } else {
    // awake
    maxIntensity();
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
Expression* Expressions::getExpression(int mood) {
  int i = 0;

  switch (mood) {
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
void Expressions::midiBeat(bool even, bool showChangeQueued) {
  maxIntensity();
  Expression e = ExpressionSets::midiBeatExpressions[even];
  writeToDisplay(e.getRegular());

  if (showChangeQueued) {
    setLed(4, 1, true);
  }
}

/**
 * Shows the last control text for 1s
 *
 * @returns {bool} whether we're actively showing control text
 */
bool Expressions::isShowingControl() {
  if (lastControlChange == 0)
    return false;
  unsigned long now = millis();
  return now - lastControlChange < 1000;
}

/**
 * Write text to the 4D7S display while updating lastControlChange
 * (to block updates so expression changes don't interupt setting changes)
 *
 * @param {byte*} digits - pointer to an array of bytes representing what should
 * be written
 * @param {bool} colon - whether or not to light the colon on the display
 */
void Expressions::writeText(byte* digits, bool colon = true) {
  maxIntensity();
  lastControlChange = millis();

  writeToDisplay(digits, false, colon);
}

/**
 * Toggle menu and write expression when leaving
 *
 * TODO if we're in MIDI mode, make sure it goes back to MIDI expressions
 *
 * @param {bool} menu - whether or not we're in the menu
 */
void Expressions::setMenu(bool menu) {
  maxIntensity();
  inMenu = menu;

  if (!menu) {
    writeExpression(false);
  }
}

void Expressions::setLed(int digit, int ledIndex, boolean state) {
  lc.setLed(0, digit, ledIndex, state);
}