#include <Button.h>

Button::Button(int _pin) : pin(_pin) {
  pinMode(_pin, INPUT_PULLUP);
  state = digitalRead(pin);
  debounceStart = millis();
}

/**
 * Read and update button state
 */
void Button::read() {
  unsigned long now = millis();
  bool read = digitalRead(pin);

  pressed = false;
  released = false;

  if (read != state) {
    if (now - debounceStart > DEBOUNCE_TIMEOUT) {
      state = read;

      // pressed
      if (state == LOW) {
        pressed = true;
        held = true;
      }
      //released
      else {
        released = ignoreRelease ? false : true;
        ignoreRelease = false;
        held = false;
      }
    }
  } else {
    debounceStart = now;
  }
}