#include <ButtonManager.h>

ButtonManager::ButtonManager() :
  play(PLAY_BUTTON_PIN),
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