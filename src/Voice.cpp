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

int Voice::setMajor7th(int startIndex, int root) {
  int len = 4;

  int notes[len] = {
    pitches[root],
    pitches[root+4],
    pitches[root+7],
    pitches[root+11]
  };

  for (int i = 0; i < len; i++) {
    melody[startIndex + i] = notes[i];
    rhythm[startIndex + i] = 250;
  }

  return len;
}

int Voice::setTriad(int startIndex, int root, boolean major) {
  int len = 3;
  int third = major ? 4 : 3;

  int notes[len] = {
    pitches[root],
    pitches[root + third],
    pitches[root + 7]
  };

  int noteLenFlip = random(0, 4);
  int noteLen = noteLenFlip == 0 ? 100
    : noteLenFlip == 1 ? 200
    : noteLenFlip == 2 ? 300
    : 400;
  for (int i = 0; i < len; i++) {
    melody[startIndex + i] = notes[i];
    rhythm[startIndex + i] = noteLen;
  }

  return len;
}

int Voice::setSong() {
  int start = random(0, 12);
  int chords = random(4, 9);

  int interval;
  boolean major;
  int melodyIndex = 0;
  for (int i = 0; i < chords; i++) {
    interval = random(0, 7);
    major = interval == 0 || interval == 3 || interval == 4;
    melodyIndex += setTriad(melodyIndex, start + interval, major);
  }
  
  return melodyIndex;
}

int Voice::setRandomSequence(int len) {
  for (int i = 0, rand; i < len; i++) {
    rand = random(0, 28);
    melody[i] = pitches[rand];
    rhythm[i] = 75;
  }

  return len;
}

int Voice::setUnhappy() {
  melody[0] = 200;
  melody[1] = 100;
  rhythm[0] = 1000;
  rhythm[1] = 1000;

  return 2;
}

void Voice::play(int playLength) {
  playing = true;
  melodyLength = playLength;
  currNoteIndex = -1;
}

void Voice::emote(int mood, int esteem) {
  int len = 0;

  switch(mood) {
    // Sleeping
    case 0:
      len = setMajor7th(0, random(0, 12));
      break;
    // Happy
    case 1:
      len = setSong();
      break;
    // Neutral
    case 2:
      len = setRandomSequence(esteem);
      break;
    // Unhappy
    case 3:
      len = setUnhappy();
      break;
  }

  play(len);
}

void Voice::update() {
  if (!playing) return;

  unsigned long now = millis();
  if (currNoteIndex == -1 || now > noteStart + rhythm[currNoteIndex]) {
    currNoteIndex++;

    if (currNoteIndex < melodyLength) {
      tone(m_voicePin, melody[currNoteIndex]);
      noteStart = now;
      return;
    } else {
      noTone(m_voicePin);
      playing = false;
    }
  }
}