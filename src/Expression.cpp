#include <Expression.h>

Expression::Expression(int (&regular)[4], int (&blinking)[4]) 
  : m_regular(regular), m_blinking(blinking)
  {}

Expression::Expression(int (*regular)[4], int (*blinking)[4]) 
  : m_regular(*regular), m_blinking(*blinking)
  {}

int* Expression::getRegular() {
  return m_regular;
}

int* Expression::getBlinking() {
  return m_blinking;
}