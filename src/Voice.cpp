#include <Voice.h>

const byte Voice::singingNotes[27] = {
    84, // C6
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
    110, // D8
};

Voice::Voice(){}

/**
 * Sets the melody/rhythm to a Major 7th sequence
 * #TODO remove startIndex?
 *
 * @param {int} startIndex - where in the sequence to start
 * @param {int} root - the root note of the chord
 * @returns {int} the length of the sequence
*/
int Voice::setMajor7th(int startIndex, int root)
{
  int len = 4;

  byte notes[len] = {
      singingNotes[root],       // Root
      singingNotes[root + 4],   // Major 3rd
      singingNotes[root + 7],   // Fifth
      singingNotes[root + 11]}; // Major 7th

  for (int i = 0; i < len; i++)
  {
    melody[startIndex + i] = notes[i];
    rhythm[startIndex + i] = 250;
  }

  return len;
}

/**
 * Sets the melody/rhythm to a triad sequence
 *
 * @param {int} startIndex - where in the sequence to start
 * @param {int} root - the root note of the chord
 * @param {bool} major - whether it'll be major or minor
 * @returns {int} the length of the sequence
*/
int Voice::setTriad(int startIndex, int root, bool major)
{
  byte len = 3;
  // Set the 3rd to a major or minor 3rd
  byte third = major ? 4 : 3;

  byte notes[len] = {
      singingNotes[root],         // Root
      singingNotes[root + third], // 3rd
      singingNotes[root + 7]};    // Fifth

  byte noteLenFlip = random(0, 4);
  int noteLen = (noteLenFlip * 100) + 100;
  for (byte i = 0; i < len; i++)
  {
    melody[startIndex + i] = notes[i];
    rhythm[startIndex + i] = noteLen;
  }

  return len;
}

/**
 * Create a harmonic melody using a major scale
 *
 * @returns {int} the length of the sequence
*/
int Voice::setSong()
{
  // Root note
  int start = random(0, 12);
  // How many chords to add to the melody
  int chords = random(4, 9);

  int interval;
  bool major;
  int melodyIndex = 0;
  for (int i = 0; i < chords; i++)
  {
    // interval within a major (not chromatic) scale
    interval = random(0, 7);
    // use a major chord for I, IV, and V; minor for ii, iii, vi, vii
    // (even though the vii should be diminished)
    major = interval == 0 || interval == 3 || interval == 4;
    melodyIndex += setTriad(melodyIndex, start + interval, major);
  }

  return melodyIndex;
}

/**
 * Create a random melody
 *
 * @param {byte} length - the length of the sequence
 * @returns {int} the length of the sequence
*/
byte Voice::setRandomSequence(byte len)
{
  for (byte i = 0, rand; i < len; i++)
  {
    rand = random(0, 27);
    melody[i] = singingNotes[rand];
    rhythm[i] = 75;
  }

  return len;
}

/**
 * Uphappy sequence is always the same ho-hum
 *
 * @returns {int} the length of the sequence
*/
int Voice::setUnhappy()
{
  melody[0] = 56;
  melody[1] = 45;
  rhythm[0] = 1000;
  rhythm[1] = 1000;

  return 2;
}

/**
 * Queue the melody to play
 *
 * @param {int} playLength - length of the new sequence
*/
void Voice::play(int playLength)
{
  playing = true;
  melodyLength = playLength;
  currNoteIndex = -1;
}

/**
 * Queue the melody to play
 *
 * @param {int} mood - enum for emotional state as defined in Grandbot.h
 * @param {byte} esteem - a finer resolution of mood
*/
void Voice::emote(int mood, byte esteem)
{
  int len = 0;

  switch (mood)
  {
  // Sleeping - Major 7th sequence
  case 0:
    len = setMajor7th(0, random(0, 12));
    break;
  // Happy - a song based on a major scale
  case 1:
    len = setSong();
    break;
  // Neutral - random sequence with a length
  // based on his esteem
  case 2:
    len = setRandomSequence(esteem);
    break;
  // Unhappy - ho-hum sequence
  case 3:
    len = setUnhappy();
    break;
  }

  play(len);
}

/**
 * Update to be called during the Arduino update cycle.
 * Handles actually playing the melody
*/
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
      byte note = melody[currNoteIndex];
      uint16_t pitch = getPitchByNote(note);
      tone(BUZZER_PIN, pitch);
      noteStart = now;
      return;
    }
    else
    {
      // end of sequence, turn off buzzer
      noTone(BUZZER_PIN);
      playing = false;
    }
  }
}