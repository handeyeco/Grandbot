#include <Expression.h>

#ifndef EXPRESSIONS_INCL_GUARD
#define EXPRESSIONS_INCL_GUARD

int sleeping[4] = {
  B00000001,
  B10000000,
  B00000000,
  B00000001
};

int closeBigEyes[4] = {
  B00000000,
  B01111110,
  B01111110,
  B00000000
};

int closeEyebrows[4] = {
  B00000000,
  B01011101,
  B01011101,
  B00000000
};

int closeBlinking[4] = {
  B00000000,
  B00001000,
  B00001000,
  B00000000
};

int splitBigEyes[4] = {
  B01111110,
  B00000000,
  B00000000,
  B01111110
};

int splitEyebrows[4] = {
  B01011101,
  B00000000,
  B00000000,
  B01011101
};

int splitBlinking[4] = {
  B00001000,
  B00000000,
  B00000000,
  B00001000
};

int skeptical[4] = {
  B01011101,
  B00001000,
  B00000000,
  B01111110
};

Expression expressions[] = {
  Expression(sleeping, sleeping),
  Expression(closeBigEyes, closeBlinking),
  Expression(closeEyebrows, closeBlinking),
  Expression(splitBigEyes, splitBlinking),
  Expression(splitEyebrows, splitBlinking),
  Expression(skeptical, splitBlinking)
};

#endif