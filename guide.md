# Grandbot Guide

## Arp

Grandbot is a generative sequencer. This means you don't tell Grandbot what to play, but rather how Grandbot should decide what to play. For the most part, this is done by setting chance percentages for different options.

For a technical overview of the Arp, check out [this blog post](https://handeyeco.github.io/tech-blog/grandbot-update-arp/).

### Menus

When using the optional Grandbot controller board, there are two sub-menus:

- Sequence (`SEqu`): these are options that come into effect when generating a new sequence
- Settings (`SEtt`): these are options that affect different things immediately

To enter or exit the menu press FORWARD and BACKWARD at the same time.

### Settings

> [!TIP]
> For most of these, adjusting the setting only affects newly generated sequences and not the current sequence. Generate a new sequence after making changes to hear the effects.

> [!WARNING]
> Pretty much all of these are only affected if the MIDI CC happens on the selected MIDI in channel (by default Grandbot listens to all channels). However CC 14 (MIDI in selector) and CC 117 (Panic) listen for changes on all channels all the time.

---

#### Base note length

 - Submenu: Sequence
 - MIDI CC: 20
 - Display: `nL`
 - Options: Random (`rA`), 16th (`16`), 8th (`8`), quarter (` 1`), half (`HA`), whole (`1-`), double whole (`2-`)
 - Default: Random

The starting note length for each step in the sequence. This can be modified by different settings like double length chance, half length chance, and ratchet chance.

When set to random, a different base length is picked (between 16th, 8th, and quarter) each time a new sequence is generated.

---

#### Sequence length

- Submenu: Sequence
- MIDI CC: 21
- Display: `SL`
- Options: Random (`rA`), 1-8 bars
- Default: Random

The length of the full sequence.

When set to random, a different Sequence length is picked (between 1-8) each time a new sequence is generated.

---

#### One octave up chance

- Submenu: Sequence
- MIDI CC: 22
- Display: `o-`
- Range: 0-99%
- Default: 7%

The percent chance that a step will be transposed one octave up.

---

#### One octave down chance

- Submenu: Sequence
- MIDI CC: 23
- Display: `o-`
- Range: 0-99%
- Default: 7%

The percent chance that a step will be transposed one octave down.

---

#### Two octave up chance

- Submenu: Sequence
- MIDI CC: 24
- Display: `o=`
- Range: 0-99%
- Default: 3%

The percent chance that a step will be transposed two octaves up.

---

#### Two octave down chance

- Submenu: Sequence
- MIDI CC: 25
- Display: `o=`
- Range: 0-99%
- Default: 3%

The percent chance that a step will be transposed two octaves down.

---

#### Fifth up chance

- Submenu: Sequence
- MIDI CC: 85
- Display: `Ft`
- Range: 0-99%
- Default: 0%

The percent chance that a step will be transposed one fifth (7 semitones) up.

---

#### Random transposition chance

- Submenu: Sequence
- MIDI CC: 86
- Display: `rn`
- Range: 0-99%
- Default: 0%

The percent chance that a step will be transposed randomly between -11 and +11 semitones.

---

#### Rest chance

- Submenu: Sequence
- MIDI CC: 29
- Display: `rE`
- Range: 0-99%
- Default: 3%

The percent chance that a step will be a rest (no note played).

> [!WARNING]
> When latch is enabled, notes are held through all rests.

---

#### Double length chance

- Submenu: Sequence
- MIDI CC: 26
- Display: `dL`
- Range: 0-99%
- Default: 0%

The percent chance that a step will be twice the length as the base note length.

---

#### Half length chance

- Submenu: Sequence
- MIDI CC: 27
- Display: `HL`
- Range: 0-99%
- Default: 0%

The percent chance that a step will be half the length as the base note length.

---

#### Ratchet chance

- Submenu: Sequence
- MIDI CC: 28
- Display: `rA`
- Range: 0-99%
- Default: 7%

The percent chance that a step will be split into a ratchet. A ratchet is a note played twice, each half the length as the base note length. This transforms one step into two.

---

#### Random length chance

- Submenu: Sequence
- MIDI CC: 87
- Display: `rL`
- Range: 0-99%
- Default: 0%

The percent chance that a step will have a random length (16th, 8th, or quarter length).

---

#### Note run chance

- Submenu: Sequence
- MIDI CC: 30
- Display: `ru`
- Range: 0-99%
- Default: 0%

The percent chance that a step will be a run. A run is a quick succession of notes which, unlike ratchets, can be different. This transforms one step into several steps.

---

#### Collapse notes

- Submenu: Sequence
- MIDI CC: 19
- Display: `Co`
- Options: None (`no`), start (`St`), end (`En`)
- Default: None

When rests are present in a new sequence, collapse shifts all of the non-rest notes to either the beginning or the end of the sequence.

---

#### Queue sequence generation

- MIDI CC: 118
- Button: UP

When the sequence is not running, it generates a new sequence. When the sequence is running, it generates a new sequence at the end of the current sequence. It generates the sequence based on the current settings.

> [!TIP]
> When controlled with MIDI CC, CC needs to go below 64 before going above 64.

---

#### Randomize sequence parameters

- MIDI CC: 9
- Button: SHIFT + UP

Randomly goes through sequence settings and updates them in preparation for a new sequence (it does not affect the current sequence).

It tries to be clever by only adjusting some settings and to different degrees depending on how chaotic those settings are.

> [!TIP]
> When controlled with MIDI CC, CC needs to go below 64 before going above 64.

---

#### Slip chance

- Submenu: Sequence
- MIDI CC: 89
- Display: `SC`
- Range: 0-99%
- Default: 7%

Slip subtly transforms a sequence by moving around notes. It's a way of getting variations without fully regenerating a sequence.

Slip chance is the likelihood that a step is affected when a sequence is being transformed by slip.

---

#### Queue sequence slip

- MIDI CC: 116
- Button: DOWN

When the sequence is not running, it "slips" the current sequence. When the sequence is running, it "slips" the sequence at the end of the current sequence. Slip transforms the sequence by randomly swapping notes.

> [!TIP]
> When controlled with MIDI CC, CC needs to go below 64 before going above 64.

---

#### Toggle drift mode

- MIDI CC: 3
- Button: SHIFT + DOWN

Drift mode is an endless slip mode. Each time the sequence reaches the end, the sequence is gently transformed by moving around notes. Also affected by slip chance.

> [!TIP]
> When controlled with MIDI CC, CC needs to go below 64 before going above 64.

---

#### Latch

- Submenu: Settings
- MIDI CC: 90
- Display: `LA`
- Options: Off - On
- Default: Off

When latch is enabled, current notes are held when a rest is encountered.

---

#### Swing

- Submenu: Settings
- MIDI CC: 115
- Display: `SG`
- Range: 50-67%
- Default: 50% (no swing)

Swing makes every other 16th note start later to create a more natural sounding rhythm. 50-67% is the percent distance a 16th note is moved between its surrounding notes.

---

#### Note sort

- Submenu: Settings
- MIDI CC: 114
- Display: `So`
- Options: Off - On
- Default: Off

Whether incoming notes are sorted by pitch; it leads to more predictable sequences when playing chords, but the sequences are less exciting due to less variation.

---

#### Select MIDI channel in

- Submenu: Settings
- MIDI CC: 14
- Display: `In`
- Options: All (`AL`), 1-16
- Default: All

What MIDI channel to listen to for incoming MIDI notes and CC messages. When set to "All", Grandbot will respond to notes and CC on all channels.

> [!WARNING]
> Grandbot listens to MIDI CC 14 on all channels no matter what MIDI channel in is set to.

---

#### Select MIDI channel out

- Submenu: Settings
- MIDI CC: 15
- Display: `ot`
- Options: All (`AL`), 1-16
- Default: All

What MIDI channel to send notes to. When set to "All", Grandbot will send notes to channel 1 (it's weird and needs to be changed).

---

#### Panic / all notes off

- MIDI CC: 117
- Button: SHIFT + LEFT

In the case of an emergency, panic tries to stop everything and tells all connected devices to stop everything.

> [!TIP]
> When controlled with MIDI CC, CC needs to go below 64 before going above 64.

---

#### Toggle internal speaker

- Submenu: Settings
- MIDI CC: 119
- Display: `SP`
- Options: Off - On
- Default: Off

Whether or not the Arp sequence should play through Grandbot's internal speaker.

---

#### Clock source

- Submenu: Settings
- MIDI CC: 112
- Display: `CL`
- Options: External (`Et`) - Internal (`In`)
- Default: External

To be able to play the sequence, there needs to be a clock source. When set to external, Grandbot will listen to its MIDI in for a clock source; this can come from a DAW or an external device like a Keystep. When set to internal, the BPM can be set in settings and the sequence can be started / stopped with the FORWARD / BACKWARD buttons respectively.

---

#### BPM

- Submenu: Settings
- MIDI CC: 113
- Display: `b`
- Range: 73-200
- Default: 120

The beats per minute (BPM) of the sequence, or how fast the sequence is played.

> [!WARNING]
> Only affects anything when clock source is set to internal.