#include <cmath>
#include <sstream>

#include "Config.h"
#include "GameMap.h"
#include "GameResult.h"
#include "Music.h"

using namespace cocos2d;

/* helper function, return distance between two dots */
double _distance(const Vec2 &p1, const Vec2 &p2) {
  return sqrt(pow(p1.x - p2.x, 2.0) + pow(p1.y - p2.y, 2.0));
}

/* check player's status: normal/danger/dead */
void GameMap::checkPlayerStatus() {
  auto playerPosition{player->getPosition()};
  if (collectedKeys == 4 && !isPlayerNearSuccess)
    onPlayerNearSuccess();
  // if player is dead, exit game
  for (auto enemy : *enemies)
    if (_distance(playerPosition, enemy->getPosition()) <=
        reinterpret_cast<Vec2 *>(enemy->getUserData())->x) {
      onPlayerDead();
    }
  // if player is in danger, prompt & set enemy auto follow
  bool _isPlayerDandgerous{false};
  for (auto enemy : *enemies)
    if (_distance(playerPosition, enemy->getPosition()) <=
        reinterpret_cast<Vec2 *>(enemy->getUserData())->y) {
      _isPlayerDandgerous = true;
      if (!isPlayerDandgerous) {
        playSoundEffect("audio/danger.mp3");
        this->playerScore -= 2;
      }
      if (!this->followingEnemies->contains(enemy)) {
        enemy->stopAllActions();
        this->followingEnemies->pushBack(enemy);
      }
    }
  isPlayerDandgerous = _isPlayerDandgerous;
}

/* create a repeated action for cruising */
Action *GameMap::createCruiseAction(Sprite *enemy, const std::string &pattern) {
  Vector<FiniteTimeAction *> actions;
  for (int i = 0; i < pattern.length(); i += 2) {
    // determine enemy move direction
    Vec2 enemyDirection;
    if (pattern[i] == 'U')
      enemyDirection = Vec2(0, 1);
    else if (pattern[i] == 'D')
      enemyDirection = Vec2(0, -1);
    else if (pattern[i] == 'L') {
      enemyDirection = Vec2(-1, 0);
      actions.pushBack(CallFunc::create([=]() { enemy->setFlippedX(false); }));
    } else if (pattern[i] == 'R') {
      enemyDirection = Vec2(1, 0);
      actions.pushBack(CallFunc::create([=]() { enemy->setFlippedX(true); }));
    }
    // determine enemy move length
    for (int j = 0, moveStep = pattern[i + 1] - '0'; j < moveStep; j++) {
      actions.pushBack(
          MoveBy::create(.5 / Config::speedUpFactor,
                         map->getTileSize().width * enemyDirection));
      actions.pushBack(DelayTime::create(.25 / Config::speedUpFactor));
    }
  }
  return RepeatForever::create(Sequence::create(actions));
}

