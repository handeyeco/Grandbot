![Grandbot smiling](grandbot.jpg)

# Grandbot

Grandbot is a robot grand-child for my parents. He's my my take on [Mohit Bhoite's Chintoo](https://twitter.com/MohitBhoite/status/1241752124087660546).

- [Demo](https://www.youtube.com/watch?v=sPIsFCZO8Fg)
- [Synth jams](https://www.youtube.com/watch?v=bJeONx7aBzU)
- [Build video](https://www.youtube.com/watch?v=zd_m1_4YeL4)

## Features

- Grandbot is a generative, pattern-based MIDI arpeggiator
- He has different moods based on his self-esteem. These moods affect everything from his expressions, the color of his light, and the sounds he plays.
  - To increase his self-esteem, it's important to give him plenty of attention. He currently can be interacted with using a "play" button; if you play with him, he feels better about himself and he gets happier.
  - He doesn't like to be ignored though. If you don't play with him regularly, he starts to feel unimportant. If his self-esteem drops too low, he gets depressed.
- He uses a photoresistor to determine when to go to sleep and when to wake up.
- He has an onboard passive buzzer and uses it to make music. The music is mostly randomly generated and is influenced by his mood.
- He likes to set mood lighting using an RGB LED as another indicator of how he's feeling.

## Arp

I had extra memory, so I decided to program Grandbot to play synths with me. He's now a chaotic, generative, pattern-based arpeggiator.

This requires the additional MIDI board for Grandbot.

The simplest use:

- Plug something that can send MIDI notes and a MIDI clock to Grandbot's MIDI input
- Plus Grandbot's MIDI out into a synth
- Start the MIDI clock and play some notes
- Press the button to generate a new sequence

There's endless potential just doing that. If you'd like more control, there are additional parameters that can be changed using MIDI CC or the optional Grandbot control board:

| Description                     | MIDI CC | Display | Button |
| ------------------------------- | ------- | ------- | ------ |
| **» Sequence Parameters**       |
| Base gate length                | 104     | GL      |
| Base note length                | 20      | nL      |
| Sequence length                 | 21      | SL      |
| **» Transposition Chance**      |
| One octave up chance            | 22      | o-      |
| One octave down chance          | 23      | o-      |
| Two octaves up chance           | 24      | o=      |
| Two octaves down chance         | 25      | o=      |
| Fifth up chance                 | 85      | Ft      |
| Random transposition chance     | 86      | rn      |
| Rest chance                     | 29      | rE      |
| **» Note Length Chance**        |
| Random gate length chance       | 105     | rG      |
| Double length chance            | 26      | dL      |
| Half length chance              | 27      | HL      |
| Ratchet (double note) chance    | 28      | rA      |
| Legato/slide chance             | 31      | LE      |
| Random length chance            | 87      | rL      |
| Note run chance                 | 30      | rU      |
| **» Post Generate**             |
| Collapse notes                  | 19      | Co      |
| **» Generate / Slip**           |
| Queue sequence generation       | 118     |         | UP                 |
| Randomize sequence params       | 9       |         | SHIFT + UP         |
| Reset sequence params           | 102     |         | SHIFT + RIGHT      |
| Queue sequence slip             | 116     |         | DOWN               |
| Toggle drift (endless slip)     | 3       |         | SHIFT + DOWN       |
| Slip chance                     | 89      | SC      |
| **» Utilities**                 |
| Latch                           | 90      | LA      |
| Swing (50-67%)                  | 115     | SG      |
| Transpose                       | 103     | t       |
| Note sort                       | 114     | So      |
| Random velocity depth           | 106     | vE      |
| Velocity high                   | 107     | vH      |
| Velocity low                    | 108     | vL      |
| Select MIDI channel in          | 14      | In      |
| Select MIDI channel out         | 15      | ot      |
| Toggle internal speaker for arp | 119     | SP      |
| Clock: external or internal     | 112     | CL      |
| BPM for internal clock (73-200) | 113     | b       |
| Panic / all notes off           | 117     | AH      | SHIFT + LEFT       |
| Play (internal clock)           |         |         | FORWARD            |
| Stop (internal clock)           |         |         | BACKWARD           |
| Enter menu                      |         |         | FORWARD + BACKWARD |

[Full guide](guide.md)

![Grandbot with buttons](grandbot-buttons.jpg)

### Button Combos

With the optional control board, six buttons are added for controlling the Arp settings.

Buttons:
- `˄`: UP
- `˃`: RIGHT
- `˅`: DOWN
- `˂`: LEFT
- `˃˃`: FORWARD (SHIFT)
- `˂˂`: BACKWARD

When outside the menu:
- UP: queue sequence generation
- SHIFT + UP: randomize sequence settings
- DOWN: queue slip
- SHIFT + DOWN: enable drift (endless slip)
- LEFT: play with Grandbot (not related to the Arp)
- SHIFT + LEFT: panic
- SHIFT + RIGHT: reset sequence settings to default
- FORWARD: play (when using internal clock)
- BACKWARD: stop (when using internal clock)
- SHIFT + BACKWARD: enter menu

When inside the menu:
- UP: increment current setting
- SHIFT + UP: increment current setting (fast)
- DOWN: decrement current setting
- SHIFT + DOWN: decrement current setting (fast)
- LEFT: select current setting
- RIGHT: select current setting
- FORWARD: enter sub-menu
- BACKWARD: exit sub-menu
- SHIFT + BACKWARD: exit menu

## Tools

- [Tool to control the arp via MIDI CC](https://handeyeco.github.io/grandbot-web-controller/) ([Source](https://github.com/handeyeco/grandbot-web-controller))
- [Tool to convert display settings to bytes](https://handeyeco.github.io/grandbot-designer/) ([Source](https://github.com/handeyeco/grandbot-designer))

## Blog Posts

Grandbot development is what I'm using to learn more about electronics and C++. I write about what I've been learning on my blog. Here are the Grandbot posts:

- [Grandbot: Sleep](https://handeyeco.github.io/tech-blog/grandbot-update-sleep/)
- [Grandbot: Voice](https://handeyeco.github.io/tech-blog/grandbot-update-voice/)
- [Grandbot: Play](https://handeyeco.github.io/tech-blog/grandbot-update-play/)
- [Grandbot: Light](https://handeyeco.github.io/tech-blog/grandbot-update-light/)
- [Grandbot: MIDI & Misc](https://handeyeco.github.io/tech-blog/grandbot-update-midi/)
- [Grandbot: Arp](https://handeyeco.github.io/tech-blog/grandbot-update-arp/)
- [Grandbot: Buttons](https://handeyeco.github.io/tech-blog/grandbot-update-buttons/)
- [Concurrent Arduino](https://handeyeco.github.io/tech-blog/concurrent-arduino/)
- [4 Digit, 7 Segment Emoji](https://handeyeco.github.io/tech-blog/seven-segment-emoji/)
- [Max7219 Emoji](https://handeyeco.github.io/tech-blog/max7219-emoji/)

## Byte to digit

One digit of an expression looks like B00000000. For the first four digits, it maps to:

- B: specifying it's binary
- 0: the decimal point (DP)
- 0: top horizontal line
- 0: top right vertical line
- 0: bottom right vertical line
- 0: bottom horizontal line
- 0: bottom left vertical line
- 0: top left vertical line
- 0: center line

The fifth digit:

- B: specifying it's binary
- 0: the center colon
- the reset are unused

The sixth digit:

- B: specifying it's binary
- 0: the top dot
- the reset are unused

## Nano vs Nano Every

This was originally built on a cheap Arduino Nano clone.

However I wanted to be able to support 8 bars of 32nd notes in the arpeggiator (the minimum base note length is 16th, but ratchets break that into 32nds) and the Arduino Nano didn't have enough memory. The [Arduino Nano Every](https://store.arduino.cc/products/arduino-nano-every) does! **It's a different board with a similar name.**
