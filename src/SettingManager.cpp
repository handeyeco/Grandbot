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

void onOffValueTransform(byte value, byte output[2]) {
  output[0] = CHAR_O;

  if (value < 64) {
    output[1] = CHAR_F;
  } else {
    output[1] = CHAR_N;
  }
}

// TODO: we store values as 0-127 (because that's what MIDI uses)
// however we display values as 0-99 (because we only have two free digits)
// this means that sometimes we have to push the button an extra time
// to get the screen to update (even though it is updating the actual value correctly)
byte ccStepTransform(byte value, bool stepUp) {
  return value + (stepUp ? 1 : -1);
}

byte onOffStepTransform(byte value, bool stepUp) {
  return stepUp ? 127 : 0;
}

SettingManager::SettingManager(Expressions* _expr, ButtonManager* _buttons) : expr(_expr), buttons(_buttons) {
  ccOctaveOneUpChance = new Setting(10, 22, B01100011, B01000000, ccValueTransform, ccStepTransform);

  ccUseSpeaker = new Setting(0, 119, CHAR_S, CHAR_P, onOffValueTransform, onOffStepTransform);

  sequenceSettings[0] = ccOctaveOneUpChance;

  generalSettings[0] = ccUseSpeaker;
}

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

bool SettingManager::usesCC(byte cc){
  Setting* needle = getSettingByCC(cc);

  return needle != NULL;
}

void SettingManager::handleCC(byte cc, byte value) {
  Setting* setting = getSettingByCC(cc);

  if (setting == NULL) return;

  byte fullDisplay[4];
  setting->getDisplay(fullDisplay);

  setting->setValue(value);
  expr->writeText(fullDisplay);
}

void SettingManager::updateMenu() {
  if (menuStage == 1 && buttons->forward.released) {
    menuStage = (menuIndex % 2 == 0) ? 2 : 3;
    menuIndex = 0;
    writeMenu();
    return;
  } else if (menuStage > 0) {
    if (buttons->backward.released) {
      menuStage = (menuStage == 1) ? 0 : 1;
      menuIndex = 0;
      Serial.print("menuStage: ");
      Serial.println(menuStage);
      if (menuStage == 0) {
        expr->setMenu(0);
      } else {
        writeMenu();
      }
      return;
    } else if (buttons->right.released) {
      menuIndex = menuIndex == 255 ? 0 : (menuIndex + 1);
      writeMenu();
      return;
    } else if (buttons->left.released) {
      menuIndex = menuIndex == 0 ? MAX_MENU_ITEMS : (menuIndex - 1);
      writeMenu();
      return;
    }
  }
}

void SettingManager::toggleMenu() {
  menuStage = menuStage == 0 ? 1 : 0;
  expr->setMenu(menuStage);
  writeMenu();
}

bool SettingManager::inMenu() {
  return menuStage > 0;
}

void SettingManager::writeMenu() {
  if (menuStage == 0) return;

  byte dis[4];
  if (menuStage == 1) {
    if (menuIndex % 2 == 0) {
      dis[0]=CHAR_S;dis[1]=CHAR_E;dis[2]=CHAR_Q;dis[3]=CHAR_U;
    } else {
      dis[0]=CHAR_S;dis[1]=CHAR_E;dis[2]=CHAR_T;dis[3]=CHAR_T;
    }
    expr->writeText(dis, false);
  } else {
    Setting* setting = menuStage == 2
      ? sequenceSettings[menuIndex %  SEQUENCE_SETTING_COUNT]
      : generalSettings[menuIndex %  GENERAL_SETTING_COUNT];

    setting->getDisplay(dis);
    expr->writeText(dis, true);
  }
}