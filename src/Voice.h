#include <Arduino.h>

#ifndef VOICE_INCL_GUARD
#define VOICE_INCL_GUARD

class Voice {
  private:
    int m_voicePin;
    static const int pitches[];
    int setTriad(int startIndex, int root, boolean major);
    int setMajor7th(int startIndex, int root);

    int setRandomSequence();
    int setUnhappy();
    int setSong();

    void playSong();
    void play(int playLength);

    int currNoteIndex;
    int melodyLength;
    unsigned long noteStart = 0;
    boolean playing = false;
    int melody[30];
    int rhythm[30];
  public:
    Voice(int voicePin);
    void emote(int mood);
    void update();
};

#endif