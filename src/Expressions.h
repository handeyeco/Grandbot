#include <LedControl.h>
#include <Light.h>
#include <Expression.h>
#include <ExpressionSets.h>

#ifndef EXPRESSIONS_INCL_GUARD
#define EXPRESSIONS_INCL_GUARD

class Expressions {
  private:
    LedControl* lc;
    Light* light;
    Expression* expression;

    boolean isBlinking = false;
    unsigned long lastBlinkChange;
    unsigned long blinkDelay;
    void handleChangeBlinkState();

    unsigned long lastExpressionChange;
    unsigned long expressionChangeDelay;
    void handleChangeExpressionState(int mood);

    void writeLedControlData(byte* data);
    void writeExpression();

    Expression* getExpression(int state);
  public:
    Expressions(LedControl* _lc, Light* _light);
    void init();
    void update(int mood);
    void setExpression(int mood);
    void midiBeat(int beat);
};

#endif