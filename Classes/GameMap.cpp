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
  // `TMXLayer::getLayerSize` returns 64×64 tiles,
  // convert `layerSize` into size in pixels.
  Size layerSize = map->getLayer("Collide")->getLayerSize();
  layerSize.width = layerSize.width * map->getTileSize().width;
  layerSize.height = layerSize.height * map->getTileSize().height;
  if (position.x >= layerSize.width || position.y >= layerSize.height ||
      position.x < 0 || position.y < 0)
    return;
  // get tileGID of current player's next position
  int tileGID = ([=](Vec2 position) {
    float x{position.x / map->getTileSize().width},
        y{(map->getMapSize().height * map->getTileSize().height - position.y) /
          map->getTileSize().height};
    return map->getLayer("Collide")->getTileGIDAt(Vec2(x, y));
  })(position);
  // don't update if next tile is collidable
  if (!map->getPropertiesForGID(tileGID).isNull()) {
    auto prop = map->getPropertiesForGID(tileGID).asValueMap();
    if (prop["Collidable"].asString() == "true")
      return;
  }
  player->setPosition(position);
  this->focusSceneOnPlayer();
}

/* set view point center to current player */
void GameMap::focusSceneOnPlayer() {
  // get window size & player position
  Size winSize = Director::getInstance()->getWinSize();
  Vec2 position = player->getPosition();
  // calculate actual position of player
  float x = MIN(MAX(position.x, winSize.width / 2),
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
