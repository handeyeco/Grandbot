#include <SettingManager.h>

SettingManager::SettingManager(Expressions* _expr, ButtonManager* _buttons)
    : expr(_expr), buttons(_buttons) {
  // Settings sorted by MIDI CC
  // available CC: 3, 9, 14-15, 20-31, 85-87, 89-90, 102-119

  // CC_DRIFT 3
  // CC_RANDOMIZE_CHANCES 9

  // MIDI channel to listen to (respected by most things except panic and
  // midiChannelIn itself) options are: 0 = all channels / 1-16 = channels 1-16
  midiChannelIn = new Setting(0, 14, CHAR_I, CHAR_N,
                              SettingTransforms::midiChValueTransform,
                              SettingTransforms::midiChStepTransform,
                              SettingTransforms::noRandomizeTransform);
  // MIDI channel to send to
  // options are: 0 = all channels / 1-16 = channels 1-16
  midiChannelOut = new Setting(0, 15, CHAR_O, CHAR_T,
                               SettingTransforms::midiChValueTransform,
                               SettingTransforms::midiChStepTransform,
                               SettingTransforms::noRandomizeTransform);

  // Collapse all notes/rests to one side
  // options are: none, start (play notes at the beginning), end (play notes at
  // the end)
  collapseNotes = new Setting(0, 19, CHAR_C, CHAR_O,
                              SettingTransforms::collapseNotesValueTransform,
                              SettingTransforms::collapseNotesStepTransform,
                              SettingTransforms::lowRandomizeTransform);
  // Default note length in a sequence (gets transformed by other parameters per
  // step) options are: random, 1/16, 1/8, 1/4, 1/2, 1 (whole note), 2
  baseNoteLength = new Setting(0, 20, CHAR_N, CHAR_L,
                               SettingTransforms::noteLengthValueTransform,
                               SettingTransforms::noteLengthStepTransform,
                               SettingTransforms::highRandomizeTransform);
  // The length of the generated sequence
  // options are: random, 1-8 bars
  sequenceLength = new Setting(0, 21, CHAR_S, CHAR_L,
                               SettingTransforms::sequenceLengthValueTransform,
                               SettingTransforms::sequenceLengthStepTransform,
                               SettingTransforms::highRandomizeTransform);

  // Chance a step will be transposed one octave up
  octaveOneUpChance = new Setting(10, 22, B01100011, B01000000,
                                  SettingTransforms::ccValueTransform,
                                  SettingTransforms::ccStepTransform,
                                  SettingTransforms::mediumRandomizeTransform);
  // Chance a step will be transposed one octave down
  octaveOneDownChance = new Setting(
      10, 23, B00011101, B00001000, SettingTransforms::ccValueTransform,
      SettingTransforms::ccStepTransform,
      SettingTransforms::mediumRandomizeTransform);
  // Chance a step will be transposed two octaves up
  octaveTwoUpChance = new Setting(5, 24, B01100011, B01000001,
                                  SettingTransforms::ccValueTransform,
                                  SettingTransforms::ccStepTransform,
                                  SettingTransforms::lowRandomizeTransform);
  // Chance a step will be transposed two octaves down
  octaveTwoDownChance = new Setting(5, 25, B00011101, B00001001,
                                    SettingTransforms::ccValueTransform,
                                    SettingTransforms::ccStepTransform,
                                    SettingTransforms::lowRandomizeTransform);
  // Chance a step's length will change to double length
  doubleLengthChance =
      new Setting(0, 26, CHAR_D, CHAR_L, SettingTransforms::ccValueTransform,
                  SettingTransforms::ccStepTransform,
                  SettingTransforms::lowRandomizeTransform);
  // Chance a step's length will change to half length
  halfLengthChance =
      new Setting(0, 27, CHAR_H, CHAR_L, SettingTransforms::ccValueTransform,
                  SettingTransforms::ccStepTransform,
                  SettingTransforms::lowRandomizeTransform);
  // Chance a step will be a ratchet (two half-length steps of the same note)
  ratchetChance =
      new Setting(10, 28, CHAR_R, CHAR_A, SettingTransforms::ccValueTransform,
                  SettingTransforms::ccStepTransform,
                  SettingTransforms::mediumRandomizeTransform);
  // Chance a step will overlap with the previous step (for triggering legato)
  legatoChance =
      new Setting(0, 31, CHAR_L, CHAR_E, SettingTransforms::ccValueTransform,
                  SettingTransforms::ccStepTransform,
                  SettingTransforms::noRandomizeTransform);
  // Chance a step will be a rest
  restChance =
      new Setting(5, 29, CHAR_R, CHAR_E, SettingTransforms::ccValueTransform,
                  SettingTransforms::ccStepTransform,
                  SettingTransforms::mediumRandomizeTransform);
  // Chance a step will be a run (4 quick, different notes)
  runChance =
      new Setting(0, 30, CHAR_R, CHAR_U, SettingTransforms::ccValueTransform,
                  SettingTransforms::ccStepTransform,
                  SettingTransforms::lowRandomizeTransform);
  // Chance a step will be transposed a fifth up
  fifthChance =
      new Setting(0, 85, CHAR_F, CHAR_T, SettingTransforms::ccValueTransform,
                  SettingTransforms::ccStepTransform,
                  SettingTransforms::noRandomizeTransform);
  // Chance a step will be transposed randomly (-11 to 11)
  randomNoteChance =
      new Setting(0, 86, CHAR_R, CHAR_N, SettingTransforms::ccValueTransform,
                  SettingTransforms::ccStepTransform,
                  SettingTransforms::noRandomizeTransform);
  // Chance a step's length will be randomized
  randomLengthChance =
      new Setting(0, 87, CHAR_R, CHAR_L, SettingTransforms::ccValueTransform,
                  SettingTransforms::ccStepTransform,
                  SettingTransforms::noRandomizeTransform);
  // Chance a step will be swapped with an adjacent step (during slips not
  // sequence generation)
  slipChance =
      new Setting(10, 89, CHAR_S, CHAR_C, SettingTransforms::ccValueTransform,
                  SettingTransforms::ccStepTransform,
                  SettingTransforms::highRandomizeTransform);
  // If active, hold notes through rests
  latch = new Setting(10, 90, CHAR_L, CHAR_A,
                      SettingTransforms::onOffValueTransform,
                      SettingTransforms::onOffStepTransform,
                      SettingTransforms::noRandomizeTransform);

  // Whether to use an external (Et) or internal (In) clock
  // TODO: should the default be internal?
  clock = new Setting(0, 112, CHAR_C, CHAR_L,
                      SettingTransforms::clockValueTransform,
                      SettingTransforms::onOffStepTransform,
                      SettingTransforms::noRandomizeTransform);
  // When using an internal clock, what BPM? 0-127 gets mapped to 73-200
  // TODO can we add an onchange callback or something to update
  // timeBetweenInternalClockPulses when changed
  bpm = new Setting(47, 113, CHAR_B, CHAR_BLANK,
                    SettingTransforms::bpmValueTransform,
                    SettingTransforms::bpmStepTransform,
                    SettingTransforms::noRandomizeTransform, false);
  // Whether incoming notes are sorted; true leads to more predictable
  // sequences, but are less exciting due to less variation
  // TODO can we add an onchange callback or something to trigger sort of
  // currently pressed/active notes?
  sort = new Setting(0, 114, CHAR_S, CHAR_O,
                     SettingTransforms::onOffValueTransform,
                     SettingTransforms::onOffStepTransform,
                     SettingTransforms::noRandomizeTransform);
  // How many semitones to transpose the output notes
  transpose = new Setting(64, 103, CHAR_T, CHAR_BLANK,
                     SettingTransforms::transposeValueTransform,
                     SettingTransforms::transposeStepTransform,
                     SettingTransforms::noRandomizeTransform, false);
  // Swing of sequence playback; delays every other 16th note when activated.
  // 50% = no swing; 67% max swing
  // TODO how hard would it be to support negative swing?
  swing = new Setting(0, 115, CHAR_S, CHAR_G,
                      SettingTransforms::swingValueTransform,
                      SettingTransforms::swingStepTransform,
                      SettingTransforms::noRandomizeTransform);

  // CC_SLIP 116
  // CC_PANIC 117
  // CC_GENERATE_SEQUENCE 118

  // Whether to play the sequence through GB's speaker or not
  useSpeaker = new Setting(0, 119, CHAR_S, CHAR_P,
                           SettingTransforms::onOffValueTransform,
                           SettingTransforms::onOffStepTransform,
                           SettingTransforms::noRandomizeTransform);

  // Settings sorted for menu
  sequenceSettings[0] = slipChance;
  sequenceSettings[1] = sequenceLength;
  sequenceSettings[2] = baseNoteLength;
  sequenceSettings[3] = ratchetChance;
  sequenceSettings[4] = legatoChance;
  sequenceSettings[5] = restChance;
  sequenceSettings[6] = octaveOneUpChance;
  sequenceSettings[7] = octaveOneDownChance;
  sequenceSettings[8] = octaveTwoUpChance;
  sequenceSettings[9] = octaveTwoDownChance;
  sequenceSettings[10] = halfLengthChance;
  sequenceSettings[11] = doubleLengthChance;
  sequenceSettings[12] = runChance;
  sequenceSettings[13] = fifthChance;
  sequenceSettings[14] = randomNoteChance;
  sequenceSettings[15] = randomLengthChance;
  sequenceSettings[16] = collapseNotes;

  generalSettings[0] = swing;
  generalSettings[1] = latch;
  generalSettings[2] = useSpeaker;
  generalSettings[3] = midiChannelIn;
  generalSettings[4] = midiChannelOut;
  generalSettings[5] = sort;
  generalSettings[6] = clock;
  generalSettings[7] = transpose;
  generalSettings[8] = bpm;
}

