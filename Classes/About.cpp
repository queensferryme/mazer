#include "About.h"
#include "Music.h"

using namespace cocos2d;

/* initialize current cocos2d::Scene object */
bool About::init() {
  if (!Scene::init())
    return false;
  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  // create back label
  auto backItem =
      MenuItemImage::create("img/BackNormal.png", "img/BackClicked.png", [](Ref *pSpender) {
        playSoundEffect("click.wav");
        Director::getInstance()->popScene();
      });
  backItem->setPosition(Vec2(60, 600));
  auto menu = Menu::create(backItem, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);
  // create about page
  auto sprite = Sprite::create("img/about.png");
  sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x + 10,
                           visibleSize.height / 2 + origin.y));
  this->addChild(sprite, 0);
  return true;
}
