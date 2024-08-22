#include <Expression.h>

Expression::Expression(byte (&regular)[4], byte (&blinking)[4])
    : m_regular(regular), m_blinking(blinking) {}

Expression::Expression(byte (*regular)[4], byte (*blinking)[4])
    : m_regular(*regular), m_blinking(*blinking) {}

byte *Expression::getRegular() { return m_regular; }

byte *Expression::getBlinking() { return m_blinking; }