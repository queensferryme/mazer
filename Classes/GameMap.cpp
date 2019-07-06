#include <cmath>
#include <iostream>

#include "GameMap.h"

using namespace cocos2d;

/* helper function, return distance between two dots */
double distance(const Vec2 &p1, const Vec2 &p2) {
  return sqrt(pow(p1.x - p2.x, 2.0) + pow(p1.y - p2.y, 2.0));
}

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
  auto enemyData = map->getObjectGroup("Enemy")->getObjects();
  this->enemies = new Vector<Sprite *>;
  for (auto enemyDatum : enemyData) {
    Sprite *enemy = Sprite::create("player.png");
    ValueMap enemyValueMap = enemyDatum.asValueMap();
    enemy->setName(enemyValueMap["Name"].asString());
    enemy->setPosition(
        Vec2(enemyValueMap["x"].asFloat(), enemyValueMap["y"].asFloat()));
    enemy->setUserData(
        new Vec2(enemyValueMap["Dead"].asFloat() * map->getTileSize().width,
                 enemyValueMap["Danger"].asFloat() * map->getTileSize().width));
    enemy->runAction(createRepeatAction(enemyValueMap["Pattern"].asString()));
    this->addChild(enemy);
    this->enemies->pushBack(enemy);
  }
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
  Vec2 position = player->getPosition() + (3 * playerDirection);
  // don't update if next tile is collidable
  int tileGID = getTileGIDForPositionInLayer("Background", position);
  if (!map->getPropertiesForGID(tileGID).isNull()) {
    auto prop = map->getPropertiesForGID(tileGID).asValueMap();
    if (prop["isCollidable"].asString() == "true")
      return;
  }
  player->setPosition(position);
  this->checkPlayerStatus();
  this->focusSceneOnPlayer();
  // collect map items if it's collectable
  tileGID = getTileGIDForPositionInLayer("Collect", position);
  if (!map->getPropertiesForGID(tileGID).isNull()) {
    auto prop = map->getPropertiesForGID(tileGID).asValueMap();
    if (prop["isCollectable"].asString() == "true")
      map->getLayer("Collect")->removeTileAt(getTileCoordForPosition(position));
  }
}

/* create a repeated action for cruising */
Action *GameMap::createRepeatAction(const std::string &pattern) {
  Vector<FiniteTimeAction *> actions;
  for (int i = 0, moveStep; i < pattern.length(); i += 2) {
    moveStep = pattern[i + 1] - '0';
    switch (pattern[i]) {
    case 'U':
      actions.pushBack(MoveBy::create(
          moveStep * 0.5, moveStep * map->getTileSize().width * Vec2(0, 1)));
      break;
    case 'D':
      actions.pushBack(MoveBy::create(
          moveStep * 0.5, moveStep * map->getTileSize().width * Vec2(0, -1)));
      break;
    case 'L':
      actions.pushBack(MoveBy::create(
          moveStep * 0.5, moveStep * map->getTileSize().width * Vec2(-1, 0)));
      break;
    case 'R':
      actions.pushBack(MoveBy::create(
          moveStep * 0.5, moveStep * map->getTileSize().width * Vec2(1, 0)));
      break;
    }
  }
  return RepeatForever::create(Sequence::create(actions));
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

/* check player's status: normal/danger/dead */
void GameMap::checkPlayerStatus() {
  auto playerPosition{player->getPosition()};
  // check if dead
  for (auto enemy : *enemies)
    if (distance(playerPosition, enemy->getPosition()) <=
        reinterpret_cast<Vec2 *>(enemy->getUserData())->x) {
      log("Dead");
      exit(0);
    }
  // check if danger
  for (auto enemy : *enemies)
    if (distance(playerPosition, enemy->getPosition()) <=
        reinterpret_cast<Vec2 *>(enemy->getUserData())->y) {
          log("Danger");
          return;
        }
  log("Normal");
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
