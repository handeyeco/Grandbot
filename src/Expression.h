#include <Arduino.h>

#ifndef EXPRESSION_INCL_GUARD
#define EXPRESSION_INCL_GUARD

struct Expression {
  private:
    byte (&m_regular)[4];
    byte (&m_blinking)[4];
  public:
    Expression(byte (&regular)[4], byte (&blinking)[4]);
    Expression(byte (*regular)[4], byte (*blinking)[4]);
    byte* getRegular();
    byte* getBlinking();
};

#endif