#include <LedControl.h>
#include <Voice.h>

class Grandbot {
    private:
        LedControl lc;
        Voice voice;

        int expression = 0;
        void setExpression(int expressionIndex);
        void writeExpression(int expr[4]);

        int nextBlink;
        int blinkLength;
        boolean isBlinking = false;
        int getNextBlink();
        int getBlinkLength();

        static const int lightThreshold = 100;

        // 0 = sleeping, 1 = neutral
        int state = 0;
    public:
        Grandbot(int dataPin, int clockPin, int loadPin, int voicePin);
        void play();
        void update(int light);
};