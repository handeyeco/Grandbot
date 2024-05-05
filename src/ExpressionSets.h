#include <Expression.h>

#ifndef EXPRESSION_SETS_INCL_GUARD
#define EXPRESSION_SETS_INCL_GUARD

#define CHAR_A B01110111
#define CHAR_B B00011111
#define CHAR_C B01001110
#define CHAR_D B00111101
#define CHAR_E B01001111
#define CHAR_F B01000111
#define CHAR_G B01011110
#define CHAR_H B00110111
#define CHAR_I B00110000
#define CHAR_J B00111100
#define CHAR_L B00001110
#define CHAR_N B00010101
#define CHAR_O B00011101
#define CHAR_P B01100111
#define CHAR_R B00000101
#define CHAR_S B01011011
#define CHAR_T B00001111
#define CHAR_U B00011100
#define CHAR_Y B00111011

#define CHAR_0 B01111110
#define CHAR_1 B00110000
#define CHAR_2 B01101101
#define CHAR_3 B01111001
#define CHAR_4 B00110011
#define CHAR_5 B01011011
#define CHAR_6 B01011111
#define CHAR_7 B01110000
#define CHAR_8 B01111111
#define CHAR_9 B01111011

#define CHAR_BLANK B00000000

/**
 * A collection of different Expression structs,
 * sorted by their emotion state
*/
struct ExpressionSets {
  public:
    static Expression errorExpressions[];
    static int errorLength;
    static Expression sleepingExpressions[];
    static int sleepingLength;
    static Expression happyExpressions[];
    static int happyLength;
    static Expression neutralExpressions[];
    static int neutralLength;
    static Expression unhappyExpressions[];
    static int unhappyLength;
    static Expression midiBeatExpressions[];
};

#endif