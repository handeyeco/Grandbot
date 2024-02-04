#include <Voice.h>

const int Voice::singingNotes[27] = {
    84,
    85,
    86,
    87,
    88,
    89,
    90,
    91,
    92,
    93,
    94,
    95,
    96,
    97,
    98,
    99,
    100,
    101,
    102,
    103,
    104,
    105,
    106,
    107,
    108,
    109,
    110,
};

Voice::Voice(int voicePin)
{
  m_voicePin = voicePin;
}

int Voice::setMajor7th(int startIndex, int root)
{
  int len = 4;

  int notes[len] = {
      singingNotes[root],
      singingNotes[root + 4],
      singingNotes[root + 7],
      singingNotes[root + 11]};

  for (int i = 0; i < len; i++)
  {
    melody[startIndex + i] = notes[i];
    rhythm[startIndex + i] = 250;
  }

  return len;
}

int Voice::setTriad(int startIndex, int root, boolean major)
{
  int len = 3;
  int third = major ? 4 : 3;

  int notes[len] = {
      singingNotes[root],
      singingNotes[root + third],
      singingNotes[root + 7]};

  int noteLenFlip = random(0, 4);
  int noteLen = noteLenFlip == 0   ? 100
                : noteLenFlip == 1 ? 200
                : noteLenFlip == 2 ? 300
                                   : 400;
  for (int i = 0; i < len; i++)
  {
    melody[startIndex + i] = notes[i];
    rhythm[startIndex + i] = noteLen;
  }

  return len;
}

int Voice::setSong()
{
  int start = random(0, 12);
  int chords = random(4, 9);

  int interval;
  boolean major;
  int melodyIndex = 0;
  for (int i = 0; i < chords; i++)
  {
    interval = random(0, 7);
    major = interval == 0 || interval == 3 || interval == 4;
    melodyIndex += setTriad(melodyIndex, start + interval, major);
  }

  return melodyIndex;
}

int Voice::setRandomSequence(int len)
{
  for (int i = 0, rand; i < len; i++)
  {
    rand = random(0, 27);
    melody[i] = singingNotes[rand];
    rhythm[i] = 75;
  }

  return len;
}

int Voice::setUnhappy()
{
  melody[0] = 56;
  melody[1] = 45;
  rhythm[0] = 1000;
  rhythm[1] = 1000;

  return 2;
}

void Voice::play(int playLength)
{
  playing = true;
  melodyLength = playLength;
  currNoteIndex = -1;
}

void Voice::emote(int mood, int esteem)
{
  int len = 0;

  switch (mood)
  {
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

void Voice::update()
{
  if (!playing)
    return;

  unsigned long now = millis();
  if (currNoteIndex == -1 || now - noteStart > rhythm[currNoteIndex])
  {
    currNoteIndex++;

    if (currNoteIndex < melodyLength)
    {
      uint16_t pitch = getPitchByNote(melody[currNoteIndex]);
      tone(m_voicePin, pitch);
      noteStart = now;
      return;
    }
    else
    {
      noTone(m_voicePin);
      playing = false;
    }
  }
}

void Voice::demo()
{
  if (playing)
  {
    tone(m_voicePin, 1047);
  }
  else
  {
    noTone(m_voicePin);
  }

  playing = !playing;
}