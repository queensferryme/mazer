#include "GameResult.h"
#include "Music.h"
#include "StartMenu.h"

/* create scene with `static create()` */
Scene *GameResult::createScene(bool isSuccess, int playerScore) {
  auto scene = GameResult::create();
  // add game result information
  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  MenuItemLabel *gameResultLable;
  if (!isSuccess)
    gameResultLable = MenuItemLabel::create(
        Label::createWithTTF("You Failed!", "fonts/MarkerFelt.ttf", 32));
  else {
    char result[100];
    sprintf(result, "You Win!\nScore: %d", playerScore);
    gameResultLable = MenuItemLabel::create(
        Label::createWithTTF(result, "fonts/MarkerFelt.ttf", 32));
  }
  gameResultLable->setPosition(Vec2(visibleSize.width / 2 + origin.x + 10,
                                    visibleSize.height / 2 + origin.y));
  auto menu = Menu::create(gameResultLable, NULL);
  menu->setPosition(Vec2::ZERO);
  scene->addChild(menu, 1);
  return scene;
}

/* initialize current cocos2d::Scene object */
bool GameResult::init() {
  if (!Scene::init())
    return false;
  // add back label
  auto backItem = MenuItemImage::create(
      "img/BackNormal.png", "img/BackClicked.png", [](Ref *pSpender) {
        playSoundEffect("click.wav");
        Director::getInstance()->replaceScene(StartMenu::createScene());
      });
  backItem->setPosition(Vec2(60, 660));
  auto menu = Menu::create(backItem, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);
  return true;
}
