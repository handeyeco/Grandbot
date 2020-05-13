#include <Expressions.h>

const int blank      = B00000000;
const int dot        = B10000000;
const int middleDash = B00000001;
const int lowDash    = B00001000;
const int lowDashDot = B10001000;
const int bigEye     = B01111110;
const int eyebrow    = B01011101;
const int plainU     = B00011100;
const int uEyebrow   = B01011100;
const int uTop       = B00100011;
const int tiredEye   = B01101011;
const int lilEyeHigh = B01100011;
const int lilEyeLow  = B00011101;

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

int sleeping3[4] = {
  uTop,
  middleDash,
  uTop,
  blank
};

int sleeping4[4] = {
  blank,
  blank,
  lowDashDot,
  lowDash
};

int bigEyeSmileLeft[4] = {
  bigEye,
  plainU,
  bigEye,
  blank
};

int smallSmileLeft[4] = {
  eyebrow,
  plainU,
  eyebrow,
  blank
};

int smileBlinkLeft[4] = {
  middleDash,
  plainU,
  middleDash,
  blank
};

int bigEyeSmileRight[4] = {
  blank,
  bigEye,
  plainU,
  bigEye
};

int smallSmileRight[4] = {
  blank,
  eyebrow,
  plainU,
  eyebrow
};

int smileBlinkRight[4] = {
  blank,
  middleDash,
  plainU,
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

int closeLowBlinking[4] = {
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

int splitLowBlinking[4] = {
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

int tiredEyesSplit[4] = {
  tiredEye,
  blank,
  blank,
  tiredEye
};

int splitMidBlinking[4] = {
  middleDash,
  blank,
  blank,
  middleDash
};


int tiredEyesClose[4] = {
  blank,
  tiredEye,
  tiredEye,
  blank
};

int closeMidBlinking[4] = {
  blank,
  middleDash,
  middleDash,
  blank
};

int splitLilEyesHigh[4] = {
  lilEyeHigh,
  blank,
  blank,
  lilEyeHigh
};

int splitLilEyesLow[4] = {
  lilEyeLow,
  blank,
  blank,
  lilEyeLow
};

int closeLilEyesHigh[4] = {
  blank,
  lilEyeHigh,
  lilEyeHigh,
  blank
};

int closeLilEyesLow[4] = {
  blank,
  lilEyeLow,
  lilEyeLow,
  blank
};

Expression Expressions::errorExpressions[] = {
  Expression(error, error)
};

Expression Expressions::sleepingExpressions[] = {
  Expression(sleeping, sleeping),
  Expression(sleeping2, sleeping2),
  Expression(sleeping3, sleeping3),
  Expression(sleeping4, sleeping4)
};

Expression Expressions::happyExpressions[] = {
  Expression(bigEyeSmileLeft, smileBlinkLeft),
  Expression(bigEyeSmileRight, smileBlinkRight),
  Expression(smallSmileLeft, smileBlinkLeft),
  Expression(smallSmileRight, smileBlinkRight)
};

Expression Expressions::neutralExpressions[] = {
  Expression(closeBigEyes, closeMidBlinking),
  Expression(splitBigEyes, splitMidBlinking),
  Expression(closeEyebrows, closeLowBlinking),
  Expression(splitEyebrows, splitLowBlinking),
  Expression(splitLilEyesHigh, splitMidBlinking),
  Expression(splitLilEyesLow, splitLowBlinking)
};

Expression Expressions::unhappyExpressions[] = {
  Expression(skeptical, splitLowBlinking),
  Expression(tiredEyesSplit, splitMidBlinking),
  Expression(tiredEyesClose, closeMidBlinking),
  Expression(closeLilEyesHigh, closeMidBlinking),
  Expression(closeLilEyesLow, closeLowBlinking)
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