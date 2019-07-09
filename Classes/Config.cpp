#include "Config.h"

using namespace CocosDenshion;
using namespace cocos2d;

/* default configuration */
SimpleAudioEngine *Config::music = SimpleAudioEngine::getInstance();
bool Config::isMusicOn = true;
bool Config::isSoundOn = true;
float Config::musicVolume = .5;
float Config::soundVolume = .5;
float Config::speedUpFactor = 1;
std::string Config::dbFilePath = FileUtils::getInstance()->getWritablePath() + "rank.db";

Config::Config() = default;

/* toggle music state */
void Config::toggleMusicState() {
  isMusicOn = !isMusicOn;
  isMusicOn ? music->resumeBackgroundMusic() : music->pauseBackgroundMusic();
}

/* toggle sound state */
void Config::toggleSoundState() { isSoundOn = !isSoundOn; }

/* set music volume */
void Config::setMusicVolume(float volume) { musicVolume = volume; }

/* set sound volume */
void Config::setSoundVolume(float volume) { soundVolume = volume; }
