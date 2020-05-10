#include <Voice.h>

const int Voice::pitches[28] = { 
  1047, 
  1109, 
  1175, 
  1245, 
  1319, 
  1397, 
  1480, 
  1568, 
  1661, 
  1760, 
  1865, 
  1976, 
  2093, 
  2217, 
  2349, 
  2489, 
  2637, 
  2794, 
  2960, 
  3136, 
  3322, 
  3520, 
  3729, 
  3951, 
  4186, 
  4435, 
  4699, 
  4978 
};

Voice::Voice(int voicePin) {
  m_voicePin = voicePin;
}

void Voice::playMajor7th() {
  int duration = 250;
  int start = random(0, 12);

  // Major 7th chord
  play(pitches[start], duration);
  play(pitches[start+4], duration);
  play(pitches[start+7], duration);
  play(pitches[start+11], duration);
}

void Voice::playRandomSequence() {
  int len = random (3, 10);
  for (int i = 0; i < len; i++) {
    playRandomNote();
  }
}

void Voice::playRandomNote() {
  int rand = random(0, 28);
  int note = Voice::pitches[rand];
  play(note);
}

void Voice::play(int note) {
  play(note, 75);
}

void Voice::play(int note, int duration) {
  tone(m_voicePin, note);
  delay(duration);
  noTone(m_voicePin);
}

void Voice::feedback() {
  playRandomNote();
}

void Voice::emote(int state) {
  switch(state) {
    // Sleeping
    case 0:
      playMajor7th();
      return;
    // Happy
    case 1:
      playRandomSequence();
      return;
    // Neutral
    case 2:
      playRandomNote();
      return;
    // Unhappy
    case 3:
      play(200, 1000);
      play(100, 1000);
      return;
  }
}