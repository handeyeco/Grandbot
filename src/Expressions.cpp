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
void Expressions::writeToDisplay(byte* data) {
  // Skip if we're currently displaying text
  // (for the Arp)
  if (isShowingControl()) {
    return;
  }

  for (int i = 0; i < 4; i++) {
    lc->setRow(0, i, data[i]);
  }

  // Turn the colon off (if it was turned on by `control`)
  lc->setRow(0, 4, B00000000);
}

/**
 * Writes the active Expression to the display
*/
void Expressions::writeExpression() {
  Expression expr = *expression;
  byte* data;
  if (isBlinking) {
    data = expr.getBlinking();
  } else {
    data = expr.getRegular();
  }

  writeToDisplay(data);
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

/**
 * Write control text to the four digit, seven segment display; it's a mess
 * 
 * ex: `SL: 8` uses five bytes:
 * - the "S"
 * - the "L"
 * - the " "
 * - the "8"
 * - the ":" <= is always displayed
 * 
 * @param {byte&[2]} ccDisplay - the left two digits as byte content
 * @param {char&[2]} valDisplay - the right two digits as char content
*/
void Expressions::control(byte (&ccDisplay)[2], char (&valDisplay)[2]) {
  lastControlChange = millis();

  // The first two symbols could probably have been chars,
  // but I wanted the octaves to show lines to represent up or down
  for (int i = 0; i < 2; i++) {
    lc->setRow(0, i, ccDisplay[i]);
  }

  // The next two symbols could have been just chars,
  // but LedControl omits a lot of possible characters
  for (int i = 0; i < 2; i++) {
    char c = valDisplay[i];
    // If LedControl doesn't support the character,
    // use my own list
    if (isUnsupportedChar(c)) {
      byte converted = convertCharToByte(c);
      lc->setRow(0, i+2, converted);
    }
    // Otherwise leverage LedControl's list
    else {
      lc->setChar(0, i+2, c, false);
    }
  }

  // Turn the colon on
  lc->setRow(0, 4, B10000000);
}

bool Expressions::isUnsupportedChar(char c) {
  return c == 'r' || c == 'R';
}

/**
 * My own char to byte conversion; it's overkill,
 * but I'm thinking of just removing LedControl
 * since it's missing chars and hasn't been maintained
 * 
 * @param {char} c - the char to convert
 * @returns {byte} the 4D7S representation of the char
*/
byte Expressions::convertCharToByte(char c) {
  switch (c) {
    case 'A':
    case 'a':
      return CHAR_A;
    case 'B':
    case 'b':
      return CHAR_B;
    case 'C':
    case 'c':
      return CHAR_C;
    case 'D':
    case 'd':
      return CHAR_D;
    case 'E':
    case 'e':
      return CHAR_E;
    case 'F':
    case 'f':
      return CHAR_F;
    case 'G':
    case 'g':
      return CHAR_G;
    case 'H':
    case 'h':
      return CHAR_H;
    case 'I':
    case 'i':
      return CHAR_I;
    case 'J':
    case 'j':
      return CHAR_J;
    case 'L':
    case 'l':
      return CHAR_L;
    case 'N':
    case 'n':
      return CHAR_N;
    case 'O':
    case 'o':
      return CHAR_O;
    case 'P':
    case 'p':
      return CHAR_P;
    case 'R':
    case 'r':
      return CHAR_R;
    case 'S':
    case 's':
      return CHAR_S;
    case 'T':
    case 't':
      return CHAR_T;
    case 'U':
    case 'u':
      return CHAR_U;
    case 'Y':
    case 'y':
      return CHAR_Y;
    case '0':
      return CHAR_0;
    case '1':
      return CHAR_1;
    case '2':
      return CHAR_2;
    case '3':
      return CHAR_3;
    case '4':
      return CHAR_4;
    case '5':
      return CHAR_5;
    case '6':
      return CHAR_6;
    case '7':
      return CHAR_7;
    case '8':
      return CHAR_8;
    case '9':
      return CHAR_9;
    case '-':
      return CHAR_DASH;
    default:
      return CHAR_BLANK;
    }
}