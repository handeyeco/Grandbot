#include <LedControl.h>
#include <Voice.h>
#include <Expressions.h>

#ifndef GRANDBOT_INCL_GUARD
#define GRANDBOT_INCL_GUARD

class Grandbot {
    private:
        LedControl lc;
        Voice voice;

        int exprIndex = 0;
        void setExpression(int expressionIndex);
        void writeExpression();

        unsigned long nextBlink;
        int blinkLength;
        boolean isBlinking = false;
        unsigned long getNextBlink();
        int getBlinkLength();

        unsigned long nextExpressionChange;
        unsigned long getNextExpressionChange();

        void sleep();
        void wakeup();

        static const int lightThreshold = 100;

        // -1 = uninitialized
        //  0 = sleeping
        //  1 = neutral
        int state = -1;

        void debug(unsigned long now);
    public:
        Grandbot(int dataPin, int clockPin, int loadPin, int voicePin);
        void play();
        void update(int light);
};

#endif