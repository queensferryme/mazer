#include "StartMenu.h"
#include "About.h"
#include "Config.h"
#include "GameMap.h"
#include "Music.h"
#include "Ranking.h"
#include "Settings.h"

using namespace cocos2d;

/* create a text menu item for StartMenu Scene */
MenuItem *createMenuItemLabel(
    const std::string &text,
    const ccMenuCallback &callback = [](Ref *pSender) {}) {
  return MenuItemLabel::create(
      Label::createWithTTF(text, "fonts/MarkerFelt.ttf", 25), callback);
}

/* initialize StartMenu Scene object */
bool StartMenu::init() {
  // if already initialized
  if (!Scene::init())
    return false;
  playBackgroundMusic("audio/bgm.mp3");
  updateBackgroundMusic();
  // create background image
  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  auto sprite = Sprite::create("img/start.jpg");
  sprite->setScale(2);
  sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x + 10,
                           visibleSize.height / 2 + origin.y));
  this->addChild(sprite, 0);
  // create a selective menu:
  // start, rank, setting, about, exit
  Vector<MenuItem *> menuItems;
  menuItems.pushBack(createMenuItemLabel("Start", [](Ref *pSender) {
    playSoundEffect("audio/click.wav");
    Director::getInstance()->replaceScene(GameMap::createScene());
  }));
  menuItems.pushBack(createMenuItemLabel("Settings", [](Ref *pSender) {
    playSoundEffect("audio/click.wav");
    Director::getInstance()->pushScene(Settings::createScene());
  }));
  menuItems.pushBack(createMenuItemLabel("About", [](Ref *pSender) {
    playSoundEffect("audio/click.wav");
    Director::getInstance()->pushScene(About::createScene());
  }));
  menuItems.pushBack(createMenuItemLabel("Ranking", [](Ref *pSender) {
    playSoundEffect("audio/click.wav");
    Director::getInstance()->pushScene(Ranking::createScene());
  }));
  menuItems.pushBack(createMenuItemLabel("Exit", [](Ref *pSender) {
    playSoundEffect("audio/click.wav");
    Director::getInstance()->end();
  }));
  for (int i = 0; i < menuItems.size(); i++)
    menuItems.at(i)->setPosition(Vec2(i * 180, 50));
  auto menu = Menu::createWithArray(menuItems);
  menu->setPosition(Vec2(110, 50));
  this->addChild(menu, 1);
  return true;
}
