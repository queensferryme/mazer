#pragma once
#include "Music.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <iostream>
#include <string>
extern Music soundSetting;

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace CocosDenshion;
/**
@brief   Start-up menu scene
*/

void ClickSound(const Music &Sound);

class Settings : public cocos2d::Scene {
public:
  /**
  @brief    Create scene with `static create()`
  @return   pointer to a cocos2d::Scene object
  */
  static cocos2d::Scene *createScene() { return Settings::create(); }

  /**
   @brief   Create a checkBox for each setting
   @return  void
   */
  void createSettingCheckBox(
      const std::string &text, const Vec2 &v2, bool isSelected,
      const cocos2d::ui::CheckBox::ccCheckBoxCallback &callback =
          [](Ref *pSender, CheckBox::EventType type) {});

  /**
   @brief   Create a slider for value control
   @return  void

   */
  void createSlider(const Vec2 &v2);
  /**
  @brief    Implementation of `static create()` function,
            used in createScene()
  @return   pointer to an object of current class
  */
  CREATE_FUNC(Settings);

  /**
  @brief   Initialize current cocos2d::Scene object
  @return   status of creation:
            * true -> success
            * false -> failure
  */
  virtual bool init();
};