/**
 * Go through all of the settings and find the one that uses
 * a specific MIDI CC
 *
 * @param {byte} cc - the MIDI CC to look for
 * @returns {Setting* | NULL} returns a pointer to the setting if found;
 * otherwise NULL
 */
Setting* SettingManager::getSettingByCC(byte cc) {
  for (int i = 0; i < SEQUENCE_SETTING_COUNT; i++) {
    if (sequenceSettings[i]->midiCC == cc) {
      return sequenceSettings[i];
    }
  }

  for (int i = 0; i < GENERAL_SETTING_COUNT; i++) {
    if (generalSettings[i]->midiCC == cc) {
      return generalSettings[i];
    }
  }

  return NULL;
}

void SettingManager::randomize() {
  for (int i = 0; i < SEQUENCE_SETTING_COUNT; i++) {
    sequenceSettings[i]->randomize();
  }
}

void SettingManager::reset() {
  for (int i = 0; i < SEQUENCE_SETTING_COUNT; i++) {
    sequenceSettings[i]->reset();
  }
}

/**
 * Go through all of the settings and see if we're using
 * a given MIDI CC
 *
 * @param {byte} cc - the MIDI CC to look for
 * @returns {bool} whether we care about the CC or not
 */
bool SettingManager::usesCC(byte cc) {
  Setting* needle = getSettingByCC(cc);

  return needle != NULL;
}

