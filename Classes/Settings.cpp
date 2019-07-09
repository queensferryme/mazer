#include "Settings.h"
#include "Config.h"
#include "Music.h"
#include "StartMenu.h"

using namespace cocos2d;
using namespace cocos2d::ui;

/* create a checkBox for boolean setting */
void Settings::createSettingCheckBox(
    const std::string &text, const Vec2 &position, const bool isSelected,
    const CheckBox::ccCheckBoxCallback &callback, const int tag) {
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
  this->addChild(checkbox, 0, tag);
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
  // create back label
  auto backItem = MenuItemImage::create(
      "img/BackNormal.png", "img/BackClicked.png", [](Ref *pSpender) {
        playSoundEffect("click.wav");
        Director::getInstance()->replaceScene(StartMenu::createScene());
      });
  backItem->setPosition(Vec2(60, 660));
  auto menu = Menu::create(backItem, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);
  // create music & sound checkbox
  createSettingCheckBox("Music", Vec2(200, 500), Config::isMusicOn,
                        [](Ref *pSender, CheckBox::EventType type) {
                          Config::toggleMusicState();
                          updateBackgroundMusic();
                          static_cast<CheckBox *>(pSender)->setSelected(
                              Config::isMusicOn);
                        });
  createSettingCheckBox("Sound", Vec2(200, 400), Config::isSoundOn,
                        [](Ref *pSender, CheckBox::EventType type) {
                          Config::toggleSoundState();
                          static_cast<CheckBox *>(pSender)->setSelected(
                              Config::isSoundOn);
                        });
  // create music & sound slider
  createSlider(Config::musicVolume, Vec2(350, 500),
               [&](Ref *pSender, Slider::EventType type) {
                 Config::music->setBackgroundMusicVolume(
                     static_cast<Slider *>(pSender)->getPercent() / 100.0f);
                 Config::setMusicVolume(
                     Config::music->getBackgroundMusicVolume());
               });
  createSlider(Config::soundVolume, Vec2(350, 400),
               [&](Ref *pSender, Slider::EventType type) {
                 Config::setSoundVolume(
                     static_cast<Slider *>(pSender)->getPercent() / 100.0f);
               });
  // create enemy speed-up factor slider
  auto speedLabel = MenuItemLabel::create(
      Label::createWithTTF("Speed", "fonts/MarkerFelt.ttf", 25));
  speedLabel->setPosition(Vec2(200, 330));
  this->addChild(speedLabel);
  createSlider(0.25 * Config::speedUpFactor - 0.25, Vec2(350, 330),
               [&](Ref *pSender, Slider::EventType type) {
                 Config::speedUpFactor =
                     ((static_cast<Slider *>(pSender)->getPercent() / 100.0f) +
                      0.25) *
                     4;
               });
  // create map size select
  auto mapSizeLabel = MenuItemLabel::create(
      Label::createWithTTF("Map Size", "fonts/MarkerFelt.ttf", 23));
  mapSizeLabel->setPosition(Vec2(200, 200));
  this->addChild(mapSizeLabel);
  createSettingCheckBox(
      "16*16", Vec2(300, 200), Config::mapSize == 16,
      [&](Ref *pSender, CheckBox::EventType type) {
        Config::mapSize = 16;
        for (auto child : getChildren())
          if (child->getTag() == 9)
            ((CheckBox *)child)->setSelected(false);
        ((CheckBox *)pSender)->setSelected(true);
      },
      9);
  createSettingCheckBox(
      "24*24", Vec2(400, 200), Config::mapSize == 24,
      [&](Ref *pSender, CheckBox::EventType type) {
        Config::mapSize = 24;
        for (auto child : getChildren())
          if (child->getTag() == 9)
            ((CheckBox *)child)->setSelected(false);
        ((CheckBox *)pSender)->setSelected(true);
      },
      9);
  createSettingCheckBox(
      "32*32", Vec2(500, 200), Config::mapSize == 32,
      [&](Ref *pSender, CheckBox::EventType type) {
        Config::mapSize = 32;
        for (auto child : getChildren())
          if (child->getTag() == 9)
            ((CheckBox *)child)->setSelected(false);
        ((CheckBox *)pSender)->setSelected(true);
      },
      9);
  createSettingCheckBox(
      "48*48", Vec2(600, 200), Config::mapSize == 48,
      [&](Ref *pSender, CheckBox::EventType type) {
        Config::mapSize = 48;
        for (auto child : getChildren())
          if (child->getTag() == 9)
            ((CheckBox *)child)->setSelected(false);
        ((CheckBox *)pSender)->setSelected(true);
      },
      9);
  createSettingCheckBox(
      "64*64", Vec2(700, 200), Config::mapSize == 64,
      [&](Ref *pSender, CheckBox::EventType type) {
        Config::mapSize = 64;
        for (auto child : getChildren())
          if (child->getTag() == 9)
            ((CheckBox *)child)->setSelected(false);
        ((CheckBox *)pSender)->setSelected(true);
      },
      9);
  createSettingCheckBox(
      "100*100", Vec2(800, 200), Config::mapSize == 100,
      [&](Ref *pSender, CheckBox::EventType type) {
        Config::mapSize = 100;
        for (auto child : getChildren())
          if (child->getTag() == 9)
            ((CheckBox *)child)->setSelected(false);
        ((CheckBox *)pSender)->setSelected(true);
      },
      9);
  return true;
}
