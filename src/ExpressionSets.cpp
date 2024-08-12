#include <ExpressionSets.h>

const byte blank = B00000000;
const byte dot = B10000000;
const byte middleDash = B00000001;
const byte lowDash = B00001000;
const byte lowDashDot = B10001000;
const byte bigEye = B01111110;
const byte eyebrow = B01011101;
const byte plainU = B00011100;
const byte uEyebrow = B01011100;
const byte uTop = B00100011;
const byte tiredEye = B01101011;
const byte lilEyeHigh = B01100011;
const byte lilEyeLow = B00011101;

byte fullBlank[4] = {
    blank, blank, blank, blank
};

byte error[4] = {
    CHAR_E,
    CHAR_R,
    CHAR_R,
    blank};

byte sleeping[4] = {
    middleDash,
    dot,
    blank,
    middleDash};

byte sleeping2[4] = {
    uEyebrow,
    blank,
    blank,
    uEyebrow};

byte sleeping3[4] = {
    uTop,
    middleDash,
    uTop,
    blank};

byte sleeping4[4] = {
    blank,
    blank,
    lowDashDot,
    lowDash};

byte bigEyeSmileLeft[4] = {
    bigEye,
    plainU,
    bigEye,
    blank};

byte smallSmileLeft[4] = {
    eyebrow,
    plainU,
    eyebrow,
    blank};

byte smileBlinkLeft[4] = {
    middleDash,
    plainU,
    middleDash,
    blank};

byte bigEyeSmileRight[4] = {
    blank,
    bigEye,
    plainU,
    bigEye};

byte smallSmileRight[4] = {
    blank,
    eyebrow,
    plainU,
    eyebrow};

byte smileBlinkRight[4] = {
    blank,
    middleDash,
    plainU,
    middleDash};

byte closeBigEyes[4] = {
    blank,
    bigEye,
    bigEye,
    blank};

byte closeEyebrows[4] = {
    blank,
    eyebrow,
    eyebrow,
    blank};

byte closeLowBlinking[4] = {
    blank,
    lowDash,
    lowDash,
    blank};

byte splitBigEyes[4] = {
    bigEye,
    blank,
    blank,
    bigEye};

byte splitEyebrows[4] = {
    eyebrow,
    blank,
    blank,
    eyebrow};

byte splitLowBlinking[4] = {
    lowDash,
    blank,
    blank,
    lowDash};

byte skeptical[4] = {
    eyebrow,
    lowDash,
    blank,
    bigEye};

byte tiredEyesSplit[4] = {
    tiredEye,
    blank,
    blank,
    tiredEye};

byte splitMidBlinking[4] = {
    middleDash,
    blank,
    blank,
    middleDash};

byte tiredEyesClose[4] = {
    blank,
    tiredEye,
    tiredEye,
    blank};

byte closeMidBlinking[4] = {
    blank,
    middleDash,
    middleDash,
    blank};

byte splitLilEyesHigh[4] = {
    lilEyeHigh,
    blank,
    blank,
    lilEyeHigh};

byte splitLilEyesLow[4] = {
    lilEyeLow,
    blank,
    blank,
    lilEyeLow};

byte closeLilEyesHigh[4] = {
    blank,
    lilEyeHigh,
    lilEyeHigh,
    blank};

byte closeLilEyesLow[4] = {
    blank,
    lilEyeLow,
    lilEyeLow,
    blank};

Expression ExpressionSets::errorExpressions[] = {
    Expression(error, error)};
int ExpressionSets::errorLength = sizeof(ExpressionSets::errorExpressions) / sizeof(errorExpressions[0]);

Expression ExpressionSets::sleepingExpressions[] = {
    Expression(sleeping, sleeping),
    Expression(sleeping2, sleeping2),
    Expression(sleeping3, sleeping3),
    Expression(sleeping4, sleeping4)};
int ExpressionSets::sleepingLength = sizeof(ExpressionSets::sleepingExpressions) / sizeof(sleepingExpressions[0]);

Expression ExpressionSets::happyExpressions[] = {
    Expression(bigEyeSmileLeft, smileBlinkLeft),
    Expression(bigEyeSmileRight, smileBlinkRight),
    Expression(smallSmileLeft, smileBlinkLeft),
    Expression(smallSmileRight, smileBlinkRight)};
int ExpressionSets::happyLength = sizeof(ExpressionSets::happyExpressions) / sizeof(happyExpressions[0]);

Expression ExpressionSets::neutralExpressions[] = {
    Expression(closeBigEyes, closeMidBlinking),
    Expression(splitBigEyes, splitMidBlinking),
    Expression(closeEyebrows, closeLowBlinking),
    Expression(splitEyebrows, splitLowBlinking),
    Expression(splitLilEyesHigh, splitMidBlinking),
    Expression(splitLilEyesLow, splitLowBlinking)};
int ExpressionSets::neutralLength = sizeof(ExpressionSets::neutralExpressions) / sizeof(neutralExpressions[0]);

Expression ExpressionSets::unhappyExpressions[] = {
    Expression(skeptical, splitLowBlinking),
    Expression(tiredEyesSplit, splitMidBlinking),
    Expression(tiredEyesClose, closeMidBlinking),
    Expression(closeLilEyesHigh, closeMidBlinking),
    Expression(closeLilEyesLow, closeLowBlinking)};
int ExpressionSets::unhappyLength = sizeof(ExpressionSets::unhappyExpressions) / sizeof(unhappyExpressions[0]);

// ARP STUFF #TODO move this
Expression ExpressionSets::midiBeatExpressions[] = {
    Expression(bigEyeSmileLeft, smileBlinkLeft),
    Expression(bigEyeSmileRight, smileBlinkRight)};

byte ExpressionSets::convertNumberToByte(byte num) {
  switch (num) {
    case 0:
      return CHAR_0;
    case 1:
      return CHAR_1;
    case 2:
      return CHAR_2;
    case 3:
      return CHAR_3;
    case 4:
      return CHAR_4;
    case 5:
      return CHAR_5;
    case 6:
      return CHAR_6;
    case 7:
      return CHAR_7;
    case 8:
      return CHAR_8;
    case 9:
      return CHAR_9;
    default:
      return CHAR_BLANK;
    }
}