#include <Expressions.h>

const byte blank      = B00000000;
const byte dot        = B10000000;
const byte middleDash = B00000001;
const byte lowDash    = B00001000;
const byte lowDashDot = B10001000;
const byte bigEye     = B01111110;
const byte eyebrow    = B01011101;
const byte plainU     = B00011100;
const byte uEyebrow   = B01011100;
const byte uTop       = B00100011;
const byte tiredEye   = B01101011;
const byte lilEyeHigh = B01100011;
const byte lilEyeLow  = B00011101;

 byte error[4] = {
  B01001111,
  B00000101,
  B00000101,
  blank
};

 byte sleeping[4] = {
  middleDash,
  dot,
  blank,
  middleDash
};

 byte sleeping2[4] = {
  uEyebrow,
  blank,
  blank,
  uEyebrow
};

 byte sleeping3[4] = {
  uTop,
  middleDash,
  uTop,
  blank
};

 byte sleeping4[4] = {
  blank,
  blank,
  lowDashDot,
  lowDash
};

 byte bigEyeSmileLeft[4] = {
  bigEye,
  plainU,
  bigEye,
  blank
};

 byte smallSmileLeft[4] = {
  eyebrow,
  plainU,
  eyebrow,
  blank
};

 byte smileBlinkLeft[4] = {
  middleDash,
  plainU,
  middleDash,
  blank
};

 byte bigEyeSmileRight[4] = {
  blank,
  bigEye,
  plainU,
  bigEye
};

 byte smallSmileRight[4] = {
  blank,
  eyebrow,
  plainU,
  eyebrow
};

 byte smileBlinkRight[4] = {
  blank,
  middleDash,
  plainU,
  middleDash
};

 byte closeBigEyes[4] = {
  blank,
  bigEye,
  bigEye,
  blank
};

 byte closeEyebrows[4] = {
  blank,
  eyebrow,
  eyebrow,
  blank
};

 byte closeLowBlinking[4] = {
  blank,
  lowDash,
  lowDash,
  blank
};

 byte splitBigEyes[4] = {
  bigEye,
  blank,
  blank,
  bigEye
};

 byte splitEyebrows[4] = {
  eyebrow,
  blank,
  blank,
  eyebrow
};

 byte splitLowBlinking[4] = {
  lowDash,
  blank,
  blank,
  lowDash
};

 byte skeptical[4] = {
  eyebrow,
  lowDash,
  blank,
  bigEye
};

 byte tiredEyesSplit[4] = {
  tiredEye,
  blank,
  blank,
  tiredEye
};

 byte splitMidBlinking[4] = {
  middleDash,
  blank,
  blank,
  middleDash
};


 byte tiredEyesClose[4] = {
  blank,
  tiredEye,
  tiredEye,
  blank
};

 byte closeMidBlinking[4] = {
  blank,
  middleDash,
  middleDash,
  blank
};

 byte splitLilEyesHigh[4] = {
  lilEyeHigh,
  blank,
  blank,
  lilEyeHigh
};

 byte splitLilEyesLow[4] = {
  lilEyeLow,
  blank,
  blank,
  lilEyeLow
};

 byte closeLilEyesHigh[4] = {
  blank,
  lilEyeHigh,
  lilEyeHigh,
  blank
};

 byte closeLilEyesLow[4] = {
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

Expression* Expressions::getExpression(int mood) {
  int i = 0;
  int length;

  switch(mood) {
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

byte segments[8] = {
  B00000001,
  B00000010,
  B00000100,
  B00001000,
  B00010000,
  B00100000,
  B01000000,
  B10000000,
};

byte* Expressions::getDemo(int demoIndex) {
  return &segments[demoIndex];
}