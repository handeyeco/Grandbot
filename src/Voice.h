#include <Arduino.h>
#include <pin_defs.h>
#include <pitches.h>

#ifndef VOICE_INCL_GUARD
#define VOICE_INCL_GUARD

class Voice {
  private:
    // A selection of notes to make melodies out of
    // (represented as MIDI notes)
    static const byte singingNotes[];

    // Whether we're actively playing a sequence
    bool playing = false;
    // Current note in the sequence
    int currNoteIndex;
    // Discrete steps in the melody
    int melodyLength;
    // When the current note started
    unsigned long noteStart = 0;
    // Array of notes to play
    byte melody[30];
    // Array of note lengths to play
    uint16_t rhythm[30];

    int setTriad(int startIndex, int root, bool major);
    int setMajor7th(int startIndex, int root);
    byte setRandomSequence(byte len);

    int setUnhappy();
    int setSong();

    void play(int playLength);
  public:
    Voice();
    void emote(int mood, byte esteem);
    void update();
};

#endif