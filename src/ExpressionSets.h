#include <Expression.h>

#ifndef EXPRESSION_SETS_INCL_GUARD
#define EXPRESSION_SETS_INCL_GUARD

struct ExpressionSets {
  public:
    static Expression errorExpressions[];
    static int errorLength;
    static Expression sleepingExpressions[];
    static int sleepingLength;
    static Expression happyExpressions[];
    static int happyLength;
    static Expression neutralExpressions[];
    static int neutralLength;
    static Expression unhappyExpressions[];
    static int unhappyLength;
};

#endif