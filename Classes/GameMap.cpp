#include <chrono>
#include <thread>

#include "GameMap.h"

using namespace cocos2d;

/* initialize GameMap Scene object */
bool GameMap::init() {
  // if already initialized
  if (!Scene::init())
    return false;
  // initialize tiled map
  auto map = TMXTiledMap::create("res/map.tmx");
  this->addChild(map);
  this->map = map;
  // initialize player sprite
  auto spawn = map->getObjectGroup("Objects")->getObject("SpawnPoint");
  auto player = Sprite::create("player.png");
  player->setPosition(Vec2(spawn["x"].asInt(), spawn["y"].asInt()));
  this->focusSceneOnPlayer();
  this->addChild(player);
  this->player = player;
  // initialize key board event listener
  auto listener = EventListenerKeyboard::create();
  listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event *_) {
    if (keyCode == EventKeyboard::KeyCode::KEY_W ||
        keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
      playerDirection = Vec2(0, 1);
    else if (keyCode == EventKeyboard::KeyCode::KEY_S ||
             keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
      playerDirection = Vec2(0, -1);
    else if (keyCode == EventKeyboard::KeyCode::KEY_A ||
             keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
      playerDirection = Vec2(-1, 0);
    else if (keyCode == EventKeyboard::KeyCode::KEY_D ||
             keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
      playerDirection = Vec2(1, 0);
  };
  listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event *_) {
    playerDirection = Vec2(0, 0);
  };
  Director::getInstance()
      ->getEventDispatcher()
      ->addEventListenerWithSceneGraphPriority(listener, this);
  this->scheduleUpdate();
  return true;
}

void GameMap::update(float delta) {
  Node::update(delta);
  // TODO: speed up factor varies with difficulty
  player->runAction(MoveBy::create(.5, 5 * playerDirection));
  this->focusSceneOnPlayer();
}

/* set view point center to current player */
void GameMap::focusSceneOnPlayer() {
  // get window size & player position
  Size winSize = Director::getInstance()->getWinSize();
  Vec2 position = this->player->getPosition();
  // calculate actual position of player
  int x = MIN(MAX(position.x, winSize.width / 2),
              (map->getMapSize().width * map->getTileSize().width) -
                  winSize.width / 2),
      y = MIN(MAX(position.y, winSize.height / 2),
              (map->getMapSize().height * map->getTileSize().height) -
                  winSize.height / 2);
  Vec2 currentCenter = Vec2(winSize.width / 2, winSize.height / 2),
       actualPosition = Vec2(x, y),
       viewPointCenter = currentCenter - actualPosition;
  this->setPosition(viewPointCenter);
}
