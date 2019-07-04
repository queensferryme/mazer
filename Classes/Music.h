#pragma once

#include <string>

/* play background music when initialized */
void playBackgroundMusic();

/* helper functions, make a click sound */
void playSoundEffect(const std::string &);

/* update background music when toggled */
void updateBackgroundMusic();
