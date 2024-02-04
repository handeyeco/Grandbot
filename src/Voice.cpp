#include <Voice.h>

const int Voice::singingPitches[28] = { 
  NOTE_C6,
  NOTE_CS6,
  NOTE_D6,
  NOTE_DS6,
  NOTE_E6,
  NOTE_F6,
  NOTE_FS6,
  NOTE_G6,
  NOTE_GS6,
  NOTE_A6,
  NOTE_AS6,
  NOTE_B6,
  NOTE_C7,
  NOTE_CS7,
  NOTE_D7,
  NOTE_DS7,
  NOTE_E7,
  NOTE_F7,
  NOTE_FS7,
  NOTE_G7,
  NOTE_GS7,
  NOTE_A7,
  NOTE_AS7,
  NOTE_B7,
  NOTE_C8,
  NOTE_CS8,
  NOTE_D8,
  NOTE_DS8,
};

Voice::Voice(int voicePin) {
  m_voicePin = voicePin;
}

int Voice::setMajor7th(int startIndex, int root) {
  int len = 4;

  int notes[len] = {
    singingPitches[root],
    singingPitches[root+4],
    singingPitches[root+7],
    singingPitches[root+11]
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
    singingPitches[root],
    singingPitches[root + third],
    singingPitches[root + 7]
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
    melody[i] = singingPitches[rand];
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
  if (currNoteIndex == -1 || now - noteStart > rhythm[currNoteIndex]) {
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

void Voice::demo() {
  if (playing) {
    tone(m_voicePin, 1047);
  } else {
    noTone(m_voicePin);
  }

  playing = !playing;
}

void Voice::handleNoteOn(byte channel, byte note, byte velocity) {
  playing = false;
  tone(m_voicePin, sNotePitches[note - 23]);
}

void Voice::handleNoteOff(byte channel, byte note, byte velocity) {
  playing = false;
  noTone(m_voicePin);
}