#pragma once

#include "cocos2d.h"

using namespace cocos2d;

/**
@brief   Start-up menu scene
*/
class GameMap : public Scene {
private:
  Sprite *player;
  cocos2d::TMXTiledMap *map;
  Vec2 playerDirection;
  Vector<Sprite *> *enemies;

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
  Action *createRepeatAction(const std::string &);

  /**
  @brief    Return tile GID for a position in a map layer
  @param    std::string, map layer name
  @param    Vec2, specific position
  @return   int, tile GID
  */
  int getTileGIDForPositionInLayer(const std::string &, const Vec2 &);

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
};
