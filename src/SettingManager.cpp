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

void swingValueTransform(byte value, byte output[2]) {
  byte mapped = map(value, 0, 127, 50, 67);

  output[0] = ExpressionSets::convertNumberToByte((mapped / 10) % 10);
  output[1] = ExpressionSets::convertNumberToByte(mapped % 10);
}

byte swingStepTransform(byte value, bool stepUp, bool shift) {
  if (shift) {
    return stepUp ? 127 : 0;
  }

  int stride = ceil(127.0 / (67 - 50));
  stride *= stepUp ? 1 : -1;
  int temp = value;
  temp += stride;
  temp = min(temp, 127);
  temp = max(temp, 0);

  return temp;
}

void midiChValueTransform(byte value, byte output[2]) {
  byte index = Stepper::getSteppedIndex(value, 17);
  
  // we use channel 0 to mean "no channel transform"
  if (index == 0) {
    output[0] = CHAR_A;
    output[1] = CHAR_L;
    return;
  }

  if (index < 10) {
    output[0] = CHAR_BLANK;
    output[1] = ExpressionSets::convertNumberToByte(index);
    return;
  }

  output[0] = ExpressionSets::convertNumberToByte((index / 10) % 10);
  output[1] = ExpressionSets::convertNumberToByte(index % 10);
}

byte midiChStepTransform(byte value, bool stepUp, bool shift) {
  if (shift) {
    if (stepUp) {
      return 127;
    } else {
      return 0;
    }
  }

  return Stepper::stepIndex(value, 17, stepUp);
}

void onOffValueTransform(byte value, byte output[2]) {
  output[0] = CHAR_O;

  if (value < 64) {
    output[1] = CHAR_F;
  } else {
    output[1] = CHAR_N;
  }
}

byte onOffStepTransform(byte value, bool stepUp, bool shift) {
  return stepUp ? 127 : 0;
}

void noteLengthValueTransform(byte value, byte output[2]) {
  byte index = Stepper::getSteppedIndex(value, 7);

  if (index == 6) {
    // double whole
    output[0] = CHAR_2;
    output[1] = CHAR_DASH;
  }
  else if (index == 5) {
    // whole
    output[0] = CHAR_1;
    output[1] = CHAR_DASH;
  }
  else if (index == 4) {
    // half
    output[0] = CHAR_H;
    output[1] = CHAR_A;
  }
  else if (index == 3) {
    // quarter
    output[0] = CHAR_BLANK;
    output[1] = CHAR_1;
  }
  else if (index == 2) {
    // 8th
    output[0] = CHAR_BLANK;
    output[1] = CHAR_8;
  }
  else if (index == 1) {
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
      return 127;
    } else {
      return 0;
    }
  }

  return Stepper::stepIndex(value, 7, stepUp);
}

void sequenceLengthValueTransform(byte value, byte output[2]) {
  byte index = Stepper::getSteppedIndex(value, 9);

  // Random
  if (index == 0) {
    output[0] = CHAR_R;
    output[1] = CHAR_A;
    return;
  }
  else {
    output[0] = CHAR_BLANK;
    output[1] = ExpressionSets::convertNumberToByte(index);
  }
}

byte sequenceLenthStepTransform(byte value, bool stepUp, bool shift) {
  if (shift) {
    if (stepUp) {
      return 127;
    } else {
      return 0;
    }
  }

  return Stepper::stepIndex(value, 9, stepUp);
}

SettingManager::SettingManager(Expressions* _expr, ButtonManager* _buttons) : expr(_expr), buttons(_buttons) {
  sequenceLength = new Setting(0, 21, CHAR_S, CHAR_L, sequenceLengthValueTransform, sequenceLenthStepTransform);
  baseNoteLength = new Setting(0, 20, CHAR_N, CHAR_L, noteLengthValueTransform, noteLenthStepTransform);
  octaveOneUpChance = new Setting(10, 22, B01100011, B01000000, ccValueTransform, ccStepTransform);
  octaveOneDownChance = new Setting(10, 23, B00011101, B00001000, ccValueTransform, ccStepTransform);

  swing = new Setting(0, 115, CHAR_S, CHAR_G, swingValueTransform, swingStepTransform);
  useSpeaker = new Setting(0, 119, CHAR_S, CHAR_P, onOffValueTransform, onOffStepTransform);
  // TODO can we add an onchange callback or something to trigger sort of currently pressed/active notes?
  sort = new Setting(0, 114, CHAR_S, CHAR_O, onOffValueTransform, onOffStepTransform);
  midiChannelIn = new Setting(0, 14, CHAR_I, CHAR_N, midiChValueTransform, midiChStepTransform);
  midiChannelOut = new Setting(0, 15, CHAR_O, CHAR_T, midiChValueTransform, midiChStepTransform);

  sequenceSettings[0] = sequenceLength;
  sequenceSettings[1] = baseNoteLength;
  sequenceSettings[2] = octaveOneUpChance;
  sequenceSettings[3] = octaveOneDownChance;

  generalSettings[0] = swing;
  generalSettings[1] = useSpeaker;
  generalSettings[2] = sort;
  generalSettings[3] = midiChannelIn;
  generalSettings[4] = midiChannelOut;
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