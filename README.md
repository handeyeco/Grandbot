# Grandbot

Grandbot is a robot grand-child for my parents. He's my implementation of [Mohit Bhoite's Chintoo](https://twitter.com/MohitBhoite/status/1241752124087660546).

## Features

Grandbot is an ongoing project, but right now:

- Grandbot has different moods based on his self-esteem. These moods affect everything from his expressions, the color of his light, and the sounds he plays.
  - To increase his self-esteem, it's important to give him plenty of attention. He currently can be interacted with using a "play" button; if you play with him, he feels better about himself and he gets happier.
  - He doesn't like to be ignored though. If you don't play with him regularly, he starts to feel unimportant. If his self-esteem drops too low, he gets depressed.
- He uses a photoresistor to determine when to go to sleep and when to wake up.
- He has an onboard passive buzzer and uses it to make music. The music is mostly randomly generated and is influenced by his mood.
- He likes to set mood lighting using an RGB LED as another indicator of how he's feeling.

## Schematic

![Grandbot schematic](https://raw.githubusercontent.com/handeyeco/Grandbot/master/schematic.jpeg)

Uses:
- Arduino Uno
- Max7219
- TDCG1060M common cathode 4-digit, 7-segment display

## Blog Posts

Grandbot development is what I'm using to learn more about electronics and C++. I write about what I've been learning on my blog. Here are the Grandbot posts:

- [4 Digit, 7 Segment Emoji](https://handeyeco.github.io/tech-blog/seven-segment-emoji/)
- [Max7219 Emoji](https://handeyeco.github.io/tech-blog/max7219-emoji/)
- [Grandbot Update - Sleep](https://handeyeco.github.io/tech-blog/grandbot-update-sleep/)
- [Grandbot Update - Voice](https://handeyeco.github.io/tech-blog/grandbot-update-voice/)
- [Grandbot Update - Play](https://handeyeco.github.io/tech-blog/grandbot-update-play/)
- [Grandbot Update - Light](https://handeyeco.github.io/tech-blog/grandbot-update-light/)
- [Concurrent Arduino](https://handeyeco.github.io/tech-blog/concurrent-arduino/)