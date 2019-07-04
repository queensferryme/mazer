#include "Config.h"

using namespace CocosDenshion;
using namespace cocos2d;

/* default configuration */
SimpleAudioEngine *Config::music = SimpleAudioEngine::getInstance();
bool Config::isMusicOn = true;
bool Config::isSoundOn = true;
float Config::musicVolume = .5;
float Config::soundVolume = .5;

/* helper functions */
void makeClickSound() {
  if (Config::isSoundOn) {
    SimpleAudioEngine::getInstance()->setEffectsVolume(Config::soundVolume);
    Config::sound->play2d("click.wav", false, Config::soundVolume);
  }
}

Config::Config() = default;

/* toggle music state */
void Config::toggleMusicState(bool isMusicOn_) {
  isMusicOn = isMusicOn_;
  isMusicOn ? music->resumeBackgroundMusic() : music->pauseBackgroundMusic();
}

/* toggle sound state */
void Config::toggleSoundState(bool isSoundOn_) {
  isSoundOn = isSoundOn_;
  isSoundOn ? sound->resumeAll() : sound->pauseAll();
}

/* set music volume */
void Config::setMusicVolume(float volume) { musicVolume = volume; }

/* set sound volume */
void Config::setSoundVolume(float volume) { soundVolume = volume; }
