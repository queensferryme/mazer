#include "GameResult.h"
#include "Config.h"
#include "Music.h"
#include "Ranking.h"
#include "StartMenu.h"
#include "sqlite3.h"
#include "ui/CocosGUI.h"

/* create scene with `static create()` */
Scene *GameResult::createScene(bool isSuccess, int playerScore) {
  auto scene = GameResult::create();
  scene->playerScore = std::to_string(playerScore);
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
  gameResultLable->setPosition(Vec2(visibleSize.width / 2 + origin.x,
                                    visibleSize.height / 2 + origin.y));
  auto menu = Menu::create(gameResultLable, NULL);
  menu->setPosition(Vec2::ZERO);
  scene->addChild(menu, 1);
  // add username input box if success
  if (isSuccess) {
    auto textField =
        ui::TextField::create("input your name here", "fonts/arial.ttf", 20);
    textField->setPosition(Vec2(visibleSize.width / 2 + origin.x,
                                visibleSize.height / 2 + origin.y - 100));
    scene->addChild(textField);
    scene->textField = textField;
  }
  return scene;
}

/* initialize current cocos2d::Scene object */
bool GameResult::init() {
  if (!Scene::init())
    return false;
  // add back label
  auto backItem = MenuItemImage::create(
      "img/BackNormal.png", "img/BackClicked.png", [&](Ref *pSpender) {
        playSoundEffect("audio/click.wav");
        if (textField != nullptr) {
          Ranking::db = Ranking::openDataBase(Config::dbFilePath, Ranking::db);
          Ranking::insertData(textField->getString(), playerScore, Ranking::db);
          sqlite3_close(Ranking::db);
        }
        Director::getInstance()->replaceScene(StartMenu::createScene());
      });
  backItem->setPosition(Vec2(60, 660));
  auto menu = Menu::create(backItem, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);
  return true;
}
