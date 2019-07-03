#include <iostream>

#include "Settings.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace CocosDenshion;

void changeState(bool &setting, Music &musicAndSound,
                 CheckBox::EventType type) {
  switch (type) {
  case ::CheckBox::EventType::SELECTED:
    setting = true;
    musicAndSound.changeSoundWorkState(musicAndSound.isSoundOn);
    musicAndSound.changeMusicWorkState(musicAndSound.isMusicOn);
    // audio->resumeBackgroundMusic();
    break;
  case ::CheckBox::EventType::UNSELECTED:
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
/*ON_PERCENTAGE_CHANGED,
        //@since v3.7
        ON_SLIDEBALL_DOWN,
        ON_SLIDEBALL_UP,
        ON_SLIDEBALL_CANCEL */

void Settings::createSlider(
    float &storedValue, const Vec2 &v2,
    const cocos2d::ui::Slider::ccSliderCallback &callback) {
  auto slider = Slider::create();

  slider->loadBarTexture("Slider_Back.png"); // what the slider looks like
  slider->loadSlidBallTextures("SliderNode_Normal.png", "SliderNode_Press.png",
                               "SliderNode_Disable.png");
  slider->loadProgressBarTexture("Slider_PressBar.png");
  slider->setMaxPercent(100);
  slider->setPercent(storedValue * 100);
  slider->addEventListener(callback);
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
  // soundSetting.sound->setEffectsVolume(0.0);
  // soundSetting.sound->playEffect("click.wav", true, 1.0f, 1.0f, 1.0f);

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
  createSlider(
      soundSetting.musicVolume, Vec2(200, 120),
      [&](Ref *pSender, Slider::EventType type) {
        auto item = (Slider *)(pSender);
        soundSetting.music->setBackgroundMusicVolume(item->getPercent() /
                                                     100.0f);
        soundSetting.musicVolume =
            SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
      });
  createSlider(soundSetting.soundVolume, Vec2(200, 160),
               [&](Ref *pSender, Slider::EventType type) {
                 auto item = (Slider *)(pSender);
                 SimpleAudioEngine::getInstance()->setEffectsVolume(
                     item->getPercent() / 100.0f);
                 soundSetting.soundVolume =
                     SimpleAudioEngine::getInstance()->getEffectsVolume();
               });

  return true;
}

void ClickSound(Music &Sound) {

  if (Sound.isSoundOn) {
    SimpleAudioEngine::getInstance()->setEffectsVolume(Sound.soundVolume);
    Sound.sound->play2d("click.wav", false, Sound.soundVolume);
  }
}
