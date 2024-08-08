#include <Button.h>

Button::Button(int _pin) : pin(_pin) {
  pinMode(_pin, INPUT_PULLUP);
}

void Button::reset() {
  prevPressed = false;
  pressed = false;
  held = false;
  released = false;
}

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
    released = true;
    held = false;
  }

  prevPressed = isPressed;
}