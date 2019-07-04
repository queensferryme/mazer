#pragma once

#include <string>

#include "AudioEngine.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

using namespace CocosDenshion;
using namespace cocos2d;
using namespace cocos2d ::experimental;

class Config {
public:
  static AudioEngine *sound;
  static SimpleAudioEngine *music;
  static bool isMusicOn;
  static bool isSoundOn;
  static float musicVolume;
  static float soundVolume;

  Config();

  /**
  @brief    Toggle music state
  @return   void
  */
  static void toggleMusicState();

  /**
  @brief    Toggle sound state
  @return   void
  */
  static void toggleSoundState();

  /**
  @brief    Set music volume
  @return   void
  */
  static void setMusicVolume(float volume);

  /**
  @brief    Set sound volume
  @return   void
  */
  static void setSoundVolume(float volume);
};
