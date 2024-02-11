#include <Expression.h>
#include <ExpressionSets.h>

#ifndef EXPRESSIONS_INCL_GUARD
#define EXPRESSIONS_INCL_GUARD

class Expressions {
  private:
  public:
    Expressions();
    Expression* getExpression(int state);
    byte* getDemo(int demoIndex);
};

#endif