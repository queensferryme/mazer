#pragma once

#include "cocos2d.h"

using namespace cocos2d;

/**
@brief   Start-up menu scene
*/
class GameMap : public Scene {
private:
  Action *playerAnimation, *enemyAnimation;
  bool isPlayerDandgerous, isPlayerNearSuccess;
  cocos2d::TMXTiledMap *map;
  int collectedKeys, playerScore;
  Sprite *player;
  Vec2 playerDirection;
  Vector<Sprite *> *enemies, *followingEnemies, *triggers;

public:
  /**
  @brief    Create scene with `static create()`
  @return   pointer to a cocos2d::Scene object
  */
  static Scene *createScene() { return GameMap::create(); }

  /**
  @brief    Implementation of `static create()` function,
            used in createScene()
  @return   pointer to an object of current class
  */
  CREATE_FUNC(GameMap);

  /**
  @brief   Initialize current cocos2d::Scene object
  @return   status of creation:
            * true -> success
            * false -> failure
  */
  virtual bool init() override;

  /**
  @brief    Register an update function for cocos2d
            which performs scheduled checking tasks
  @param    float, update interval
  @return   void
  */
  virtual void update(float) override;

  /**
  @brief    Create a repeated action for cruising
  @param    std::string, a descriptive pattern for cruising
  @return   Action *, a pointer to the repeated action
  */
  Action *createCruiseAction(Sprite *, const std::string &);

  /**
  @brief    Create an action for enemy auto following
  @param    Sprite *, enemy sprite
  @param    Sprite *, player sprite
  @return   Action *, a pointer to the following action
  */
  Action *createFollowAction(Sprite *, Sprite *);

  /**
  @brief    Load an animation for player/enemy movement
  @param    char *, name of animation to be loaded
  @return   void
  */
  Action *loadSpriteAnimation(const char *);

  /**
  @brief    Return tile GID for a position in a map layer
  @param    std::string, map layer name
  @param    Vec2, specific position
  @return   int, tile GID
  */
  int getTileGIDForPositionInLayer(const std::string &, const Vec2 &);

  /**
  @brief    Return a property value of the specific tile
  @param    int, tileGID
  @param    std::string, property name
  @return   property value
  */
  Value getPropertyForTileGID(const int, const std::string &);

  /**'
  @brief    Return tile coordinate for a specific position
  @param    Vec2, a specific position
  @return   void
  */
  Vec2 getTileCoordForPosition(const Vec2 &);

  /*
  @brief    Check player's status: normal/danger/dead
  @return   void
  */
  void checkPlayerStatus();

  /**
  @brief    Set view point center to current player
  @return   void
  */
  void focusSceneOnPlayer();

  /**
  @brief    Load sprite frame animations
  @return   void
  */
  void loadSpriteAnimations();

  /**
  @brief    Callback function when a key is pressed
  @param    EventKeyboard::Keycode, code of pressed key
  @param    Event *, a pointer to event object
  @return   void
  */
  void onKeyPressed(EventKeyboard::KeyCode, Event *);

  /**
  @brief    Callback function when a key is released
  @param    EventKeyboard::Keycode, code of released key
  @param    Event *, a pointer to event object
  @return   void
  */
  void onKeyReleased(EventKeyboard::KeyCode, Event *);

  /**
  @brief    Handler function which is invoked when player dies
  @return   void
  */
  void onPlayerDead();

  /**
  @brief    Handler function which is invoked when player
            collected all four key items
  @return   void
  */
  void onPlayerNearSuccess();

  /**
  @brief    Handler function which is invoked when player succeed
  @return   void
  */
  void onPlayerSuccess();

  /**
  @brief    Create collidable tiles when triggered
  @param    std::string, information of tiles to be created
  @return   void
  */
  void triggerWithPattern(const std::string &);
};
