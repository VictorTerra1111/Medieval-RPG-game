#ifndef SCREEN_EFFECTS_H
#define SCREEN_EFFECTS_H

#include "raylib.h"
#include "defines.h"

int fade_screen(int io);

void draw_menu_background(Texture2D b1, Texture2D b2, Texture2D b3, double *wall_changing, int *wall_turn);

#endif
