#include <Arduino.h>
#include <Setting.h>
#include <Expressions.h>
#include <ExpressionSets.h>

#ifndef SETTING_MANAGER_INCL_GUARD
#define SETTING_MANAGER_INCL_GUARD

#define SETTING_COUNT 1

/**
 * Manages all settings
*/
struct SettingManager {
  private:
    // 4D7S display manager
    Expressions* expr;
    Setting* allSettings[SETTING_COUNT];
  public:
    SettingManager(Expressions* expr);

    Setting* ccOctaveOneUpChance;
    bool usesCC(byte cc);
    void handleCC(byte cc, byte value);
};

#endif