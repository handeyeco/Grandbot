#include <Arduino.h>
#include <pitches.h>
#include <Synth.h>

#ifndef VOICE_INCL_GUARD
#define VOICE_INCL_GUARD

class Voice {
  private:
    Synth* synth;
    int m_voicePin;
    static const int singingNotes[];
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
    Voice(Synth* synth, int voicePin);
    void emote(int mood, int esteem);
    void update();
    void demo();
};

#endif