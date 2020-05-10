#include <Expressions.h>
#include <Expression.h>

const int blank      = B00000000;
const int dot        = B10000000;
const int middleDash = B00000001;
const int lowDash    = B00001000;
const int bigEye     = B01111110;
const int eyebrow    = B01011101;

int sleeping[4] = {
  middleDash,
  dot,
  blank,
  middleDash
};

int closeBigEyes[4] = {
  blank,
  bigEye,
  bigEye,
  blank
};

int closeEyebrows[4] = {
  blank,
  eyebrow,
  eyebrow,
  blank
};

int closeBlinking[4] = {
  blank,
  lowDash,
  lowDash,
  blank
};

int splitBigEyes[4] = {
  bigEye,
  blank,
  blank,
  bigEye
};

int splitEyebrows[4] = {
  eyebrow,
  blank,
  blank,
  eyebrow
};

int splitBlinking[4] = {
  lowDash,
  blank,
  blank,
  lowDash
};

int skeptical[4] = {
  eyebrow,
  lowDash,
  blank,
  bigEye
};

Expression Expressions::expressions[] = {
  Expression(sleeping, sleeping),

  Expression(closeBigEyes, closeBlinking),
  Expression(splitBigEyes, splitBlinking),

  Expression(closeEyebrows, closeBlinking),
  Expression(splitEyebrows, splitBlinking),

  Expression(skeptical, splitBlinking)
};

int Expressions::getSleepyIndex() {
  return 0;
}

int Expressions::getHappyIndex() {
  return random(1, 3);
}

int Expressions::getNeutralIndex() {
  return random(3, 5);
}

int Expressions::getUpsetIndex() {
  return 5;
}