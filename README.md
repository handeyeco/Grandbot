# Grandbot

Grandbot is a robot grand-child for my parents. He's my my take on [Mohit Bhoite's Chintoo](https://twitter.com/MohitBhoite/status/1241752124087660546).

## Features

Grandbot is an ongoing project, but right now:

- Grandbot is a generative, pattern-based arpeggiator
- He has different moods based on his self-esteem. These moods affect everything from his expressions, the color of his light, and the sounds he plays.
  - To increase his self-esteem, it's important to give him plenty of attention. He currently can be interacted with using a "play" button; if you play with him, he feels better about himself and he gets happier.
  - He doesn't like to be ignored though. If you don't play with him regularly, he starts to feel unimportant. If his self-esteem drops too low, he gets depressed.
- He uses a photoresistor to determine when to go to sleep and when to wake up.
- He has an onboard passive buzzer and uses it to make music. The music is mostly randomly generated and is influenced by his mood.
- He likes to set mood lighting using an RGB LED as another indicator of how he's feeling.

## Arp

I had extra memory on the Nano, so I decided to program Grandbot to play synths with me. He's now a chaotic, generative, pattern-based arpeggiator.

This requires the additional MIDI board for Grandbot.

The simplest use:
- Plug something that can send MIDI notes and a MIDI clock to Grandbot's MIDI input
- Plus Grandbot's MIDI out into a synth
- Start the MIDI clock and play some notes
- Press the button to generate a new sequence

Additionally various parameters can be modified using MIDI CC:
- MIDI Channel in (14): select the MIDI channel to listen to
  - By default, Grandbot listens to all channels
- MIDI Channel out (15): select the MIDI channel to send to
  - By default, Grandbot sends to the same channel it received
- Base note length (20): base note length before length variation is added
  - By default, this is randomly set for each new sequence
- Sequence length (21): total sequence length
  - By default, this is randomly set for each new sequence
- Transposition randomness
  - One octave up chance (22)
  - One octave down chance (23)
  - Two octaves up chance (24)
  - Two octaves down chance (25)
  - Fifth up chance (85)
  - Random transposition chance (86)
- Length randomness
  - Double length chance (26)
  - Half length chance (27)
  - Ratchet (double note) chance (28)
  - Random length chance (87)
- Rest chance (29)
- Slipping is a gentle transformation that randomly swaps notes in a sequence
  - Slip note chance (89)
  - Trigger slip (116)
    - Going from around 0 to around 127 triggers it
    - Happens after the sequence has finished
- Generate sequence (118)
  - Going from around 0 to around 127 triggers it
  - Happens at the end of the next bar
- Panic tries to turn off all notes (117)
  - Going from around 0 to around 127 triggers it
- Use internal speaker to play arp (119)
  - Around 0: off - Around 127: on

> [!TIP]
> Changing variation chances only affects new sequences, not the current sequence. Press the button after making changes to hear the effects.

> [!WARNING] 
> Pretty much all of these are only affected if the commands happen on the selected MIDI in channel (by default Grandbot listens to all channels). However CC 14 (MIDI in selector) and CC 117 (Panic) listen for changes on all channels all the time.

## Tools
- [Tool to control the arp via MIDI CC](https://handeyeco.github.io/grandbot-web-controller/) ([Source](https://github.com/handeyeco/grandbot-web-controller))
- [Tool to convert display settings to bytes](https://handeyeco.github.io/grandbot-designer/) ([Source](https://github.com/handeyeco/grandbot-designer))

## Blog Posts

Grandbot development is what I'm using to learn more about electronics and C++. I write about what I've been learning on my blog. Here are the Grandbot posts:

- [4 Digit, 7 Segment Emoji](https://handeyeco.github.io/tech-blog/seven-segment-emoji/)
- [Max7219 Emoji](https://handeyeco.github.io/tech-blog/max7219-emoji/)
- [Grandbot Update - Sleep](https://handeyeco.github.io/tech-blog/grandbot-update-sleep/)
- [Grandbot Update - Voice](https://handeyeco.github.io/tech-blog/grandbot-update-voice/)
- [Grandbot Update - Play](https://handeyeco.github.io/tech-blog/grandbot-update-play/)
- [Grandbot Update - Light](https://handeyeco.github.io/tech-blog/grandbot-update-light/)
- [Concurrent Arduino](https://handeyeco.github.io/tech-blog/concurrent-arduino/)

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