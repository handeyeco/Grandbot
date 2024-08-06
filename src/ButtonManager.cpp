#include <ButtonManager.h>

ButtonManager::ButtonManager() :
  play(PLAY_BUTTON_PIN),
  up(UP_BUTTON_PIN),
  right(RIGHT_BUTTON_PIN),
  down(RIGHT_BUTTON_PIN),
  left(LEFT_BUTTON_PIN),
  forward(FORWARD_BUTTON_PIN),
  backward(BACKWARD_BUTTON_PIN),
  allButtons({&play, &up, &right, &down, &left, &forward, &backward}) {}

/**
 * Read and update button state
*/
void ButtonManager::read() {
  for (byte i = 0; i < 7; i++) {
    allButtons[i]->read();
  }
}