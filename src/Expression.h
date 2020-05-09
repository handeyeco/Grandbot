#include <Arduino.h>

struct Expression {
  private:
    int (&m_regular)[4];
    int (&m_blinking)[4];
  public:
    Expression(int (&regular)[4], int (&blinking)[4]);
    Expression(int (*regular)[4], int (*blinking)[4]);
    int* getRegular();
    int* getBlinking();
};