#include <stdio.h>
#include <string.h>

#include "raylib.h"

#include "../include/defines.h"
#include "../include/player.h"
#include "../include/save_file_f.h"
#include "../include/screen_effects_f.h"
#include "../include/structs.h"

#define FADE_IN 0
#define FADE_OUT 1
#define MENU_FONT_SIZE 40
#define SCALE 4.0f

double fade_rate = 0.0f;

State current = GAME_INTRO;

int main(void)
{
	SetTraceLogLevel(LOG_NONE);
	InitWindow(500, 500, "Corpsia");
	ToggleFullscreen();
	SetTargetFPS(60);

	Texture2D logo = LoadTexture("assets/sprites/corpsia_logo_black.png");
	Texture2D logo2 = LoadTexture("assets/sprites/corpsia_logo_white.png");
	Texture2D background_intro = LoadTexture("assets/sprites/Background_intro.png");

	Texture2D b_menu1 = LoadTexture("assets/sprites/background_menu_pic1.png");
	Texture2D b_menu2 = LoadTexture("assets/sprites/background_menu_pic2.png");
	Texture2D b_menu3 = LoadTexture("assets/sprites/background_menu_pic3.png");

	bool fade_o = false, fade_i = true, running = true;

	double wall_changing = 0.0f;
	int wall_turn = 1, sel2 = 0, sel = 0, sel3 = 0, sel_slot = 1;

	const char *sure = "ARE YOU SURE?";
	const char *no = "NO";
	const char *yes = "YES";
	const char *start = "START";
	const char *start_op = "SAVE FILES";
	const char *options_op = "OPTIONS";
	const char *exit_op = "EXIT";
	const char *go_back = "BACK TO START";

	char slot1_text[32] = "EMPTY SLOT";
	char slot2_text[32] = "EMPTY SLOT";
	char slot3_text[32] = "EMPTY SLOT";

	int width = logo.width * SCALE;
	int height = logo.height * SCALE;
	int sure_wid = MeasureText(sure, MENU_FONT_SIZE);
	int no_wid = MeasureText(no, MENU_FONT_SIZE);
	int yes_wid = MeasureText(yes, MENU_FONT_SIZE);
	int t_wid = MeasureText(start, MENU_FONT_SIZE);
	int sta_wid = MeasureText(start_op, MENU_FONT_SIZE);
	int opt_wid = MeasureText(options_op, MENU_FONT_SIZE);
	int exi_wid = MeasureText(exit_op, MENU_FONT_SIZE);
	int gob_wid = MeasureText(go_back, MENU_FONT_SIZE);

	int x_gob = (GetScreenWidth() - gob_wid) / 2;
	int x_sta = (GetScreenWidth() - sta_wid) / 2;
	int x_opt = (GetScreenWidth() - opt_wid) / 2;
	int x_exi = (GetScreenWidth() - exi_wid) / 2;
	int x_text = (GetScreenWidth() - t_wid) / 2;
	int x = (GetScreenWidth() - width) / 2;
	int y = (GetScreenHeight() - height) / 2;
	int x_sure = (GetScreenWidth() - sure_wid) / 2;
	int x_no = (GetScreenWidth() - no_wid) / 2;
	int x_yes = (GetScreenWidth() - yes_wid) / 2;

	MainChar p1;

	snprintf(p1.name, sizeof(p1.name), "%s", "Renna legot");
	
	while (!WindowShouldClose() && running)
	{ // current state of the game looping
		BeginDrawing();
		ClearBackground(BLACK);

		switch (current)
		{
		case GAME_INTRO: // intro screen with logo and enter only
			DrawTexture(background_intro, 0, 0, WHITE);

			DrawTextureEx(logo, (Vector2){x, y}, 0, SCALE, WHITE);

			DrawText(start, x_text, 960, 40, BLACK);

			if (IsKeyPressed(KEY_ENTER))
			{
				fade_o = true;
			}
			if (fade_o)
			{
				DrawTextureEx(logo2, (Vector2){x, y}, 0, SCALE, WHITE);
				if (!fade_screen(FADE_OUT))
				{
					current = GAME_MENU;
					fade_o = false;
					fade_i = true;
					fade_rate = 255;
				}
			}
			break;

		case GAME_MENU:
			draw_menu_background(b_menu1, b_menu2, b_menu3, &wall_changing, &wall_turn);

			DrawText(start_op, x_sta, 300, 40, BLACK);
			DrawText(options_op, x_opt, 400, 40, BLACK);
			DrawText(exit_op, x_exi, 500, 40, BLACK);

			if (fade_i)
				if (!fade_screen(FADE_IN))
					fade_i = false;

			if (IsKeyPressed(KEY_S))
			{
				if (sel < 2)
					sel++;
				else
					sel = 0;
			}
			else if (IsKeyPressed(KEY_W))
			{
				if (sel == 0)
					sel = 2;
				else
					sel--;
			}
			switch (sel)
			{
			case 0:
				DrawText(">", x_sta - 100, 300, 40, BLACK);
				break;
			case 1:
				DrawText(">", x_opt - 100, 400, 40, BLACK);
				break;
			case 2:
				DrawText(">", x_exi - 100, 500, 40, BLACK);
				break;
			}

			if (IsKeyPressed(KEY_ENTER))
			{
				switch (sel)
				{
				case 0: // start
					current = GAME_SAVES;
					break;

				case 1: // options
					current = GAME_OPTIONS;
					break;

				case 2: // out
					current = GAME_EXIT;
					sel2 = 0;
					break;
				}
			}
			break;

		case GAME_SAVES:
			ClearBackground(WHITE);

			if (save_exists(1))
			{
				p1 = load_save_file(1);
				snprintf(slot1_text, sizeof(slot1_text), "%s", p1.name);
			}
			if (save_exists(2))
			{
				p1 = load_save_file(2);
				snprintf(slot2_text, sizeof(slot2_text), "%s", p1.name);
			}

			if (save_exists(3))
			{
				p1 = load_save_file(3);
				snprintf(slot3_text, sizeof(slot3_text), "%s", p1.name);
			}


			int wid1 = MeasureText(slot1_text, MENU_FONT_SIZE);
			int wid2 = MeasureText(slot2_text, MENU_FONT_SIZE);
			int wid3 = MeasureText(slot3_text, MENU_FONT_SIZE);
			int x_s1 = (GetScreenWidth() - wid1) / 2;
			int x_s2 = (GetScreenWidth() - wid2) / 2;
			int x_s3 = (GetScreenWidth() - wid3) / 2;

			DrawText(slot1_text, x_s1, 300, 40, BLACK);
			DrawText(slot2_text, x_s2, 400, 40, BLACK);
			DrawText(slot3_text, x_s3, 500, 40, BLACK);
			DrawText(go_back, x_gob, 800, 40, BLACK);

			if (IsKeyPressed(KEY_S))
			{
				if (sel3 < 3)
					sel3++;
				else
					sel3 = 0;
			}
			else if (IsKeyPressed(KEY_W))
			{
				if (sel3 == 0)
					sel3 = 3;
				else
					sel3--;
			}
			switch (sel3)
			{
			case 0:
				DrawText(">", x_s1 - 100, 300, 40, BLACK);

				if (IsKeyPressed(KEY_ENTER))
				{
					sel_slot = 1;
					if (create_save_file(1, &p1) == -2)
						current = GAME_PLAYING;
				}
				else if (IsKeyPressed(KEY_DELETE))
				{
					sel_slot = 1;
					current = GAME_SAVE_ALREADY_EXIST;
				}

				break;
			case 1:
				DrawText(">", x_s2 - 100, 400, 40, BLACK);
				if (IsKeyPressed(KEY_ENTER))
				{
					sel_slot = 2;
					if (create_save_file(2, &p1) == -2)
						current = GAME_PLAYING;
				}
				else if (IsKeyPressed(KEY_DELETE))
				{
					sel_slot = 2;
					current = GAME_SAVE_ALREADY_EXIST;
				}

				break;
			case 2:

				DrawText(">", x_s3 - 100, 500, 40, BLACK);
				if (IsKeyPressed(KEY_ENTER))
				{
					sel_slot = 3;
					if (create_save_file(3, &p1) == -2)
						current = GAME_PLAYING;
				}
				else if (IsKeyPressed(KEY_DELETE))
				{
					sel_slot = 3;
					sel2 = 0;
					current = GAME_SAVE_ALREADY_EXIST;
				}

				break;
			case 3:
				DrawText(">", x_gob - 100, 800, 40, BLACK);
				if (IsKeyPressed(KEY_ENTER))
				{
					sel = 0;
					current = GAME_MENU;
				}
				break;
			}

			break;

		case GAME_SAVE_ALREADY_EXIST:
			ClearBackground(WHITE);

			if (IsKeyPressed(KEY_D))
			{
				if (sel2 < 1)
					sel2++;
				else
					sel2 = 0;
			}
			else if (IsKeyPressed(KEY_A))
			{
				if (sel2 == 0)
					sel2 = 1;
				else
					sel2--;
			}

			DrawText(sure, x_sure, 400, 40, BLACK);
			DrawText(no, x_no + 100, 600, 40, BLACK);
			DrawText(yes, x_yes - 100, 600, 40, BLACK);

			if (!sel2)
			{
				DrawText(">", x_yes - 125, 600, 40, BLACK);
				if (IsKeyPressed(KEY_ENTER))
				{
					delete_save_file(sel_slot);
				}
			}
			else
			{
				DrawText(">", x_no + 75, 600, 40, BLACK);
				if (IsKeyPressed(KEY_ENTER))
					current = GAME_SAVES;
			}

			break;

		case GAME_PLAYING:
			ClearBackground(BLACK);
			break;

		case GAME_OPTIONS:
			ClearBackground(WHITE);
			DrawText("There are no options yet, so go play the game", 0, 0, 40, BLACK);
			if (IsKeyPressed(KEY_ENTER))
				current = GAME_MENU;
			break;

		case GAME_EXIT:
			ClearBackground(WHITE);
			if (IsKeyPressed(KEY_D))
			{
				if (sel2 < 1)
					sel2++;
				else
					sel2 = 0;
			}
			else if (IsKeyPressed(KEY_A))
			{
				if (sel2 == 0)
					sel2 = 1;
				else
					sel2--;
			}

			DrawText(sure, x_sure, 400, 40, BLACK);
			DrawText(no, x_no + 100, 600, 40, BLACK);
			DrawText(yes, x_yes - 100, 600, 40, BLACK);

			if (!sel2)
			{
				DrawText(">", x_yes - 125, 600, 40, BLACK);
				if (IsKeyPressed(KEY_ENTER))
				{
					running = false;
				}
			}
			else
			{
				DrawText(">", x_no + 75, 600, 40, BLACK);
				if (IsKeyPressed(KEY_ENTER))
					current = GAME_MENU;
			}
			break;
		}

		EndDrawing();
	}

	UnloadTexture(logo);
	CloseWindow();
	return 0;
}
