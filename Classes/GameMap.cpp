#include <iostream>

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
  auto spawnPoint = map->getObjectGroup("Objects")->getObject("SpawnPoint");
  auto player = Sprite::create("player.png");
  player->setPosition(
      Vec2(spawnPoint["x"].asFloat(), spawnPoint["y"].asFloat()));
  this->addChild(player);
  this->player = player;
  this->focusSceneOnPlayer();
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

/* Register an update function for cocos2d
   which performs scheduled checking tasks */
void GameMap::update(float delta) {
  Node::update(delta);
  // calculate current player's next position
  // TODO: speed up factor varies with difficulty
  Vec2 position = player->getPosition() + (5 * playerDirection);
  // don't update if poisition overflows.
  // `TMXTiledMap::getMapSize` returns 64×64 tiles,
  // convert `mapSize` into size in pixels.
  Size mapSize = map->getMapSize();
  mapSize.width = mapSize.width * map->getTileSize().width;
  mapSize.height = mapSize.height * map->getTileSize().height;
  if (position.x >= mapSize.width || position.y >= mapSize.height ||
      position.x < 0 || position.y < 0)
    return;
  // get tileGID of current player's next position
  int tileGID = ([=](Vec2 position) {
    float x{position.x / map->getTileSize().width},
        y{(map->getMapSize().height * map->getTileSize().height - position.y) /
          map->getTileSize().height};
    return map->getLayer("Background")->getTileGIDAt(Vec2(x, y));
  })(position);
  // don't update if next tile is collidable
  if (!map->getPropertiesForGID(tileGID).isNull()) {
    auto prop = map->getPropertiesForGID(tileGID).asValueMap();
    if (prop["isCollidable"].asString() == "true") {
      return;
    }
  }
  player->setPosition(position);
  this->focusSceneOnPlayer();
}

/* set view point center to current player */
void GameMap::focusSceneOnPlayer() {
  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 position = player->getPosition();
  int x = MIN(MAX(position.x, visibleSize.width / 2),
              (map->getMapSize().width * map->getTileSize().width) -
                  visibleSize.width / 2);
  int y = MIN(MAX(position.y, visibleSize.height / 2),
              (map->getMapSize().height * map->getTileSize().height) -
                  visibleSize.height / 2);
  Vec2 currentCenter = Point(visibleSize.width / 2, visibleSize.height / 2),
       targetCenter = Point(x, y);
  this->setPosition(currentCenter - targetCenter);
}
