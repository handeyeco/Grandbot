#include <Arduino.h>
#include <ButtonManager.h>
#include <ExpressionSets.h>
#include <Expressions.h>
#include <Setting.h>
#include <SettingTransforms.h>
#include <Stepper.h>

#ifndef SETTING_MANAGER_INCL_GUARD
#define SETTING_MANAGER_INCL_GUARD

#define SUBMENU_COUNT 5
#define SEQUENCE_SETTING_COUNT 3
#define NOTE_SETTING_COUNT 7
#define GATE_SETTING_COUNT 10
#define PLAY_SETTING_COUNT 9
#define GENERAL_SETTING_COUNT 3

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
  Setting* noteSettings[NOTE_SETTING_COUNT];
  Setting* gateSettings[GATE_SETTING_COUNT];
  Setting* playSettings[PLAY_SETTING_COUNT];
  Setting* generalSettings[GENERAL_SETTING_COUNT];

  // whether or not the menu is active
  bool isInMenu = false;
  // whether or not the submenu is active
  bool isInSubMenu = false;
  // which submenu we're in
  // 0 = sequence
  // 1 = note
  // 2 = gate
  // 3 = play
  // 4 = settings
  byte subMenu = 0;
  // index of the parameter that's focused
  byte menuOptionIndex = 0;

  void writeMenu();

 public:
  SettingManager(Expressions* expr, ButtonManager* buttons);

  Setting* collapseNotes;
  Setting* sequenceLength;
  Setting* baseGateLength;
  Setting* baseNoteLength;
  Setting* octaveOneUpChance;
  Setting* octaveOneDownChance;
  Setting* octaveTwoUpChance;
  Setting* octaveTwoDownChance;
  Setting* halfLengthChance;
  Setting* doubleLengthChance;
  Setting* ratchetChance;
  Setting* tripletChance;
  Setting* legatoChance;
  Setting* restChance;
  Setting* runChance;
  Setting* fifthChance;
  Setting* randomNoteChance;
  Setting* randomGateChance;
  Setting* randomLengthChance;
  Setting* slipChance;

  Setting* clock;
  Setting* bpm;
  Setting* swing;
  Setting* velocityHigh;
  Setting* velocityLow;
  Setting* useSpeaker;
  Setting* sort;
  Setting* transpose;
  Setting* latch;
  Setting* combineNotes;
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