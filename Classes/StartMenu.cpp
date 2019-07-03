#include "StartMenu.h"
#include "Settings.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace CocosDenshion;


Music soundSetting; // used for music settings

/* create a text menu item for StartMenu Scene */
MenuItem *createMenuItemLabel(const std::string &text,
                              const ccMenuCallback &callback =
                                  [](Ref *pSender) {}) {
  return MenuItemLabel::create(
      Label::createWithTTF(text, "fonts/Marker Felt.ttf", 15), callback);
}

/* initialize StartMenu Scene object */
bool StartMenu::init() {
  // if already initialized
  if (!Scene::init())
    return false;
  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  // create a selective menu:
  // start, rank, setting, about, exit
  soundSetting.music->playBackgroundMusic("Honor.mp3");

  Vector<MenuItem *> menuItems;
  menuItems.pushBack(createMenuItemLabel(
      "Start", [](Ref *pSender) { ClickSound(soundSetting); }));
  menuItems.pushBack(createMenuItemLabel("Settings", [](Ref *pSender) {
    auto settingsScene = Settings::create();
    Director::getInstance()->pushScene(settingsScene);
    ClickSound(soundSetting);
  }));
  menuItems.pushBack(createMenuItemLabel(
      "About", [](Ref *pSender) { ClickSound(soundSetting); }));
  menuItems.pushBack(createMenuItemLabel(
      "Ranking", [](Ref *pSender) { ClickSound(soundSetting); }));
  menuItems.pushBack(createMenuItemLabel("Exit", [](Ref *pSender) {
    ClickSound(soundSetting);
    Director::getInstance()->end();
  }));
  for (int i = 0; i < menuItems.size(); i++)
    menuItems.at(i)->setPosition(Vec2(i * 60, 10));
  auto menu = Menu::createWithArray(menuItems);
  menu->setPosition(Vec2(110, 50));
  this->addChild(menu, 1);
  return true;
}
