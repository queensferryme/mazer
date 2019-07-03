#include "Settings.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace CocosDenshion;

void changeState(bool &setting, Music &musicAndSound,
                 CheckBox::EventType type) {
  switch (type) {
  case cocos2d::ui::CheckBox::EventType::SELECTED:
    std::cout << "on" << std::endl;
    setting = true;
    musicAndSound.changeSoundWorkState(musicAndSound.isSoundOn);
    musicAndSound.changeMusicWorkState(musicAndSound.isMusicOn);
    // audio->resumeBackgroundMusic();
    break;
  case cocos2d::ui::CheckBox::EventType::UNSELECTED:
    std::cout << "off" << std::endl;
    setting = false;
    musicAndSound.changeSoundWorkState(musicAndSound.isSoundOn);
    musicAndSound.changeMusicWorkState(musicAndSound.isMusicOn);
    // audio->pauseBackgroundMusic();
    break;
  default:
    break;
  }
}

void Settings::createSettingCheckBox(
    const std::string &text, const Vec2 &v2, bool isSelected,
    const cocos2d::ui::CheckBox::ccCheckBoxCallback &callback) {
  auto checkbox = ui::CheckBox::create(
      "CheckBox_Press.png", "CheckBox_Press.png", "CheckBoxNode_Disable.png",
      "CheckBoxNode_Normal.png", "CheckBox_Normal.png");
  checkbox->setSelectedState(isSelected);
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
    case ui::Widget::TouchEventType::BEGAN:
      break;
    case ui::Widget::TouchEventType::ENDED:
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

  auto backLabel = MenuItemLabel::create(
      Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 15),
      [](Ref *pSender) {
        ClickSound(soundSetting);
        Director::getInstance()->popScene();
      });
  auto backMenu = Menu::create(backLabel, NULL);
  backMenu->setPosition(Vec2(50, 300));
  this->addChild(backMenu);

  // set the background music and continuously play it.

  // create a text menu item for Settings Scene

  createSettingCheckBox("Music", Vec2(100, 120), soundSetting.isMusicOn,
                        [](Ref *pSender, CheckBox::EventType type) {
                          changeState(soundSetting.isMusicOn, soundSetting,
                                      type);
                        });
  createSettingCheckBox("Sound", Vec2(100, 160), soundSetting.isSoundOn,
                        [](Ref *pSender, CheckBox::EventType type) {
                          changeState(soundSetting.isSoundOn, soundSetting,
                                      type);
                        });
  createSlider(Vec2(200, 120));
  createSlider(Vec2(200, 160));

  return true;
}

void ClickSound(const Music &Sound) {
  if (Sound.isSoundOn)
    soundSetting.sound->playEffect("click.wav", false, 1.0f, 1.0f, 1.0f);
}
