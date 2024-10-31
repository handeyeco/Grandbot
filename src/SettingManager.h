#include <Arduino.h>
#include <ButtonManager.h>
#include <ExpressionSets.h>
#include <Expressions.h>
#include <Setting.h>
#include <SettingTransforms.h>
#include <Stepper.h>

#ifndef SETTING_MANAGER_INCL_GUARD
#define SETTING_MANAGER_INCL_GUARD

#define MAX_MENU_ITEM_INDEX 254
#define SEQUENCE_SETTING_COUNT 17
#define GENERAL_SETTING_COUNT 8

// used to map MIDI CC 0-127 to 73-200 BPM
#define BPM_OFFSET 73

/**
 * Manages all settings
 */
struct SettingManager {
 private:
  // 4D7S display manager
  Expressions* expr;
  ButtonManager* buttons;

  Setting* sequenceSettings[SEQUENCE_SETTING_COUNT];
  Setting* generalSettings[GENERAL_SETTING_COUNT];

  // 0 = no menu
  // 1 = submenu select
  // 2 = seq menu
  // 3 = general menu
  byte menuStage = 0;
  byte menuIndex = 0;
  void writeMenu();

 public:
  SettingManager(Expressions* expr, ButtonManager* buttons);

  Setting* collapseNotes;
  Setting* sequenceLength;
  Setting* baseNoteLength;
  Setting* octaveOneUpChance;
  Setting* octaveOneDownChance;
  Setting* octaveTwoUpChance;
  Setting* octaveTwoDownChance;
  Setting* halfLengthChance;
  Setting* doubleLengthChance;
  Setting* ratchetChance;
  Setting* legatoChance;
  Setting* restChance;
  Setting* runChance;
  Setting* fifthChance;
  Setting* randomNoteChance;
  Setting* randomLengthChance;
  Setting* slipChance;

  Setting* clock;
  Setting* bpm;
  Setting* swing;
  Setting* useSpeaker;
  Setting* sort;
  Setting* latch;
  Setting* midiChannelIn;
  Setting* midiChannelOut;

  Setting* getSettingByCC(byte cc);
  bool usesCC(byte cc);
  void handleCC(byte cc, byte value);

  void randomize();
  void reset();

  void updateMenu();
  void toggleMenu();
  bool inMenu();
};

#endif