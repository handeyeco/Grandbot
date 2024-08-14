#include <SettingManager.h>

SettingManager::SettingManager(Expressions* _expr, ButtonManager* _buttons) : expr(_expr), buttons(_buttons) {
  // Settings sorted by MIDI CC

  // MIDI channel to listen to (respected by most things except panic and midiChannelIn itself)
  // options are: 0 = all channels / 1-16 = channels 1-16
  midiChannelIn = new Setting(0, 14, CHAR_I, CHAR_N, SettingTransforms::midiChValueTransform, SettingTransforms::midiChStepTransform);
  // MIDI channel to send to
  // options are: 0 = all channels / 1-16 = channels 1-16
  midiChannelOut = new Setting(0, 15, CHAR_O, CHAR_T, SettingTransforms::midiChValueTransform, SettingTransforms::midiChStepTransform);

  // Default note length in a sequence (gets transformed by other parameters per step)
  // options are: random, 1/16, 1/8, 1/4, 1/2, 1 (whole note), 2
  baseNoteLength = new Setting(0, 20, CHAR_N, CHAR_L, SettingTransforms::noteLengthValueTransform, SettingTransforms::noteLenthStepTransform);
  // The length of the generated sequence
  // options are: random, 1-8 bars
  sequenceLength = new Setting(0, 21, CHAR_S, CHAR_L, SettingTransforms::sequenceLengthValueTransform, SettingTransforms::sequenceLenthStepTransform);

  octaveOneUpChance = new Setting(10, 22, B01100011, B01000000, SettingTransforms::ccValueTransform, SettingTransforms::ccStepTransform);
  octaveOneDownChance = new Setting(10, 23, B00011101, B00001000, SettingTransforms::ccValueTransform, SettingTransforms::ccStepTransform);
  octaveTwoUpChance = new Setting(5, 24, B01100011, B01000001, SettingTransforms::ccValueTransform, SettingTransforms::ccStepTransform);
  octaveTwoDownChance = new Setting(5, 25, B00011101, B00001001, SettingTransforms::ccValueTransform, SettingTransforms::ccStepTransform);
  halfLengthChance = new Setting(0, 27, CHAR_H, CHAR_L, SettingTransforms::ccValueTransform, SettingTransforms::ccStepTransform);
  doubleLengthChance = new Setting(0, 26, CHAR_D, CHAR_L, SettingTransforms::ccValueTransform, SettingTransforms::ccStepTransform);
  ratchetChance = new Setting(10, 28, CHAR_R, CHAR_A, SettingTransforms::ccValueTransform, SettingTransforms::ccStepTransform);
  restChance = new Setting(5, 29, CHAR_R, CHAR_E, SettingTransforms::ccValueTransform, SettingTransforms::ccStepTransform);
  runChance = new Setting(0, 30, CHAR_R, CHAR_U, SettingTransforms::ccValueTransform, SettingTransforms::ccStepTransform);
  fifthChance = new Setting(0, 85, CHAR_F, CHAR_T, SettingTransforms::ccValueTransform, SettingTransforms::ccStepTransform);
  randomNoteChance = new Setting(0, 86, CHAR_R, CHAR_N, SettingTransforms::ccValueTransform, SettingTransforms::ccStepTransform);
  randomLengthChance = new Setting(0, 87, CHAR_R, CHAR_L, SettingTransforms::ccValueTransform, SettingTransforms::ccStepTransform);
  slipChance = new Setting(10, 89, CHAR_S, CHAR_C, SettingTransforms::ccValueTransform, SettingTransforms::ccStepTransform);

  // TODO can we add an onchange callback or something to trigger sort of currently pressed/active notes?
  sort = new Setting(0, 114, CHAR_S, CHAR_O, SettingTransforms::onOffValueTransform, SettingTransforms::onOffStepTransform);
  swing = new Setting(0, 115, CHAR_S, CHAR_G, SettingTransforms::swingValueTransform, SettingTransforms::swingStepTransform);
  useSpeaker = new Setting(0, 119, CHAR_S, CHAR_P, SettingTransforms::onOffValueTransform, SettingTransforms::onOffStepTransform);

  // Settings sorted for menu
  sequenceSettings[0] = slipChance;
  sequenceSettings[1] = sequenceLength;
  sequenceSettings[2] = baseNoteLength;
  sequenceSettings[3] = ratchetChance;
  sequenceSettings[4] = restChance;
  sequenceSettings[5] = octaveOneUpChance;
  sequenceSettings[6] = octaveOneDownChance;
  sequenceSettings[7] = octaveTwoUpChance;
  sequenceSettings[8] = octaveTwoDownChance;
  sequenceSettings[9] = halfLengthChance;
  sequenceSettings[10] = doubleLengthChance;
  sequenceSettings[11] = runChance;
  sequenceSettings[12] = fifthChance;
  sequenceSettings[13] = randomNoteChance;
  sequenceSettings[14] = randomLengthChance;

  generalSettings[0] = swing;
  generalSettings[1] = useSpeaker;
  generalSettings[2] = midiChannelIn;
  generalSettings[3] = midiChannelOut;
  generalSettings[4] = sort;
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

  setting->setValue(value);

  byte fullDisplay[4];
  setting->getDisplay(fullDisplay);
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
        expr->setMenu(false);
      } else {
        writeMenu();
      }
      return;
    } else if (buttons->right.released) {
      menuIndex = menuIndex == MAX_MENU_ITEMS ? 0 : (menuIndex + 1);
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
  menuIndex = 0;
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