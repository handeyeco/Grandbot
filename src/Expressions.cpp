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
  for (int i = 0; i < 4; i++) {
    lc->setRow(0, i, data[i]);
  }
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

byte segments[8] = {
    B00000001,
    B00000010,
    B00000100,
    B00001000,
    B00010000,
    B00100000,
    B01000000,
    B10000000,
};

byte *Expressions::getDemo(int demoIndex)
{
  return &segments[demoIndex];
}

void Expressions::midiBeat(int beat) {
  int half = beat % 2;
  expression = &ExpressionSets::midiBeatExpressions[half];
  writeExpression();
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