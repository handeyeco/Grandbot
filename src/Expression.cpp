#include <Expression.h>

Expression::Expression(byte (&regular)[4], byte (&blinking)[4], bool sleep)
    : m_regular(regular), m_blinking(blinking), m_sleep(sleep) {}

byte* Expression::getRegular() {
  return m_regular;
}

byte* Expression::getBlinking() {
  return m_blinking;
}

bool Expression::isSleep() {
  return m_sleep;
}