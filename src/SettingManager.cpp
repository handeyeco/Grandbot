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

SettingManager::SettingManager() {
  byte ccOctaveOneUpChanceDisplay[2] = {B01100011, B01000000};
  ccOctaveOneUpChance = new Setting(10, 22, ccOctaveOneUpChanceDisplay, ccValueTransform);
}