#include <iostream>
#include <utility>

#include "GameMap.h"

using namespace cocos2d;

/* initialize GameMap Scene object */
bool GameMap::init() {
  if (!Scene::init())
    return false;
  // initialize tiled map
  auto map = TMXTiledMap::create("res/map.tmx");
  this->addChild(map);
  this->map = map;
  // initialize player sprite
  auto spawnPoint = map->getObjectGroup("Player")->getObject("SpawnPoint");
  auto player = Sprite::create("player.png");
  player->setPosition(
      Vec2(spawnPoint["x"].asFloat(), spawnPoint["y"].asFloat()));
  this->addChild(player);
  this->player = player;
  this->focusSceneOnPlayer();
  // initialize enemy sprites
  // initialize key board event listener
  auto listener = EventListenerKeyboard::create();
  listener->onKeyPressed = CC_CALLBACK_2(GameMap::onKeyPressed, this);
  listener->onKeyReleased = CC_CALLBACK_2(GameMap::onKeyReleased, this);
  Director::getInstance()
      ->getEventDispatcher()
      ->addEventListenerWithSceneGraphPriority(listener, this);
  this->scheduleUpdate();
  return true;
}

/* register an update function for cocos2d
   which performs scheduled checking tasks */
void GameMap::update(float delta) {
  Node::update(delta);
  // calculate current player's next position
  // TODO: speed up factor varies with difficulty
  if (playerDirection.x == 0 && playerDirection.y == 0)
    return;
  Vec2 position = player->getPosition() + (5 * playerDirection);
  // don't update if next tile is collidable
  int tileGID = getTileGIDForPositionInLayer("Background", position);
  if (!map->getPropertiesForGID(tileGID).isNull()) {
    auto prop = map->getPropertiesForGID(tileGID).asValueMap();
    if (prop["isCollidable"].asString() == "true")
      return;
  }
  player->setPosition(position);
  this->focusSceneOnPlayer();
  // collect map items if it's collectable
  tileGID = getTileGIDForPositionInLayer("Collect", position);
  if (!map->getPropertiesForGID(tileGID).isNull()) {
    auto prop = map->getPropertiesForGID(tileGID).asValueMap();
    if (prop["isCollectable"].asString() == "true")
      map->getLayer("Collect")->removeTileAt(getTileCoordForPosition(position));
  }
}

/* return tile GID for a position in a map layer */
int GameMap::getTileGIDForPositionInLayer(const std::string &layerName,
                                          const Vec2 &position) {
  return map->getLayer(layerName)->getTileGIDAt(
      getTileCoordForPosition(position));
}

/* return tile coordinate for a specific position */
Vec2 GameMap::getTileCoordForPosition(const Vec2 &position) {
  float x{position.x / map->getTileSize().width},
      y{(map->getMapSize().height * map->getTileSize().height - position.y) /
        map->getTileSize().height};
  return Vec2(static_cast<int>(x), static_cast<int>(y));
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

/* callback function when a key is pressed */
void GameMap::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *_) {
  switch (keyCode) {
  case EventKeyboard::KeyCode::KEY_W:
  case EventKeyboard::KeyCode::KEY_UP_ARROW:
    playerDirection = Vec2(0, 1);
    break;
  case EventKeyboard::KeyCode::KEY_S:
  case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    playerDirection = Vec2(0, -1);
    break;
  case EventKeyboard::KeyCode::KEY_A:
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    playerDirection = Vec2(-1, 0);
    break;
  case EventKeyboard::KeyCode::KEY_D:
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    playerDirection = Vec2(1, 0);
    break;
  };
}

/* callback function when a key is released */
void GameMap::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *_) {
  playerDirection = Vec2(0, 0);
}
