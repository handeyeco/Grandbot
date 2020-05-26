#include <LedControl.h>
#include <Voice.h>
#include <Light.h>
#include <Expression.h>
#include <Expressions.h>

#ifndef GRANDBOT_INCL_GUARD
#define GRANDBOT_INCL_GUARD

class Grandbot {
    private:
        LedControl lc;
        Voice voice;
        Light light;
        Expression* expression;

        void setExpression();
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

        static const int wakeThresh = 200;
        static const int sleepThresh = 100;

        // -1 = uninitialized
        //  0 = sleeping
        //  1 = happy
        //  2 = neutral
        //  3 = unhappy
        int mood = -1;

        // between 0 (unhappiest) and 9 (happiest)
        int esteem = 8;

        void updateEsteem();
        void updateMood();
        unsigned long lastPlayTime = 0;

        // One hour
        static const unsigned long playThresh = 3600000LL;
        // Two hours
        static const unsigned long ignoreThresh = 7200000LL;

        void debug(unsigned long now);
    public:
        Grandbot(int dataPin, int clockPin, int loadPin, int voicePin, int redPin, int greenPin, int bluePin);
        void play();
        void update(int lightReading);
};

#endif