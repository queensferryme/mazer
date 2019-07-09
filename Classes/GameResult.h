#pragma once

#include <string>

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::ui;

/**
@brief   About scene
*/
class GameResult : public Scene {
public:
  std::string playerScore;
  TextField *textField;

  /**
  @brief    Create scene with `static create()`
  @param    bool, whether the player succeeded or failed in the game
  @param    int, player's score
  @return   pointer to a cocos2d::Scene object
  */
  static Scene *createScene(bool, int);

  /**
  @brief    Implementation of `static create()` function,
            used in createScene()
  @return   pointer to an object of current class
  */
  CREATE_FUNC(GameResult);

  /**
  @brief    Initialize current cocos2d::Scene object
  @return   status of creation:
            * true -> success
            * false -> failure
  */
  virtual bool init();
};
