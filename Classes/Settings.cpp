#include "Settings.h"
#include "Config.h"
#include "Music.h"

using namespace cocos2d;
using namespace cocos2d::ui;

/* create a checkBox for boolean setting */
void Settings::createSettingCheckBox(
    const std::string &text, const Vec2 &position, const bool isSelected,
    const CheckBox::ccCheckBoxCallback &callback) {
  // create checkbox
  auto checkbox =
      CheckBox::create("img/CheckBoxPress.png", "img/CheckBoxPress.png",
                       "img/CheckBoxNodeDisable.png",
                       "img/CheckBoxNodeNormal.png", "img/CheckBoxNormal.png");
  checkbox->setSelected(isSelected);
  checkbox->addEventListener(callback);
  // create descriptive label
  auto label = Label::createWithTTF(text, "fonts/MarkerFelt.ttf", 25);
  checkbox->setPosition(position);
  label->setPosition(Vec2(position.x, position.y + 50));
  this->addChild(checkbox);
  this->addChild(label);
}

/* create a slider for value control */
void Settings::createSlider(float storedValue, const Vec2 &position,
                            const Slider::ccSliderCallback &callback) {
  auto slider = Slider::create();
  slider->loadBarTexture("img/SliderBack.png");
  slider->loadSlidBallTextures("img/SliderNodeNormal.png",
                               "img/SliderNodePress.png",
                               "img/SliderNodeDisable.png");
  slider->loadProgressBarTexture("img/SliderPressBar.png");
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
  // create background image
  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  auto sprite = Sprite::create("img/start.jpg");
  sprite->setScale(2);
  sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x + 10,
                           visibleSize.height / 2 + origin.y));
  this->addChild(sprite, 0);
  // create back label
  auto backLabel = MenuItemLabel::create(
      Label::createWithTTF("Back", "fonts/MarkerFelt.ttf", 25),
      [](Ref *pSender) {
        playSoundEffect("click.wav");
        Director::getInstance()->popScene();
      });
  auto backMenu = Menu::create(backLabel, NULL);
  backMenu->setPosition(Vec2(50, 675));
  this->addChild(backMenu);
  // create music & sound checkbox
  createSettingCheckBox("Music", Vec2(100, 500), Config::isMusicOn,
                        [](Ref *pSender, CheckBox::EventType type) {
                          Config::toggleMusicState();
                          updateBackgroundMusic();
                          static_cast<CheckBox *>(pSender)->setSelected(
                              Config::isMusicOn);
                        });
  createSettingCheckBox("Sound", Vec2(100, 400), Config::isSoundOn,
                        [](Ref *pSender, CheckBox::EventType type) {
                          Config::toggleSoundState();
                          static_cast<CheckBox *>(pSender)->setSelected(
                              Config::isSoundOn);
                        });
  // create music & sound slider
  createSlider(Config::musicVolume, Vec2(250, 500),
               [&](Ref *pSender, Slider::EventType type) {
                 Config::music->setBackgroundMusicVolume(
                     static_cast<Slider *>(pSender)->getPercent() / 100.0f);
                 Config::setMusicVolume(
                     Config::music->getBackgroundMusicVolume());
               });
  createSlider(Config::soundVolume, Vec2(250, 400),
               [&](Ref *pSender, Slider::EventType type) {
                 Config::setSoundVolume(
                     static_cast<Slider *>(pSender)->getPercent() / 100.0f);
               });
  // create enemy speed-up factor slider
  auto speedLable = MenuItemLabel::create(
      Label::createWithTTF("Speed", "fonts/MarkerFelt.ttf", 25));
  speedLable->setPosition(Vec2(100, 300));
  this->addChild(speedLable);
  createSlider(0.25 * Config::speedUpFactor - 0.25, Vec2(250, 300),
               [&](Ref *pSender, Slider::EventType type) {
                 Config::speedUpFactor =
                     ((static_cast<Slider *>(pSender)->getPercent() / 100.0f) +
                      0.25) *
                     4;
               });
  return true;
}
