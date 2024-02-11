#include <Expressions.h>

Expressions::Expressions() {
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