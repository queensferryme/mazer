#include <iostream>

#include "Config.h"
#include "Settings.h"

using namespace CocosDenshion;
using namespace cocos2d;
using namespace cocos2d::ui;

extern void makeClickSound();

/* helper function which toggle checkbox when clicked */
void toggleCheckBoxState(CheckBox::EventType type) {
  switch (type) {
  case CheckBox::EventType::SELECTED:
    Config::isMusicOn = true;
    break;
  case ::CheckBox::EventType::UNSELECTED:
    Config::isMusicOn = false;
    break;
  }
  Config::toggleMusicState(Config::isMusicOn);
}

/* create a checkBox for boolean setting */
void Settings::createSettingCheckBox(
    const std::string &text, const Vec2 &position, const bool isSelected,
    const CheckBox::ccCheckBoxCallback &callback) {
  // create checkbox
  auto checkbox = CheckBox::create(
      "CheckBox_Press.png", "CheckBox_Press.png", "CheckBoxNode_Disable.png",
      "CheckBoxNode_Normal.png", "CheckBox_Normal.png");
  checkbox->setSelectedState(isSelected);
  checkbox->addEventListener(callback);
  // create descriptive label
  auto label = Label::createWithTTF(text, "fonts/Marker Felt.ttf", 15);
  checkbox->setPosition(position);
  label->setPosition(Vec2(position.x, position.y + 20));
  this->addChild(checkbox);
  this->addChild(label);
}

/* create a slider for value control */
void Settings::createSlider(float &storedValue, const Vec2 &position,
                            const Slider::ccSliderCallback &callback) {
  auto slider = Slider::create();
  slider->loadBarTexture("Slider_Back.png");
  slider->loadSlidBallTextures("SliderNode_Normal.png", "SliderNode_Press.png",
                               "SliderNode_Disable.png");
  slider->loadProgressBarTexture("Slider_PressBar.png");
  slider->setMaxPercent(100);
  slider->setPercent(storedValue * 100);
  slider->addEventListener(callback);
  slider->setPosition(position);
  this->addChild(slider);
}

/* initialize current cocos2d::Scene object */
bool Settings::init() {
  if (!Scene::init())
    return false;
  // create back label
  auto backLabel = MenuItemLabel::create(
      Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 15),
      [](Ref *pSender) {
        makeClickSound();
        Director::getInstance()->popScene();
      });
  auto backMenu = Menu::create(backLabel, NULL);
  backMenu->setPosition(Vec2(50, 300));
  this->addChild(backMenu);
  // create music & sound checkbox
  createSettingCheckBox("Music", Vec2(100, 120), Config::isMusicOn,
                        [](Ref *pSender, CheckBox::EventType type) {
                          toggleCheckBoxState(type);
                        });
  createSettingCheckBox("Sound", Vec2(100, 160), Config::isSoundOn,
                        [](Ref *pSender, CheckBox::EventType type) {
                          toggleCheckBoxState(type);
                        });
  // create music & sound slider
  createSlider(
      Config::musicVolume, Vec2(200, 120),
      [&](Ref *pSender, Slider::EventType type) {
        auto item = (Slider *)(pSender);
        Config::music->setBackgroundMusicVolume(item->getPercent() / 100.0f);
        Config::setMusicVolume(Config::music->getBackgroundMusicVolume());
      });
  createSlider(Config::soundVolume, Vec2(200, 160),
               [&](Ref *pSender, Slider::EventType type) {
                 auto item = (Slider *)(pSender);
                 Config::music->setEffectsVolume(item->getPercent() / 100.0f);
                 Config::setMusicVolume(
                     Config::music->getBackgroundMusicVolume());
               });
  return true;
}
