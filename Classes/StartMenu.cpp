#include "StartMenu.h"
#include "Config.h"
#include "GameMap.h"
#include "Music.h"
#include "Settings.h"

using namespace CocosDenshion;
using namespace cocos2d;
using namespace cocos2d::ui;

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
  updateBackgroundMusic();
  // create a selective menu:
  // start, rank, setting, about, exit
  Vector<MenuItem *> menuItems;
  menuItems.pushBack(createMenuItemLabel("Start", [](Ref *pSender) {
    playSoundEffect("click.wav");
    Director::getInstance()->replaceScene(GameMap::createScene());
  }));
  menuItems.pushBack(createMenuItemLabel("Settings", [](Ref *pSender) {
    playSoundEffect("click.wav");
    Director::getInstance()->pushScene(Settings::create());
  }));
  menuItems.pushBack(createMenuItemLabel(
      "About", [](Ref *pSender) { playSoundEffect("click.wav"); }));
  menuItems.pushBack(createMenuItemLabel(
      "Ranking", [](Ref *pSender) { playSoundEffect("click.wav"); }));
  menuItems.pushBack(createMenuItemLabel("Exit", [](Ref *pSender) {
    playSoundEffect("click.wav");
    Director::getInstance()->end();
  }));
  for (int i = 0; i < menuItems.size(); i++)
    menuItems.at(i)->setPosition(Vec2(i * 60, 10));
  auto menu = Menu::createWithArray(menuItems);
  menu->setPosition(Vec2(110, 50));
  this->addChild(menu, 1);
  return true;
}
