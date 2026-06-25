#include <stdio.h>

#include "raylib.h"
#include "../include/defines.h"
#include "../include/screen_effects_f.h"


int fade_screen(int io)
{

	if (io == FADE_IN)
	{

		fade_rate -= 200 * GetFrameTime();
		if (fade_rate <= 0)
		{
			fade_rate = 0;
			return 0; // OVER
		}
	}

	else
	{
		fade_rate += 200 * GetFrameTime();

		if (fade_rate >= 255)
		{
			fade_rate = 255;
			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, fade_rate / 255.0f));
			return 0; // OVER
		}
	}
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, fade_rate / 255.0f));
	return 1; // NOT YET FINISHED
}

void draw_menu_background(Texture2D b1, Texture2D b2, Texture2D b3, double *wall_changing, int *wall_turn)
{
	*wall_changing += GetFrameTime();

	if (*wall_changing >= 3.0f)
	{
		*wall_changing = 0.0f;

		if (*wall_turn == 3)
			*wall_turn = 1;
		else
			(*wall_turn)++;
	}

	switch (*wall_turn)
	{
	case 1:
		DrawTexture(b1, 0, 0, WHITE);
		break;

	case 2:
		DrawTexture(b2, 0, 0, WHITE);
		break;

	case 3:
		DrawTexture(b3, 0, 0, WHITE);
		break;
	}
}