/**
 * Handle incoming CC by setting Setting value
 * and updating the display
 *
 * @param {byte} cc - the MIDI CC to handle
 * @param {byte} value - the incoming value
 */
void SettingManager::handleCC(byte cc, byte value) {
  Setting* setting = getSettingByCC(cc);

  if (setting == NULL)
    return;

  setting->setValue(value);

  byte fullDisplay[4];
  setting->getDisplay(fullDisplay);
  expr->writeText(fullDisplay);
}

/**
 * Handles interactions and displays when inside
 * of the menu
 */
void SettingManager::updateMenu() {
  // Select submenu from top menu
  if (menuStage == 1 && buttons->forward.released) {
    menuStage = (menuIndex % 2 == 0) ? 2 : 3;
    menuIndex = 0;
    writeMenu();
    return;
  }
  // Increment/decrement focused setting when in a submenu
  else if (menuStage > 1 && (buttons->up.released || buttons->down.released)) {
    Setting* setting = menuStage == 2 ? sequenceSettings[menuIndex]
                                      : generalSettings[menuIndex];

    setting->step(buttons->up.released, buttons->forward.held);
    writeMenu();
    return;
  } else if (menuStage > 0) {
    // Back out of menus
    if (buttons->backward.released) {
      menuStage = (menuStage == 1) ? 0 : 1;
      menuIndex = 0;
      if (menuStage == 0) {
        expr->setMenu(false);
      } else {
        writeMenu();
      }
      return;
    }
    // Handle scrolling right through settings
    else if (buttons->right.released || buttons->left.released) {
      int nextIndex = menuIndex + (buttons->right.released ? 1 : -1);

      byte maxIndex = menuStage == 1   ? 2
                      : menuStage == 2 ? SEQUENCE_SETTING_COUNT
                                       : GENERAL_SETTING_COUNT;
      maxIndex -= 1;

      if (nextIndex < 0) {
        nextIndex = maxIndex;
      } else if (nextIndex > maxIndex) {
        nextIndex = 0;
      }

      menuIndex = nextIndex;
      writeMenu();
      return;
    }
  }
}

/**
 * Jump in and out of the menu
 */
void SettingManager::toggleMenu() {
  menuStage = menuStage == 0 ? 1 : 0;
  menuIndex = 0;
  expr->setMenu(menuStage);
  writeMenu();
}

/**
 * Are we in a menu?
 *
 * @returns {bool} whether we're in the menu
 */
bool SettingManager::inMenu() {
  return menuStage > 0;
}

/**
 * Write the current state of the menu to the display
 */
void SettingManager::writeMenu() {
  if (menuStage == 0)
    return;

  byte dis[4];
  // Handle top-level menu
  if (menuStage == 1) {
    if (menuIndex == 0) {
      // SEQU = sequence settings
      dis[0] = CHAR_S;
      dis[1] = CHAR_E;
      dis[2] = CHAR_Q;
      dis[3] = CHAR_U;
    } else {
      // SETT = general settings
      dis[0] = CHAR_S;
      dis[1] = CHAR_E;
      dis[2] = CHAR_T;
      dis[3] = CHAR_T;
    }
    expr->writeText(dis, false);
  }
  // Handle sub menus
  else {
    Setting* setting = menuStage == 2 ? sequenceSettings[menuIndex]
                                      : generalSettings[menuIndex];

    setting->getDisplay(dis);
    expr->writeText(dis, setting->usesColon);
  }
}