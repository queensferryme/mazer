#include "Config.h"
#include "Music.h"

/* play background music when initialized */
void playBackgroundMusic(const std::string &file) {
  Config::music->stopBackgroundMusic();
  if (Config::isMusicOn) {
    Config::music->playBackgroundMusic(file.c_str(), true);
  }
}

/* helper functions, make a click sound */
void playSoundEffect(const std::string &file) {
  if (Config::isSoundOn)
    Config::sound->play2d(file, false, Config::soundVolume);
}

/* update background music when toggled */
void updateBackgroundMusic() {
  if (Config::isMusicOn)
    Config::music->rewindBackgroundMusic();
  else
    Config::music->pauseBackgroundMusic();
}
