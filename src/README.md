Uses:
- Arduino
- [Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library)
- [LedControl](https://github.com/wayoda/LedControl)

Built with [PlatformIO](https://platformio.org/) using a cheap Arduino Nano clone.

Main files:
- `main`: the main Arduino `setup`/`loop` setup
- `Grandbot`: handles main Grandbot state
- Expression files (I don't know why there are so many)
  - `Expression`: struct for pairing normal and blink expressions
  - `ExpressionSets`: collection of `Expression` structs grouped by emotion
  - `Expressions`: the main manager for expressions and the 4 digit, 7 segment display
- `Light`: controls the RGB LED state
- `Voice`: controls the buzzer and audio emotes
- `Arp`: optional MIDI arpeggiator