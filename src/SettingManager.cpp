#include <SettingManager.h>

/**
 * Transformers
 */
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

void noteLengthValueTransform(byte value, byte output[2]) {
  if (value > 108) {
    // double whole
    output[0] = CHAR_2;
    output[1] = CHAR_DASH;
  }
  else if (value > 90) {
    // whole
    output[0] = CHAR_1;
    output[1] = CHAR_DASH;
  }
  else if (value > 72) {
    // half
    output[0] = CHAR_H;
    output[1] = CHAR_A;
  }
  else if (value > 54) {
    // quarter
    output[0] = CHAR_BLANK;
    output[1] = CHAR_1;
  }
  else if (value > 36) {
    // 8th
    output[0] = CHAR_BLANK;
    output[1] = CHAR_8;
  }
  else if (value > 18) {
    // 16th
    output[0] = CHAR_1;
    output[1] = CHAR_6;
  }
  else {
    // random
    output[0] = CHAR_R;
    output[1] = CHAR_A;
  }
}

byte noteLenthStepTransform(byte value, bool stepUp, bool shift) {
  if (shift) {
    if (stepUp) {
      return 108 + 10;
    } else {
      return 0;
    }
  }

  int step = 127 / 7 + 1;
  step = step * (stepUp ? 1 : -1);
  int temp = value;
  temp += step;

  // TODO this is an embarrassing solution to what I imagine
  // is a simple arithmatic problem
  if (temp > 108) {
    // double whole
    return 108 + 10;
  }
  else if (temp > 90) {
    // whole
    return 90 + 10;
  }
  else if (temp > 72) {
    // half
    return 72 + 10;
  }
  else if (temp > 54) {
    // quarter
    return 54 + 10;
  }
  else if (temp > 36) {
    // 8th
    return 36 + 10;
  }
  else if (temp > 18) {
    // 16th
    return 18 + 10;
  }
  else {
    // random
    return 0;
  }
}

// TODO: we store values as 0-127 (because that's what MIDI uses)
// however we display values as 0-99 (because we only have two free digits)
// this means that sometimes we have to push the button an extra time
// to get the screen to update (even though it is updating the actual value correctly)
byte ccStepTransform(byte value, bool stepUp, bool shift) {
  int stride = shift ? 10 : 1;
  stride *= stepUp ? 1 : -1;
  int temp = value;
  temp += stride;
  temp = min(temp, 127);
  temp = max(temp, 0);

  return temp;
}

byte onOffStepTransform(byte value, bool stepUp, bool shift) {
  return stepUp ? 127 : 0;
}

SettingManager::SettingManager(Expressions* _expr, ButtonManager* _buttons) : expr(_expr), buttons(_buttons) {
  baseNoteLength = new Setting(0, 20, CHAR_N, CHAR_L, noteLengthValueTransform, noteLenthStepTransform);
  octaveOneUpChance = new Setting(10, 22, B01100011, B01000000, ccValueTransform, ccStepTransform);
  octaveOneDownChance = new Setting(10, 23, B00011101, B00001000, ccValueTransform, ccStepTransform);

  useSpeaker = new Setting(0, 119, CHAR_S, CHAR_P, onOffValueTransform, onOffStepTransform);
  // TODO can we add an onchange callback or something to trigger sort of currently pressed/active notes?
  sort = new Setting(0, 114, CHAR_S, CHAR_O, onOffValueTransform, onOffStepTransform);

  sequenceSettings[0] = baseNoteLength;
  sequenceSettings[1] = octaveOneUpChance;
  sequenceSettings[2] = octaveOneDownChance;

  generalSettings[0] = useSpeaker;
  generalSettings[1] = sort;
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
  }
  else if (menuStage > 1 && (buttons->up.released || buttons->down.released)) {
    Setting* setting = menuStage == 2
      ? sequenceSettings[menuIndex %  SEQUENCE_SETTING_COUNT]
      : generalSettings[menuIndex %  GENERAL_SETTING_COUNT];
    
    setting->step(buttons->up.released, buttons->forward.held);
    writeMenu();
    return;
  }
  else if (menuStage > 0) {
    if (buttons->backward.released) {
      menuStage = (menuStage == 1) ? 0 : 1;
      menuIndex = 0;
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