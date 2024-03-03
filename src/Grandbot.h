#include <LedControl.h>
#include <Voice.h>
#include <Light.h>
#include <Expression.h>
#include <Expressions.h>

#ifndef GRANDBOT_INCL_GUARD
#define GRANDBOT_INCL_GUARD

class Grandbot {
    private:
        Expressions* expr;
        LedControl* lc;
        Voice* voice;
        Light* light;

        void sleep();
        void wakeup();

        static const int wakeThresh = 400;
        static const int sleepThresh = 300;

        // -1 = uninitialized
        //  0 = sleeping
        //  1 = happy
        //  2 = neutral
        //  3 = unhappy
        int mood = -1;

        // between 0 (unhappiest) and 9 (happiest)
        byte esteem = 8;

        void updateMood();
        unsigned long lastPlayTime = 0;

        // One hour
        static const unsigned long playThresh = 30000LL;
        // Two hours
        static const unsigned long ignoreThresh = 7200000LL;

        void debug(unsigned long now);
    public:
        Grandbot(Expressions* _expr, LedControl* _lc, Voice* _voice, Light* _light);
        void play();
        void update(int lightReading);
};

#endif