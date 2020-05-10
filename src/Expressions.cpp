#include <Expressions.h>
#include <Expression.h>

const int blank      = B00000000;
const int dot        = B10000000;
const int middleDash = B00000001;
const int lowDash    = B00001000;
const int bigEye     = B01111110;
const int eyebrow    = B01011101;

int error[4] = {
  B01001111,
  B00000101,
  B00000101,
  blank
};

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
  Expression(error, error),

  // Sleeping
  Expression(sleeping, sleeping),

  // Happy
  Expression(closeBigEyes, closeBlinking),
  Expression(splitBigEyes, splitBlinking),

  // Neutral
  Expression(closeEyebrows, closeBlinking),
  Expression(splitEyebrows, splitBlinking),

  // Unhappy
  Expression(skeptical, splitBlinking)
};

Expression* Expressions::getExpression(int state) {
  int i = 0;

  switch(state) {
    // Sleeping
    case 0:
      i = 1;
      break;
    // Happy
    case 1:
      i = random(2, 4);
      break;
    // Neutral
    case 2:
      i = random(4, 6);
      break;
    // Unhappy
    case 3:
      i = 6;
      break;
  }

  return &Expressions::expressions[i];
}