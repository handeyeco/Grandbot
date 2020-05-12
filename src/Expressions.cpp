#include <Expressions.h>

const int blank      = B00000000;
const int dot        = B10000000;
const int middleDash = B00000001;
const int lowDash    = B00001000;
const int bigEye     = B01111110;
const int eyebrow    = B01011101;
const int uEyebrow   = B01011100;

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

int sleeping2[4] = {
  uEyebrow,
  blank,
  blank,
  uEyebrow
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

Expression Expressions::errorExpressions[] = {
  Expression(error, error)
};

Expression Expressions::sleepingExpressions[] = {
  Expression(sleeping, sleeping),
  Expression(sleeping2, sleeping2)
};

Expression Expressions::happyExpressions[] = {
  Expression(closeBigEyes, closeBlinking),
  Expression(splitBigEyes, splitBlinking)
};

Expression Expressions::neutralExpressions[] = {
  Expression(closeEyebrows, closeBlinking),
  Expression(splitEyebrows, splitBlinking)
};

Expression Expressions::unhappyExpressions[] = {
  Expression(skeptical, splitBlinking)
};

Expression* Expressions::getExpression(int state) {
  int i = 0;
  int length;

  switch(state) {
    // Sleeping
    case 0:
      length = sizeof(sleepingExpressions) / sizeof(sleepingExpressions[0]);
      i = random(0, length);
      return &Expressions::sleepingExpressions[i];
    // Happy
    case 1:
      length = sizeof(happyExpressions) / sizeof(happyExpressions[0]);
      i = random(0, length);
      return &Expressions::happyExpressions[i];
    // Neutral
    case 2:
      length = sizeof(neutralExpressions) / sizeof(neutralExpressions[0]);
      i = random(0, length);
      return &Expressions::neutralExpressions[i];
    // Unhappy
    case 3:
      length = sizeof(unhappyExpressions) / sizeof(unhappyExpressions[0]);
      i = random(0, length);
      return &Expressions::unhappyExpressions[i];
    default:
      return &Expressions::unhappyExpressions[0];
  }
}