#pragma once
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include <string>

using namespace CocosDenshion;
using namespace cocos2d;

class Music {
public:
  Music();

  // Music(const char* musicFile, const char* soundFile);

  bool isMusicOn{true};
  bool isSoundOn{true};
  float musicVolume = 0.5;
  float soundVolume = 0.5;
  void changeMusicWorkState(bool isMusicOn);
  /**
    @brief    change music's work state

   @return void
   */
  void changeSoundWorkState(bool isSoundOn);
  /**
    @brief    change sound's work state

   @return void
   */
  void changeMusicVolume(float volume);
  /**
    @brief    change music's volume

   @return void
   */
  void changeSoundVolume(float volume);
  /**
    @brief    change sound's volume

   @return void
   */
  SimpleAudioEngine *music = SimpleAudioEngine::getInstance();
  SimpleAudioEngine *sound = SimpleAudioEngine::getInstance();
};