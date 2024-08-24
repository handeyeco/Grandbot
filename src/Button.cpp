#include <Button.h>

Button::Button(int _pin) : pin(_pin) {
  pinMode(_pin, INPUT_PULLUP);
}

void Button::update() {
  bool read = digitalRead(pin);

  if (read == 0) {
    Serial.println(pin);
  }

  history = history << 1;
  history |= (read == 0);

  pressed = 0;
  released = 0;

  // handle press
  if ((history & BUTTON_MASK) == 0b00000111) {
      pressed = 1;
      history = 0b11111111;
  }
  // handle release
  else if ((history & BUTTON_MASK) == 0b11000000) {
    released = ignoreRelease ? 0 : 1;
    ignoreRelease = false;
    history = 0b00000000;
  }

  held = history == 0b11111111;
}