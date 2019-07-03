#include "StartMenu.h"
#include "Settings.h"
#include "utils.h"

using namespace cocos2d;

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
  Vector<MenuItem *> menuItems;
  menuItems.pushBack(createMenuItemLabel("Start"));
  menuItems.pushBack(createMenuItemLabel("Settings"));
  menuItems.pushBack(createMenuItemLabel("About"));
  menuItems.pushBack(createMenuItemLabel("Ranking"));
  menuItems.pushBack(createMenuItemLabel(
      "Exit", [](Ref *pSender) { Director::getInstance()->end(); }));
  for (int i = 0; i < menuItems.size(); i++)
    menuItems.at(i)->setPosition(Vec2(i * 60, 10));
  auto menu = Menu::createWithArray(menuItems);
  menu->setPosition(Vec2(110, 50));
  this->addChild(menu, 1);
  return true;
}
