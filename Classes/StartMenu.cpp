#include "StartMenu.h"
#include "GameMap.h"
#include "Config.h"
#include "Settings.h"

using namespace CocosDenshion;
using namespace cocos2d;
using namespace cocos2d::ui;

extern void makeClickSound();

/* create a text menu item for StartMenu Scene */
MenuItem *createMenuItemLabel(
    const std::string &text,
    const ccMenuCallback &callback = [](Ref *pSender) {}) {
  return MenuItemLabel::create(
      Label::createWithTTF(text, "fonts/Marker Felt.ttf", 15), callback);
}

/* initialize StartMenu Scene object */
bool StartMenu::init() {
  // if already initialized
  if (!Scene::init())
    return false;
  Size visibleSize = Director::getInstance()->getVisibleSize();
  // create a selective menu:
  // start, rank, setting, about, exit
  Config::music->playBackgroundMusic("Honor.mp3");
  Vector<MenuItem *> menuItems;
  menuItems.pushBack(createMenuItemLabel("Start", [](Ref *pSender) {
    makeClickSound();
    Director::getInstance()->replaceScene(GameMap::createScene());
  }));
  menuItems.pushBack(createMenuItemLabel("Settings", [](Ref *pSender) {
    makeClickSound();
    Director::getInstance()->pushScene(Settings::create());
  }));
  menuItems.pushBack(createMenuItemLabel(
      "About", [](Ref *pSender) { makeClickSound(); }));
  menuItems.pushBack(createMenuItemLabel(
      "Ranking", [](Ref *pSender) { makeClickSound(); }));
  menuItems.pushBack(createMenuItemLabel(
      "Exit", [](Ref *pSender) { Director::getInstance()->end(); }));
  for (int i = 0; i < menuItems.size(); i++)
    menuItems.at(i)->setPosition(Vec2(i * 60, 10));
  auto menu = Menu::createWithArray(menuItems);
  menu->setPosition(Vec2(110, 50));
  this->addChild(menu, 1);
  return true;
}
