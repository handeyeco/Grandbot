#include <Expression.h>

#ifndef EXPRESSIONS_INCL_GUARD
#define EXPRESSIONS_INCL_GUARD

struct Expressions {
  public:
    static Expression expressions[];
    static int getSleepyIndex();
    static int getHappyIndex();
    static int getNeutralIndex();
    static int getUpsetIndex();
};

#endif