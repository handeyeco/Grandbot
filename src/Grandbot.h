#include <Arduino.h>
#include <Expression.h>
#include <LedControl.h>

class Grandbot {
    private:
        LedControl lc;

        int expression = 0;
        void setExpression(int expressionIndex);
        void writeExpression(int expr[4]);

        int nextBlink;
        int blinkLength;
        boolean isBlinking = false;
        int getNextBlink();
        int getBlinkLength();
    public:
        Grandbot(int dataPin, int clockPin, int loadPin);
        void play();
        void update();
};