#include <ButtonManager.h>

ButtonManager::ButtonManager()
    : play(PLAY_BUTTON_PIN),
      up(UP_BUTTON_PIN),
      right(RIGHT_BUTTON_PIN),
      down(DOWN_BUTTON_PIN),
      left(LEFT_BUTTON_PIN),
      forward(FORWARD_BUTTON_PIN),
      backward(BACKWARD_BUTTON_PIN),
      allButtons({&play, &up, &right, &down, &left, &forward, &backward}) {}

/**
 * Read and update button state
 */
void ButtonManager::read() {
  anyPressed = false;
  anyReleased = false;
  anyHeld = false;

  for (byte i = 0; i < 7; i++) {
    allButtons[i]->read();

    if (allButtons[i]->pressed) {
      anyPressed = true;
    } else if (allButtons[i]->released) {
      anyReleased = true;
    }

    if (allButtons[i]->held) {
      anyHeld = true;
    }
  }
}

/**
 * Determine if we triggered a button combo
 *
 * @param {Button&} b1 - button that might be part of combo
 * @param {Button&} b2 - button that might be part of combo
 * @returns {bool} if the buttons triggered a combo
 */
bool ButtonManager::combo(Button& b1, Button& b2) {
  if ((b1.pressed && b2.held) || (b2.pressed && b1.held)) {
    b1.ignoreRelease = true;
    b2.ignoreRelease = true;
    return true;
  }

  return false;
}