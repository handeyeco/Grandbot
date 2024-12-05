- [Cheat sheet](#cheat-sheet)
- [Button Combos](#button-combos)
- [Menus](#menus)
- [Sequence menu](#sequence-menu)
  - [SL: Sequence length](#sl-sequence-length)
  - [SC: Slip chance](#sc-slip-chance)
  - [Co: Collapse notes](#co-collapse-notes)
- [Note menu](#note-menu)
  - [rE: Rest chance](#re-rest-chance)
  - [o-: One octave up chance](#o--one-octave-up-chance)
  - [o-: One octave down chance](#o--one-octave-down-chance)
  - [o=: Two octave up chance](#o-two-octave-up-chance)
  - [o=: Two octave down chance](#o-two-octave-down-chance)
  - [Ft: Fifth up chance](#ft-fifth-up-chance)
  - [rn: Random transposition chance](#rn-random-transposition-chance)
- [Gate menu](#gate-menu)
  - [nL: Base note length](#nl-base-note-length)
  - [GL: Base gate length](#gl-base-gate-length)
  - [rA: Ratchet chance](#ra-ratchet-chance)
  - [LE: Legato / slide chance](#le-legato-slide-chance)
  - [dL: Double length chance](#dl-double-length-chance)
  - [HL: Half length chance](#hl-half-length-chance)
  - [rU: Note run chance](#ru-note-run-chance)
  - [rL: Random length chance](#rl-random-length-chance)
  - [rG: Random gate length chance](#rg-random-gate-length-chance)
- [Play menu](#play-menu)
  - [SG: Swing](#sg-swing)
  - [t: Transpose](#t-transpose)
  - [CL: Clock source](#cl-clock-source)
  - [b: BPM](#b-bpm)
  - [LA: Latch](#la-latch)
  - [so: Note sort](#so-note-sort)
  - [vH: Velocity high](#vh-velocity-high)
  - [vL: Velocity low](#vl-velocity-low)
- [Settings menu](#settings-menu)
  - [In: Select MIDI channel in](#in-select-midi-channel-in)
  - [ot: Select MIDI channel out](#ot-select-midi-channel-out)
  - [SP: Toggle internal speaker](#sp-toggle-internal-speaker)
- [Non-menu items](#non-menu-items)
  - [Queue sequence generation](#queue-sequence-generation)
  - [Randomize sequence parameters](#randomize-sequence-parameters)
  - [Reset sequence parameters](#reset-sequence-parameters)
  - [Queue sequence slip](#queue-sequence-slip)
  - [Toggle drift mode](#toggle-drift-mode)
  - [Panic: all notes off](#panic-all-notes-off)
- [Tools](#tools)
- [Byte to digit](#byte-to-digit)

[//]: # "__endtoc__"

# Grandbot Arp Guide

Grandbot is a generative sequencer. This means you don't tell Grandbot what to play, but rather how Grandbot should decide what to play. For the most part, this is done by setting chance percentages for different options.

For a technical overview of the Arp, check out [this blog post](https://handeyeco.github.io/tech-blog/grandbot-update-arp/).

## Cheat sheet

| Description                     | MIDI CC | Display | Button             |
| ------------------------------- | ------- | ------- | ------------------ |
| **» SEqU**                      |
| Sequence length                 | 21      | SL      |
| Slip chance                     | 89      | SC      |
| Collapse notes                  | 19      | Co      |
| **» notE**                      |
| Rest chance                     | 29      | rE      |
| One octave up chance            | 22      | o-      |
| One octave down chance          | 23      | o-      |
| Two octaves up chance           | 24      | o=      |
| Two octaves down chance         | 25      | o=      |
| Fifth up chance                 | 85      | Ft      |
| Random transposition chance     | 86      | rn      |
| **» GAtE**                      |
| Base note length                | 20      | nL      |
| Base gate length                | 104     | GL      |
| Ratchet (double note) chance    | 28      | rA      |
| Legato/slide chance             | 31      | LE      |
| Double length chance            | 26      | dL      |
| Half length chance              | 27      | HL      |
| Note run chance                 | 30      | rU      |
| Random length chance            | 87      | rL      |
| Random gate length chance       | 105     | rG      |
| **» PLAy**                      |
| Swing (50-67%)                  | 115     | SG      |
| Transpose                       | 103     | t       |
| Clock: external or internal     | 112     | CL      |
| BPM for internal clock (73-200) | 113     | b       |
| Latch                           | 90      | LA      |
| Note sort                       | 114     | So      |
| Velocity high                   | 107     | vH      |
| Velocity low                    | 108     | vL      |
| **» SEtt**                      |
| Select MIDI channel in          | 14      | In      |
| Select MIDI channel out         | 15      | ot      |
| Toggle internal speaker for arp | 119     | SP      |
| **» Non-menu items**            |
| Enter menu                      |         |         | FORWARD + BACKWARD |
| Queue sequence generation       | 118     |         | UP                 |
| Randomize sequence params       | 9       |         | SHIFT + UP         |
| Reset sequence params           | 102     |         | SHIFT + RIGHT      |
| Queue sequence slip             | 116     |         | DOWN               |
| Toggle drift (endless slip)     | 3       |         | SHIFT + DOWN       |
| Play (internal clock)           |         |         | FORWARD            |
| Stop (internal clock)           |         |         | BACKWARD           |
| Panic / all notes off           | 117     | AH      | SHIFT + LEFT       |

## Button Combos

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

## Menus

When using the optional Grandbot controller board, there are a few sub-menus:

- Sequence (`SEqU`): sequence-wide settings for generating/slipping a sequence
- Note (`notE`): settings for the pitches of individual steps
- Gate (`GAtE`): settings for the lengths/gates of individual steps
- Play (`PLAy`): settings for how to play the current sequence
- Settings (`SEtt`): these are options that affect different things immediately

The Sequence, Note, and Gate settings affect how to generate a sequence. The Play and Settings settings affect how things work once a sequence is playing.

To enter or exit the menu press FORWARD and BACKWARD at the same time.

## Sequence menu

> [!TIP]
> These only affect newly generated sequences. You won't hear a difference until generating a new sequence.

> [!WARNING]
> Pretty much all of these are only affected if the MIDI CC happens on the selected MIDI in channel (by default Grandbot listens to all channels). However CC 14 (MIDI in selector) and CC 117 (Panic) listen for changes on all channels all the time.

---

### SL: Sequence length

- Submenu: Sequence
- Display: `SL`
- MIDI CC: 21
- Options: Random (`rA`), 1-8 bars
- Default: Random

The length of the full sequence.

When set to random, a different Sequence length is picked (between 1-8) each time a new sequence is generated.

---

### SC: Slip chance

- Submenu: Sequence
- Display: `SC`
- MIDI CC: 89
- Range: 0-99%
- Default: 7%

Slip subtly transforms a sequence by moving around notes. It's a way of getting variations without fully regenerating a sequence.

Slip chance is the likelihood that a step is affected when a sequence is being transformed by slip.

---

### Co: Collapse notes

- Submenu: Sequence
- Display: `Co`
- MIDI CC: 19
- Options: None (`no`), start (`St`), end (`En`)
- Default: None

When rests are present in a new sequence, collapse shifts all of the non-rest notes to either the beginning or the end of the sequence.

---

## Note menu

> [!TIP]
> These only affect newly generated sequences. You won't hear a difference until generating a new sequence.

> [!WARNING]
> Pretty much all of these are only affected if the MIDI CC happens on the selected MIDI in channel (by default Grandbot listens to all channels). However CC 14 (MIDI in selector) and CC 117 (Panic) listen for changes on all channels all the time.

---

### rE: Rest chance

- Submenu: Sequence
- Display: `rE`
- MIDI CC: 29
- Range: 0-99%
- Default: 3%

The percent chance that a step will be a rest (no note played).

> [!WARNING]
> When latch is enabled, notes are held through all rests.

---

### o-: One octave up chance

- Submenu: Sequence
- Display: `o-`
- MIDI CC: 22
- Range: 0-99%
- Default: 7%

The percent chance that a step will be transposed one octave up.

---

### o-: One octave down chance

- Submenu: Sequence
- Display: `o-`
- MIDI CC: 23
- Range: 0-99%
- Default: 7%

The percent chance that a step will be transposed one octave down.

---

### o=: Two octave up chance

- Submenu: Sequence
- Display: `o=`
- MIDI CC: 24
- Range: 0-99%
- Default: 3%

The percent chance that a step will be transposed two octaves up.

---

### o=: Two octave down chance

- Submenu: Sequence
- Display: `o=`
- MIDI CC: 25
- Range: 0-99%
- Default: 3%

The percent chance that a step will be transposed two octaves down.

---

### Ft: Fifth up chance

- Submenu: Sequence
- Display: `Ft`
- MIDI CC: 85
- Range: 0-99%
- Default: 0%

The percent chance that a step will be transposed one fifth (7 semitones) up.

---

### rn: Random transposition chance

- Submenu: Sequence
- Display: `rn`
- MIDI CC: 86
- Range: 0-99%
- Default: 0%

The percent chance that a step will be transposed randomly between -11 and +11 semitones.

---

## Gate menu

> [!TIP]
> These only affect newly generated sequences. You won't hear a difference until generating a new sequence.

> [!WARNING]
> Pretty much all of these are only affected if the MIDI CC happens on the selected MIDI in channel (by default Grandbot listens to all channels). However CC 14 (MIDI in selector) and CC 117 (Panic) listen for changes on all channels all the time.

---

### nL: Base note length

- Submenu: Sequence
- Display: `nL`
- MIDI CC: 20
- Options: Random (`rA`), 16th (`16`), 8th (` 8`), quarter (` 4`), half (`HA`), whole (`1-`), double whole (`2-`)
- Default: Random

The starting note length for each step in the sequence. This can be modified by different settings like double length chance, half length chance, and ratchet chance.

When set to random, a different base length is picked (between 16th, 8th, and quarter) each time a new sequence is generated.

---

### GL: Base gate length

- Submenu: Sequence
- Display: `GL`
- MIDI CC: 104
- Options: Random (`rA`), 33%, 66%, full (`Fu`)
- Default: Full

How long the note is held during the full note length.

Full holds the note for the full note length. 66% and 33% hold it for a percent of the full note length. Random picks between one of the other three options.

> [!WARNING]
> When latch is enabled, notes are held through full note length.

> [!WARNING]
> Legato notes are held through the following note regardless of their gate.

---

### rA: Ratchet chance

- Submenu: Sequence
- Display: `rA`
- MIDI CC: 28
- Range: 0-99%
- Default: 7%

The percent chance that a step will be split into a ratchet. A ratchet is a note played twice, each half the length as the base note length. This transforms one step into two.

---

### LE: Legato / slide chance

- Submenu: Sequence
- Display: `LE`
- MIDI CC: 31
- Range: 0-99%
- Default: 0%

Legato triggers 303 style slides in supported synths by overlapping the current note with the previous note. Example synth settings:

- Bass Station II: set triggering to "Autoglide" and increase the "Glide Time"
- Hydrasynth: select a mono/unison setting in "Polyphony", turn "Glide" on, turn "Glide Legato" on, and increase "Glide Time"

---

### dL: Double length chance

- Submenu: Sequence
- Display: `dL`
- MIDI CC: 26
- Range: 0-99%
- Default: 0%

The percent chance that a step will be twice the length as the base note length.

---

### HL: Half length chance

- Submenu: Sequence
- Display: `HL`
- MIDI CC: 27
- Range: 0-99%
- Default: 0%

The percent chance that a step will be half the length as the base note length.

---

### rU: Note run chance

- Submenu: Sequence
- Display: `rU`
- MIDI CC: 30
- Range: 0-99%
- Default: 0%

The percent chance that a step will be a run. A run is a quick succession of notes which, unlike ratchets, can be different. This transforms one step into several steps.

---

### rL: Random length chance

- Submenu: Sequence
- Display: `rL`
- MIDI CC: 87
- Range: 0-99%
- Default: 0%

The percent chance that a step will have a random length (16th, 8th, or quarter length).

---

### rG: Random gate length chance

- Submenu: Sequence
- Display: `rG`
- MIDI CC: 105
- Range: 0-99%
- Default: 0%

For each step, the chance that step will get a random gate length.

---

## Play menu

> [!WARNING]
> Pretty much all of these are only affected if the MIDI CC happens on the selected MIDI in channel (by default Grandbot listens to all channels). However CC 14 (MIDI in selector) and CC 117 (Panic) listen for changes on all channels all the time.

---

### SG: Swing

- Submenu: Settings
- Display: `SG`
- MIDI CC: 115
- Range: 50-67%
- Default: 50% (no swing)

Swing makes every other 16th note start later to create a more natural sounding rhythm. 50-67% is the percent distance a 16th note is moved between its surrounding notes.

---

### t: Transpose

- Submenu: Settings
- Display: `t`
- MIDI CC: 103
- Range: -24 to +24
- Default: 0

Transposes outgoing notes by a fixed number of semitones. Good for changing base octave or key changes.

---

### CL: Clock source

- Submenu: Settings
- Display: `CL`
- MIDI CC: 112
- Options: External (`Et`) - Internal (`In`)
- Default: External

To be able to play the sequence, there needs to be a clock source. When set to external, Grandbot will listen to its MIDI in for a clock source; this can come from a DAW or an external device like a Keystep. When set to internal, the BPM can be set in settings and the sequence can be started / stopped with the FORWARD / BACKWARD buttons respectively.

---

### b: BPM

- Submenu: Settings
- Display: `b`
- MIDI CC: 113
- Range: 73-200
- Default: 120

The beats per minute (BPM) of the sequence, or how fast the sequence is played.

> [!WARNING]
> Only affects anything when clock source is set to internal.

---

### LA: Latch

- Submenu: Settings
- Display: `LA`
- MIDI CC: 90
- Options: Off - On
- Default: Off

When latch is enabled, current notes are held when a rest is encountered.

---

### so: Note sort

- Submenu: Settings
- Display: `So`
- MIDI CC: 114
- Options: Off - On
- Default: Off

Whether incoming notes are sorted by pitch; it leads to more predictable sequences when playing chords, but the sequences are less exciting due to less variation.

---

### vH: Velocity high

- Submenu: Settings
- Display: `vH`
- MIDI CC: 107
- Range: 0-99%
- Default: 86%

Highest possible velocity (0-99% maps to 1-127).

> [!WARNING]
> If velocity high is lower than velocity low, the values will be swapped

---

### vL: Velocity low

- Submenu: Settings
- Display: `vL`
- MIDI CC: 108
- Range: 0-99%
- Default: 70%

Lowest possible velocity (0-99% maps to 1-127).

> [!WARNING]
> If velocity high is lower than velocity low, the values will be swapped

---

## Settings menu

> [!WARNING]
> Pretty much all of these are only affected if the MIDI CC happens on the selected MIDI in channel (by default Grandbot listens to all channels). However CC 14 (MIDI in selector) and CC 117 (Panic) listen for changes on all channels all the time.

---

### In: Select MIDI channel in

- Submenu: Settings
- Display: `In`
- MIDI CC: 14
- Options: All (`AL`), 1-16
- Default: All

What MIDI channel to listen to for incoming MIDI notes and CC messages. When set to "All", Grandbot will respond to notes and CC on all channels.

> [!WARNING]
> Grandbot listens to MIDI CC 14 on all channels no matter what MIDI channel in is set to.

---

### ot: Select MIDI channel out

- Submenu: Settings
- Display: `ot`
- MIDI CC: 15
- Options: All (`AL`), 1-16
- Default: All

What MIDI channel to send notes to. When set to "All", Grandbot will send notes to channel 1 (it's weird and needs to be changed).

---

### SP: Toggle internal speaker

- Submenu: Settings
- Display: `SP`
- MIDI CC: 119
- Options: Off - On
- Default: Off

Whether or not the Arp sequence should play through Grandbot's internal speaker.

## Non-menu items

---

### Queue sequence generation

- MIDI CC: 118
- Button: UP

When the sequence is not running, it generates a new sequence. When the sequence is running, it generates a new sequence at the end of the current sequence. It generates the sequence based on the current settings.

> [!TIP]
> When controlled with MIDI CC, CC needs to go below 64 before going above 64.

---

### Randomize sequence parameters

- MIDI CC: 9
- Button: SHIFT + UP

Randomly goes through sequence settings and updates them in preparation for a new sequence (it does not affect the current sequence).

It tries to be clever by only adjusting some settings and to different degrees depending on how chaotic those settings are.

> [!TIP]
> When controlled with MIDI CC, CC needs to go below 64 before going above 64.

---

### Reset sequence parameters

- MIDI CC: 102
- Button: SHIFT + RIGHT

Resets all sequence generation parameters.

> [!TIP]
> When controlled with MIDI CC, CC needs to go below 64 before going above 64.

---

### Queue sequence slip

- MIDI CC: 116
- Button: DOWN

When the sequence is not running, it "slips" the current sequence. When the sequence is running, it "slips" the sequence at the end of the current sequence. Slip transforms the sequence by randomly swapping notes.

> [!TIP]
> When controlled with MIDI CC, CC needs to go below 64 before going above 64.

---

### Toggle drift mode

- MIDI CC: 3
- Button: SHIFT + DOWN

Drift mode is an endless slip mode. Each time the sequence reaches the end, the sequence is gently transformed by moving around notes. Also affected by slip chance.

> [!TIP]
> When controlled with MIDI CC, CC needs to go below 64 before going above 64.

---

### Panic: all notes off

- MIDI CC: 117
- Button: SHIFT + LEFT

In the case of an emergency, panic tries to stop everything and tells all connected devices to stop everything.

> [!TIP]
> When controlled with MIDI CC, CC needs to go below 64 before going above 64.

---

## Tools

- [Tool to control the arp via MIDI CC](https://handeyeco.github.io/grandbot-web-controller/) ([Source](https://github.com/handeyeco/grandbot-web-controller))
- [Tool to convert display settings to bytes](https://handeyeco.github.io/grandbot-designer/) ([Source](https://github.com/handeyeco/grandbot-designer))

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