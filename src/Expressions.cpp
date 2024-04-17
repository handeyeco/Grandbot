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

void Expressions::handleChangeBlinkState() {
  lastBlinkChange = millis();
  isBlinking = !isBlinking;
  if (isBlinking) {
    blinkDelay = random(100, 300);
  } else {
    blinkDelay = random(2000, 10000);
  }
}

void Expressions::handleChangeExpressionState(int mood) {
  setExpression(mood);
  expressionChangeDelay = random(10000, 100000);
}

void Expressions::writeLedControlData(byte* data) {
  if (isShowingControl()) {
    return;
  }

  for (int i = 0; i < 4; i++) {
    lc->setRow(0, i, data[i]);
  }

  lc->setRow(0, 4, B00000000);
}

void Expressions::writeExpression() {
  Expression expr = *expression;
  byte* data;
  if (isBlinking) {
    data = expr.getBlinking();
  } else {
    data = expr.getRegular();
  }

  writeLedControlData(data);
}

void Expressions::setExpression(int mood) {
  expression = getExpression(mood);
  writeExpression();
}

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

void Expressions::midiBeat(int even) {
  Expression e = ExpressionSets::midiBeatExpressions[even];
  writeLedControlData(e.getRegular());
}

bool Expressions::isShowingControl() {
  unsigned long now = millis();
  return now - lastControlChange < 1000;
}

void Expressions::control(byte (&ccDisplay)[2], char (&valDisplay)[2]) {
  lastControlChange = millis();

  for (int i = 0; i < 2; i++) {
    lc->setRow(0, i, ccDisplay[i]);
  }

  for (int i = 0; i < 2; i++) {
    lc->setChar(0, i+2, valDisplay[i], false);
  }

  lc->setRow(0, 4, B10000000);
}

void Expressions::update(int mood) {
  unsigned long now = millis();

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
    default:
      return CHAR_BLANK;
    }
}