#include <Arduino.h>
#include <Setting.h>
#include <Expressions.h>
#include <ExpressionSets.h>

#ifndef SETTING_MANAGER_INCL_GUARD
#define SETTING_MANAGER_INCL_GUARD


/**
 * Manages all settings
*/
struct SettingManager {
  private:
  public:
    SettingManager();

    Setting* ccOctaveOneUpChance;
};

#endif