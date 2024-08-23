#include <inttypes.h>

#ifndef PITCHES_INCL_GUARD
#define PITCHES_INCL_GUARD

// B0 is the first note and its MIDI number is 23
// the first pitch in the array is 0,
// so use this to convert MIDI note to index
static const byte noteOffset = 23;

// An array of pitches from B0 to D8
static const uint16_t noteToPitchMap[88] = {
    31,  // B0 - 23
    33,   35,   37,   39,   41,   44,   46,   49,   52,   55,   58,   62,
    65,   69,   73,   78,   82,   87,   93,   98,   104,  110,  117,  123,
    131,  139,  147,  156,  165,  175,  185,  196,  208,  220,  233,  247,
    262,  // C4 - 60
    277,  294,  311,  330,  349,  370,  392,  415,
    440,  // A4 - 69
    466,  494,  523,  554,  587,  622,  659,  698,  740,  784,  831,  880,
    932,  988,  1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760,
    1865, 1976, 2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520,
    3729, 3951, 4186, 4435,
    4699,  // D8 - 110
};

/**
 * Convert a MIDI note (23-110) to a pitch (31Hz-4699Hz aka B0-D8)
 * used to convert MIDI to buzzer sounds
 *
 * @param {byte} note - the MIDI note in question
 * @returns {uint16_t} the pitch in Hz
 */
static uint16_t getPitchByNote(byte note) {
  // outside of note range
  if (note < 23 || note > 110) {
    return 0;
  }

  return noteToPitchMap[note - noteOffset];
}

#endif