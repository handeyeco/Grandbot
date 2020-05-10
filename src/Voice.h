#include <Arduino.h>

#ifndef VOICE_INCL_GUARD
#define VOICE_INCL_GUARD

class Voice {
  private:
    int m_voicePin;
    static const int pitches[];
    void playMajor7th();
    void playRandomSequence();
    void playRandomNote();
    void play(int note);
    void play(int note, int duration);
  public:
    Voice(int voicePin);
    void feedback();
    void emote(int state);
};

#endif