/* create an action for enemy auto following */
Action *GameMap::createFollowAction(Sprite *enemy, Sprite *player) {
  Vec2 enemyPosition{enemy->getPosition()},
      playerPosition{player->getPosition()},
      vectors[5]{Vec2(0, 0), Vec2(0, 1) * map->getTileSize().width,
                 Vec2(0, -1) * map->getTileSize().width,
                 Vec2(-1, 0) * map->getTileSize().width,
                 Vec2(1, 0) * map->getTileSize().width};
  // find the moving direction which minimize the distance
  int minimumIndex{0}, tileGID;
  double distanceValue, minimumValue{0xfffffffffLL};
  for (int i{0}; i < 5; i++) {
    Vec2 enemyNextPosition{enemyPosition + vectors[i]};
    tileGID = getTileGIDForPositionInLayer("Background", enemyNextPosition);
    distanceValue = _distance(enemyNextPosition, playerPosition);
    if (distanceValue < minimumValue &&
        (getPropertyForTileGID(tileGID, "isCollidable").asString() != "true" ||
         isPlayerNearSuccess)) {
      minimumIndex = i;
      minimumValue = distanceValue;
    }
  }
  if (minimumIndex == 3)
    enemy->setFlippedX(false);
  else if (minimumIndex == 4)
    enemy->setFlippedX(true);
  return Sequence::create(
      MoveBy::create(.5 / Config::speedUpFactor, vectors[minimumIndex]),
      DelayTime::create(.25 / Config::speedUpFactor), nullptr);
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

/* return a property value of the specific tile */
Value GameMap::getPropertyForTileGID(const int tileGID,
                                     const std::string &propertyName) {
  if (!map->getPropertiesForGID(tileGID).isNull())
    return map->getPropertiesForGID(tileGID).asValueMap()[propertyName];
  return Value();
}

/* return tile coordinate for a specific position */
Vec2 GameMap::getTileCoordForPosition(const Vec2 &position) {
  float x{position.x / map->getTileSize().width},
      y{(map->getMapSize().height * map->getTileSize().height - position.y) /
        map->getTileSize().height};
  return Vec2(static_cast<int>(x), static_cast<int>(y));
}

/* return tile GID for a position in a map layer */
int GameMap::getTileGIDForPositionInLayer(const std::string &layerName,
                                          const Vec2 &position) {
  return map->getLayer(layerName)->getTileGIDAt(
      getTileCoordForPosition(position));
}

/* initialize GameMap Scene object */
bool GameMap::init() {
  if (!Scene::init())
    return false;
  this->loadSpriteAnimations();
  playBackgroundMusic("audio/battle.mp3");
  this->isPlayerDandgerous = false;
  this->collectedKeys = this->playerScore = 0;
  // initialize tiled map
  auto map = cocos2d::TMXTiledMap::create("map/map.tmx");
  this->addChild(map);
  this->map = map;
  // initialize player sprite
  auto spawnPoint = map->getObjectGroup("Player")->getObject("SpawnPoint");
  auto player = Sprite::create();
  player->runAction(playerAnimation);
  player->setScale(.22, .22);
  player->setAnchorPoint(Vec2(.5, .4));
  player->setPosition(
      Vec2(spawnPoint["x"].asFloat(), spawnPoint["y"].asFloat()));
  this->addChild(player);
  this->player = player;
  this->focusSceneOnPlayer();
  this->isPlayerNearSuccess = false;
  // initialize enemy sprites
  auto enemyData = map->getObjectGroup("Enemy")->getObjects();
  this->enemies = new Vector<Sprite *>;
  this->followingEnemies = new Vector<Sprite *>;
  for (auto enemyDatum : enemyData) {
    Sprite *enemy = Sprite::create();
    enemy->runAction(enemyAnimation->clone());
    enemy->setScale(.2, .2);
    enemy->setAnchorPoint(Vec2(.5, .4));
    ValueMap enemyValueMap = enemyDatum.asValueMap();
    enemy->setPosition(
        Vec2(enemyValueMap["x"].asFloat(), enemyValueMap["y"].asFloat()));
    enemy->setUserData(
        new Vec2(enemyValueMap["Dead"].asFloat() * map->getTileSize().width,
                 enemyValueMap["Danger"].asFloat() * map->getTileSize().width));
    enemy->runAction(
        createCruiseAction(enemy, enemyValueMap["Pattern"].asString()));
    this->addChild(enemy);
    this->enemies->pushBack(enemy);
  }
  // initialize trigger sprites
  auto triggerData = map->getObjectGroup("Trigger")->getObjects();
  this->triggers = new Vector<Sprite *>;
  for (auto triggerDatum : triggerData) {
    Sprite *trigger = Sprite::create("img/trigger.png");
    trigger->setScale(.1, .1);
    ValueMap triggerValueMap = triggerDatum.asValueMap();
    trigger->setPosition(
        Vec2(triggerValueMap["x"].asFloat(), triggerValueMap["y"].asFloat()));
    trigger->setUserData(
        new std::string(triggerValueMap["Pattern"].asString()));
    this->addChild(trigger);
    this->triggers->pushBack(trigger);
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

/* load an animation for player/enemy movement */
Action *GameMap::loadSpriteAnimation(const char *animationName) {
  char frameName[100];
  SpriteFrame *frame;
  Vector<SpriteFrame *> frames;
  for (int i{1}; i <= 8; i++) {
    sprintf(frameName, "%s%d.png", animationName, i);
    frames.pushBack(
        SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
  }
  auto animation = Animation::createWithSpriteFrames(frames, 0.1f);
  animation->setLoops(-1);
  return Animate::create(animation);
}

/* load sprite frame animations */
void GameMap::loadSpriteAnimations() {
  SpriteFrameCache::getInstance()->addSpriteFramesWithFile("img/player.plist");
  SpriteFrameCache::getInstance()->addSpriteFramesWithFile("img/enemy.plist");
  this->playerAnimation = loadSpriteAnimation("player");
  this->enemyAnimation = loadSpriteAnimation("enemy");
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
    player->setFlippedX(false);
    break;
  case EventKeyboard::KeyCode::KEY_D:
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    playerDirection = Vec2(1, 0);
    player->setFlippedX(true);
    break;
  };
}

/* callback function when a key is released */
void GameMap::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *_) {
  playerDirection = Vec2(0, 0);
}

/* handler function which is invoked when player dies */
void GameMap::onPlayerDead() {
  Director::getInstance()->replaceScene(
      GameResult::createScene(false, playerScore));
}

/* handler function which is invoked when player collected all four key items */
void GameMap::onPlayerNearSuccess() {
  playBackgroundMusic("audio/endgame.mp3");
  isPlayerNearSuccess = true;
}

/* handler function which is invoked when player succeed */
void GameMap::onPlayerSuccess() {
  Director::getInstance()->replaceScene(
      GameResult::createScene(true, playerScore));
}

/* create collidable tiles when triggered */
void GameMap::triggerWithPattern(const std::string &pattern) {
  std::stringstream stream{pattern};
  int tileCoordX, tileCoordY, tileGID;
  while (true) {
    stream >> tileCoordX >> tileCoordY >> tileGID;
    if (stream.fail())
      break;
    map->getLayer("Background")
        ->setTileGID(tileGID, Vec2(tileCoordX, tileCoordY));
  }
}

/* register an update function for cocos2d
   which performs scheduled checking tasks */
void GameMap::update(float delta) {
  Node::update(delta);
  // check player's status: normal/danger/dead
  this->checkPlayerStatus();
  // if user successfully passes the game
  auto spawnPoint = map->getObjectGroup("Player")->getObject("SpawnPoint");
  if (isPlayerNearSuccess &&
      _distance(player->getPosition(),
                Vec2(spawnPoint["x"].asFloat(), spawnPoint["y"].asFloat())) <
          map->getTileSize().width)
    onPlayerSuccess();
  // if in danger zone, enemies will auto follow
  for (auto enemy : *followingEnemies)
    if (!enemy->getNumberOfRunningActions())
      enemy->runAction(createFollowAction(enemy, player));
  // calculate current player's next position
  // TODO: speed up factor varies with difficulty
  if (playerDirection.x == 0 && playerDirection.y == 0)
    return;
  Vec2 position = player->getPosition() + (3 * playerDirection);
  // don't update if next tile is collidable
  int tileGID = getTileGIDForPositionInLayer("Background", position);
  if (getPropertyForTileGID(tileGID, "isCollidable").asString() == "true")
    return;
  player->setPosition(position);
  this->focusSceneOnPlayer();
  // collect map items if it's collectable
  tileGID = getTileGIDForPositionInLayer("Collect", position);
  if (getPropertyForTileGID(tileGID, "isCollectable").asString() == "true") {
    map->getLayer("Collect")->removeTileAt(getTileCoordForPosition(position));
    playSoundEffect("audio/collectItem.mp3");
    if (getPropertyForTileGID(tileGID, "isKey").asString() == "true")
      this->collectedKeys++;
    else
      this->playerScore += 10;
  }
  // create new collidable tiles if triggered
  for (auto trigger : *triggers)
    if (_distance(player->getPosition(), trigger->getPosition()) <=
        map->getTileSize().width) {
      playSoundEffect("audio/trigger.mp3");
      triggerWithPattern(
          *reinterpret_cast<std::string *>(trigger->getUserData()));
      triggers->erase(triggers->find(trigger));
      this->removeChild(trigger);
    }
}
