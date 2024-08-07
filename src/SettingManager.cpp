#include <SettingManager.h>

void ccValueTransform(byte value, byte output[2]) {
  byte mapped = map(value, 0, 127, 0, 99);

  if (mapped < 10) {
    output[0] = CHAR_BLANK;
    output[1] = ExpressionSets::convertNumberToByte(mapped);
    return;
  }

  output[0] = ExpressionSets::convertNumberToByte((mapped / 10) % 10);
  output[1] = ExpressionSets::convertNumberToByte(mapped % 10);
}

// TODO: we store values as 0-127 (because that's what MIDI uses)
// however we display values as 0-99 (because we only have two free digits)
// this means that sometimes we have to push the button an extra time
// to get the screen to update (even though it is updating the actual value correctly)
byte ccStepTransform(byte value, bool stepUp) {
  return value + (stepUp ? 1 : -1);
}

SettingManager::SettingManager(Expressions* _expr) {
  expr = _expr;

  ccOctaveOneUpChance = new Setting(10, 22, B01100011, B01000000, ccValueTransform, ccStepTransform);

  allSettings[0] = ccOctaveOneUpChance;
}

bool SettingManager::usesCC(byte cc){
  for (int i = 0; i < SETTING_COUNT; i++) {
    if (allSettings[i]->midiCC == cc) {
      return true;
    }
  }

  return false;
}

void SettingManager::handleCC(byte cc, byte value) {
  Setting* setting;
  for (int i = 0; i < SETTING_COUNT; i++) {
    if (allSettings[i]->midiCC == cc) {
      setting = allSettings[i];
      break;
    }
  }

  setting->setValue(value);
  expr->setting(*setting);
}