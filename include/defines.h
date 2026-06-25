#ifndef DEFINES_H
#define DEFINES_H

#include "structs.h"

State current = GAME_INTRO;
State next_state;

double fade_rate = 0.0f;

#define FADE_IN 0
#define FADE_OUT 1
#define MENU_FONT_SIZE 40
#define SCALE 4.0f

#endif