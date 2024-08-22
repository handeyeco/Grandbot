#include <Arduino.h>
#include <Button.h>
#include <pin_defs.h>

#ifndef BUTTON_MANAGER_INCL_GUARD
#define BUTTON_MANAGER_INCL_GUARD

/**
 * State manager for all buttons
 */
struct ButtonManager {
private:
  Button *allButtons[7];

public:
  ButtonManager();
  void read();
  bool combo(Button &b1, Button &b2);

  bool anyPressed;
  bool anyReleased;
  bool anyHeld;

  // Vanilla Grandbot button
  Button play;

  // Grandbot controller buttons
  Button up;
  Button right;
  Button down;
  Button left;
  Button forward;
  Button backward;
};

#endif