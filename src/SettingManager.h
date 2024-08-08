#include <Arduino.h>
#include <Setting.h>
#include <ButtonManager.h>
#include <Expressions.h>
#include <ExpressionSets.h>

#ifndef SETTING_MANAGER_INCL_GUARD
#define SETTING_MANAGER_INCL_GUARD

#define MAX_MENU_ITEMS 255
#define SEQUENCE_SETTING_COUNT 2
#define GENERAL_SETTING_COUNT 2

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

    Setting* ccOctaveOneUpChance;
    Setting* ccOctaveOneDownChance;

    Setting* ccUseSpeaker;
    Setting* ccSort;

    Setting* getSettingByCC(byte cc);
    bool usesCC(byte cc);
    void handleCC(byte cc, byte value);

    void updateMenu();
    void toggleMenu();
    bool inMenu();
};

#endif