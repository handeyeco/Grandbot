#include <Button.h>

Button::Button(int _pin) : pin(_pin) { pinMode(_pin, INPUT_PULLUP); }

/**
 * Read and update button state
 */
void Button::read() {
  // active LOW
  bool isPressed = !digitalRead(pin);
  pressed = false;
  released = false;

  if (isPressed && !prevPressed) {
    pressed = true;
    held = true;
  } else if (!isPressed && prevPressed) {
    if (ignoreRelease) {
      ignoreRelease = false;
    } else {
      released = true;
    }

    held = false;
  }

  prevPressed = isPressed;
}