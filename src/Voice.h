#include <Arduino.h>
#include <pitches.h>

#ifndef VOICE_INCL_GUARD
#define VOICE_INCL_GUARD

class Voice {
  private:
    int m_voicePin;
    static const int singingPitches[];
    int setTriad(int startIndex, int root, boolean major);
    int setMajor7th(int startIndex, int root);

    int setRandomSequence(int len);
    int setUnhappy();
    int setSong();

    void play(int playLength);

    int currNoteIndex;
    int melodyLength;
    unsigned long noteStart = 0;
    boolean playing = false;
    int melody[30];
    int rhythm[30];
    int heldPitches[88];
  public:
    Voice(int voicePin);
    void emote(int mood, int esteem);
    void update();
    void demo();
    void handleNoteOn(byte channel, byte pitch, byte velocity);
    void handleNoteOff(byte channel, byte pitch, byte velocity);
};

#endif