#include <Arduino.h>
#include <Setting.h>
#include <Expressions.h>
#include <ExpressionSets.h>

#ifndef SETTING_MANAGER_INCL_GUARD
#define SETTING_MANAGER_INCL_GUARD

#define SEQUENCE_SETTING_COUNT 1
#define GENERAL_SETTING_COUNT 1

/**
 * Manages all settings
*/
struct SettingManager {
  private:
    // 4D7S display manager
    Expressions* expr;
    Setting* sequenceSettings[SEQUENCE_SETTING_COUNT];
    Setting* generalSettings[GENERAL_SETTING_COUNT];
  public:
    SettingManager(Expressions* expr);

    Setting* ccOctaveOneUpChance;
    Setting* ccUseSpeaker;

    Setting* getSettingByCC(byte cc);
    bool usesCC(byte cc);
    void handleCC(byte cc, byte value);
};

#endif