#include <Expression.h>

#ifndef EXPRESSIONS_INCL_GUARD
#define EXPRESSIONS_INCL_GUARD

struct Expressions {
  private:
    static Expression expressions[];
  public:
    static Expression* getExpression(int state);
};

#endif