#pragma once

#include "Music.h"
#include "cocos2d.h"

using namespace cocos2d;

/**
@brief   About scene
*/
class About : public cocos2d::Scene {
public:
  /**
  @brief    Create scene with `static create()`
  @return   pointer to a cocos2d::Scene object
  */
  static cocos2d::Scene *createScene() { return About::create(); }

  /**
  @brief    Implementation of `static create()` function,
            used in createScene()
  @return   pointer to an object of current class
  */
  CREATE_FUNC(About);

  /**
  @brief    Initialize current cocos2d::Scene object
  @return   status of creation:
            * true -> success
            * false -> failure
  */
  virtual bool init();
};
