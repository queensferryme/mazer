#include "Music.h"

using namespace CocosDenshion;
using namespace cocos2d;

Music::Music() = default;

void Music::changeMusicWorkState(bool isMusicOn_) {
  isMusicOn = isMusicOn_;
  isMusicOn ? music->resumeBackgroundMusic() : music->pauseBackgroundMusic();
}
/**
  @brief    change music's work state

 @return void
 */
void Music::changeSoundWorkState(bool isSoundOn_) {
  isSoundOn = isSoundOn_;
  isSoundOn ? sound->resumeAll() : sound->pauseAll();
}
/**
  @brief    change sound's work state

 @return void
 */
void Music::changeMusicVolume(float volume) { musicVolume = volume; }
/**
  @brief    change music's volume

 @return void
 */
void Music::changeSoundVolume(float volume) { soundVolume = volume; }
