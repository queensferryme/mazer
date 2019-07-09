#pragma once

#include <string>

#include "Config.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::ui;

/**
@brief   Settings scene
*/
class Settings : public cocos2d::Scene {
public:
  /**
  @brief    Create scene with `static create()`
  @return   pointer to a cocos2d::Scene object
  */
  static cocos2d::Scene *createScene() { return Settings::create(); }

  /**
  @brief    Implementation of `static create()` function,
            used in createScene()
  @return   pointer to an object of current class
  */
  CREATE_FUNC(Settings);

  /**
   @brief   Create a checkBox for boolean setting
   @return  void
   */
  void createSettingCheckBox(
      const std::string &, const Vec2 &v2, bool,
      const CheckBox::ccCheckBoxCallback & = [](Ref *pSender,
                                                CheckBox::EventType type) {},
      const int = -1);

  /**
  @brief    Create a slider for value control
  @return   void
  */
  void createSlider(float, const Vec2 &, const Slider::ccSliderCallback &);

  /**
  @brief    Initialize current cocos2d::Scene object
  @return   status of creation:
            * true -> success
            * false -> failure
  */
  virtual bool init();
};
