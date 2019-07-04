#include <unistd.h>

#include "Music.h"
#include "Config.h"

/* helper functions, make a click sound */
void playSoundEffect(const std::string &file) {
  if (Config::isSoundOn)
    Config::sound->play2d(file, false, Config::soundVolume);
}

/* update background music when toggled */
void updateBackgroundMusic() {
  if (!Config::music->willPlayBackgroundMusic())
    Config::music->playBackgroundMusic("Honor.mp3", true);
  if (Config::isMusicOn)
    Config::music->rewindBackgroundMusic();
  else
    Config::music->pauseBackgroundMusic();
}
