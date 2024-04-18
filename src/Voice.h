#include <Arduino.h>
#include <pitches.h>

#ifndef VOICE_INCL_GUARD
#define VOICE_INCL_GUARD

class Voice {
  private:
    int m_voicePin;
    static const byte singingNotes[];
    int setTriad(int startIndex, int root, boolean major);
    int setMajor7th(int startIndex, int root);

    byte setRandomSequence(byte len);
    int setUnhappy();
    int setSong();

    void play(int playLength);

    int currNoteIndex;
    int melodyLength;
    unsigned long noteStart = 0;
    boolean playing = false;
    byte melody[30];
    uint16_t rhythm[30];
  public:
    Voice(int voicePin);
    void emote(int mood, byte esteem);
    void update();
};

#endif