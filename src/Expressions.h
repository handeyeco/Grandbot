#include <LedControl.h>
#include <Light.h>
#include <Expression.h>
#include <ExpressionSets.h>

#ifndef EXPRESSIONS_INCL_GUARD
#define EXPRESSIONS_INCL_GUARD

/**
 * Expressions manages fetching Expression structs
 * and displaying them based on Grandbot's esteem value.
 * This also includes managing blinking.
 * 
 * Expressions also writes text to the display, which
 * is mostly used by the Arp. #TODO split this logic out,
 * along with all Arp logic, from Grandbot logic.
*/
class Expressions {
  private:
    // Library for the 4D7S
    LedControl* lc;
    // Controls the RGB LED
    Light* light;
    // Getters for the different faces
    Expression* expression;

    // Is it actively blinking
    bool isBlinking = false;
    // When we last changed blink states
    unsigned long lastBlinkChange;
    // Time between blink state changes
    unsigned long blinkDelay;
    void handleChangeBlinkState();

    // Last time we changed the expression
    unsigned long lastExpressionChange;
    // Time between expression changes
    unsigned long expressionChangeDelay;
    void handleChangeExpressionState(int mood);
    Expression* getExpression(int state);
  
    void writeToDisplay(byte* data);
    void writeExpression();

    // ARP STUFF #TODO move this
    // When we last wrote text to the screen
    unsigned long lastControlChange = 0;
    bool isShowingControl();
    bool isUnsupportedChar(char c);
    byte convertCharToByte(char c);
  public:
    Expressions(LedControl* _lc, Light* _light);
    void init();
    void update(int mood);
    void setExpression(int mood);

    // ARP STUFF #TODO move this
    void midiBeat(bool beat);
    void control(byte (&ccDisplay)[2], char (&valDisplay)[2]);
};

#endif