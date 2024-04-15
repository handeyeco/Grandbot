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

void Expressions::control(byte (&data)[5]) {
  lastControlChange = millis();

  for (int i = 0; i < 5; i++) {
    lc->setRow(0, i, data[i]);
  }
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