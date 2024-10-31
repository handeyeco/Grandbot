#include <Arduino.h>

#ifndef EXPRESSION_INCL_GUARD
#define EXPRESSION_INCL_GUARD

/**
 * An Expression is just a way to store a blinking state
 * and a non-blinking state to be displayed on the
 * four digit, seven segment display
 *
 * Each state has 4 bytes (one for each digit),
 * each byte controls a segment (7 lines, plus a dot)
 */
struct Expression {
 private:
  byte (&m_regular)[4];
  byte (&m_blinking)[4];
  bool m_sleep;

 public:
  Expression(byte (&regular)[4], byte (&blinking)[4], bool sleep = false);
  byte* getRegular();
  byte* getBlinking();
  bool isSleep();
};

#endif