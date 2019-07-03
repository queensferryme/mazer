#pragma once

#include "cocos2d.h"

using namespace cocos2d;

/**
@brief   Start-up menu scene
*/
class GameMap : public Scene {
private:
  Sprite *player;
  TMXTiledMap *map;
  Vec2 playerDirection;

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
  @brief    Set view point center to current player
  @return   void
  */
  void focusSceneOnPlayer();
};
