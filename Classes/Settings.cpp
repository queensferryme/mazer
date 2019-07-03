#include <iostream>

#include "Settings.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace CocosDenshion;

void changeState(bool setting, CheckBox::EventType type) {
  switch (type) {
  case ::CheckBox::EventType::SELECTED:
    setting = true;
    break;
  case ::CheckBox::EventType::UNSELECTED:
    setting = false;
    break;
  }
}

bool Settings::isMusicOn{false};
bool Settings::isSoundOn{false};

void Settings::createSettingCheckBox(
    const std::string &text, const Vec2 &v2,
    const CheckBox::ccCheckBoxCallback &callback) {
  auto checkbox = CheckBox::create(
      "CheckBox_Press.png", "CheckBox_Press.png", "CheckBoxNode_Disable.png",
      "CheckBoxNode_Normal.png", "CheckBox_Normal.png");
  checkbox->addEventListener(callback);
  auto label = Label::createWithTTF(text, "fonts/Marker Felt.ttf", 15);
  checkbox->setPosition(v2);
  label->setPosition(Vec2(v2.x, v2.y + 20));
  this->addChild(checkbox);
  this->addChild(label);
}
void Settings::createSlider(const Vec2 &v2) {
  auto slider = Slider::create();
  slider->loadBarTexture("Slider_Back.png"); // what the slider looks like
  slider->loadSlidBallTextures("SliderNode_Normal.png", "SliderNode_Press.png",
                               "SliderNode_Disable.png");
  slider->loadProgressBarTexture("Slider_PressBar.png");
  slider->addTouchEventListener([&](Ref *sender, Widget::TouchEventType type) {
    switch (type) {
    case Widget::TouchEventType::BEGAN:
      break;
    case Widget::TouchEventType::ENDED:
      std::cout << "slider moved" << std::endl;
      break;
    default:
      break;
    }
  });
  slider->setPosition(v2);
  this->addChild(slider);
}

bool Settings::init() {
  if (!Scene::init())
    return false;
  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  auto audio = SimpleAudioEngine::getInstance();
  // set the background music and continuously play it.
  audio->playBackgroundMusic("Honor.mp3", true);
  // create a text menu item for Settings Scene
  createSettingCheckBox("Music", Vec2(100, 120),
                        [](Ref *pSender, CheckBox::EventType type) {
                          changeState(isMusicOn, type);
                        });
  createSettingCheckBox("Sound", Vec2(100, 160),
                        [](Ref *pSender, CheckBox::EventType type) {
                          changeState(isSoundOn, type);
                        });
  createSlider(Vec2(200, 120));
  createSlider(Vec2(200, 160));
  return true;
}