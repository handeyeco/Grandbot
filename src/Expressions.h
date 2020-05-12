#include <Expression.h>

#ifndef EXPRESSIONS_INCL_GUARD
#define EXPRESSIONS_INCL_GUARD

struct Expressions {
  private:
    static Expression errorExpressions[];
    static Expression sleepingExpressions[];
    static Expression happyExpressions[];
    static Expression neutralExpressions[];
    static Expression unhappyExpressions[];
  public:
    static Expression* getExpression(int state);
};

#endif