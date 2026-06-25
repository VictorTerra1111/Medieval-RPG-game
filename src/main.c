#include <stdio.h>
#include "raylib.h"

typedef enum {
    GAME_INTRO,
    GAME_MENU,
    GAME_PLAYING,
    GAME_SAVES,
    GAME_OPTIONS,
    GAME_EXIT
} State;

State current = GAME_INTRO;
State next_state;

double fade_rate = 0.0f;

#define FADE_IN 0
#define FADE_OUT 1
#define MENU_FONT_SIZE 40
#define SCALE 4.0f

int fade_screen(int io){
	
	if(io == FADE_IN){

		fade_rate -= 200 * GetFrameTime();
		if (fade_rate <= 0){
			fade_rate = 0;
			return 0; // OVER
		}	
	}

	else{
		fade_rate += 200 * GetFrameTime();

                if (fade_rate >= 255){
			fade_rate = 255;
			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),Fade(BLACK, fade_rate / 255.0f) );
			return 0; // OVER
		}
	}
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),Fade(BLACK, fade_rate / 255.0f) );
	return 1; // NOT YET FINISHED
}


int main(void){

	SetTraceLogLevel(LOG_NONE);
	InitWindow(500, 500, "Corpsia");

	Texture2D logo = LoadTexture("assets/sprites/corpsia_logo_black.png");
	Texture2D logo2= LoadTexture("assets/sprites/corpsia_logo_white.png");
	Texture2D background_intro = LoadTexture("assets/sprites/Background_intro.png");

	Texture2D b_menu1 = LoadTexture("assets/sprites/background_menu_pic1.png");
	Texture2D b_menu2 = LoadTexture("assets/sprites/background_menu_pic2.png");
        Texture2D b_menu3 = LoadTexture("assets/sprites/background_menu_pic3.png");

	ToggleFullscreen();
	SetTargetFPS(60);
	bool fade_o = false;
	bool fade_i = true;
	double wall_changing = 0.0f;
        int wall_turn = 1;
	bool running = true;	
	int sel2 = 0;
	int sel = 0;
	while (!WindowShouldClose() && running) {
        	BeginDrawing();
		ClearBackground(BLACK);
			
		switch (current) { // current state of the game looping
			case GAME_INTRO: // intro screen with logo and enter only
				// printf("ENTERED GAME_INTRO STATE\n");

				//calculate logo position
                		int width = logo.width * SCALE; 
                		int height = logo.height * SCALE;

                		int x = (GetScreenWidth() - width) / 2;
                		int y = (GetScreenHeight() - height) / 2;
		
				DrawTexture(background_intro, 0, 0, WHITE);

                		DrawTextureEx(logo, (Vector2){x, y}, 0, SCALE, WHITE);
				
				const char *start = "START";
				
				int t_wid = MeasureText(start, MENU_FONT_SIZE);
				int x_text = (GetScreenWidth() - t_wid) / 2;
				
				DrawText(start, x_text, 960, 40, BLACK);
	
				if (IsKeyPressed(KEY_ENTER)) {	
					fade_o = true;

				}	
				if(fade_o){
					
					DrawTextureEx(logo2, (Vector2){x, y}, 0, SCALE, WHITE);
					
					if(!fade_screen(FADE_OUT)) 
					{
						current = GAME_MENU;
						fade_o = false;
						fade_i = true;
						fade_rate = 255;
						// printf("\n\nDEBUG game state during fade: GAME_MENU\n\n");
					}
				}
			
				break;

    			case GAME_MENU:
				
				const char *start_op = "SAVE FILES";
				const char *options_op = "OPTIONS";
				const char *exit_op = "EXIT";

                                int sta_wid = MeasureText(start_op, MENU_FONT_SIZE);
				int opt_wid = MeasureText(options_op, MENU_FONT_SIZE);
                                int exi_wid = MeasureText(exit_op, MENU_FONT_SIZE);

                                int x_sta = (GetScreenWidth() - sta_wid) / 2;
                                int x_opt = (GetScreenWidth() - opt_wid) / 2;
                                int x_exi = (GetScreenWidth() - exi_wid) / 2;

						
				wall_changing += GetFrameTime();
				
				if(wall_changing >= 3.0f){
					wall_changing = 0.0f;

					if(wall_turn == 3){
						wall_turn = 1;
					}
					else wall_turn++;
				}

				switch (wall_turn){
					case 1: DrawTexture(b_menu1, 0, 0, WHITE); break;
					case 2: DrawTexture(b_menu2, 0, 0, WHITE); break;
					case 3: DrawTexture(b_menu3, 0, 0, WHITE); break;
				}
                                
				DrawText(start_op, x_sta, 300, 40, BLACK);
                                DrawText(options_op, x_opt, 400, 40, BLACK);
                                DrawText(exit_op, x_exi, 500, 40, BLACK);

				// printf("FADE RATE: %.2f\n", fade_rate);

				if(fade_i) if(!fade_screen(FADE_IN)) fade_i = false;

				if (IsKeyPressed(KEY_S)) {
					if(sel < 2) sel++;
					else sel = 0;
				}
				else if (IsKeyPressed(KEY_W)){   
					if(sel == 0) sel = 2;
					else sel--;
				}
				switch(sel){
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

				if (IsKeyPressed(KEY_ENTER)){
					switch(sel){
						case 0: // start
							// printf("PRESSED START\n");
							current = GAME_SAVES;
						break;

						case 1: // options
							// printf("PRESSED OPTIONS\n");
							current = GAME_OPTIONS;
						break;

						case 2: // out
							// printf("PRESSED OUT\n");
							current = GAME_EXIT;
							sel2 = 0;
						break;
						
				
					}
				}
			break;

			case GAME_SAVES:
				ClearBackground(PURPLE);
				const char *save_file = "EMPTY SLOT";
                                const char *go_back = "BACK TO START";
                                int sav_wid= MeasureText(save_file, MENU_FONT_SIZE);
                                int gob_wid = MeasureText(go_back, MENU_FONT_SIZE);
                    
                                int x_sav = (GetScreenWidth() - sav_wid) / 2;
                                int x_gob = (GetScreenWidth() - gob_wid) / 2;
                           

                                DrawText(save_file, x_sav, 300, 40, BLACK);
                                DrawText(save_file, x_sav, 400, 40, BLACK);
                                DrawText(save_file, x_sav, 500, 40, BLACK);
				DrawText(go_back, x_gob, 800, 40, BLACK);
			break;
    			
			case GAME_PLAYING:
				ClearBackground(GREEN);
				// printf("ENTERED GAME_PLAYING STATE\n");
			break;

			case GAME_OPTIONS:
				ClearBackground(WHITE);
				DrawText("There are no options yet, so go play the game", 0, 0, 40, BLACK);
				if(IsKeyPressed(KEY_ENTER)) current = GAME_MENU;

			
				// printf("ENTERED GAME_OPTIONS STATE\n");
			break;

			case GAME_EXIT:
				const char *sure = "ARE YOU SURE?";
				const char *no = "NO";
				const char *yes = "YES";

				int sure_wid = MeasureText(sure, MENU_FONT_SIZE);
				int no_wid = MeasureText(no, MENU_FONT_SIZE);
				int yes_wid = MeasureText(yes, MENU_FONT_SIZE);

				int x_sure = (GetScreenWidth() - sure_wid) / 2;
				int x_no = (GetScreenWidth() - no_wid) / 2;
				int x_yes = (GetScreenWidth() - yes_wid) / 2;
			       	
				ClearBackground(WHITE);

				if (IsKeyPressed(KEY_D)) {
                                        if(sel2 < 1) sel2 ++;
                                        else sel2 = 0;
                                }
                                else if (IsKeyPressed(KEY_A)){
                                        if(sel2 == 0) sel2 = 1;
                                        else sel2 --;
                                }
	
				DrawText(sure, x_sure, 400, 40, BLACK);
				DrawText(no, x_no + 100, 600, 40, BLACK);
				DrawText(yes, x_yes - 100, 600, 40, BLACK);
				
				if(!sel2) {
					DrawText(">", x_yes - 125, 600, 40, BLACK);
					if(IsKeyPressed(KEY_ENTER)) {
						running = false;	
					}
				}
				else {
					DrawText(">", x_no + 75, 600, 40, BLACK);
					if(IsKeyPressed(KEY_ENTER)) current = GAME_MENU;
				}
				// ClearBackground(RED);
				// printf("ENTERED GAME_EXIT STATE\n");
			break;
		}

		EndDrawing();
    	}
	
	UnloadTexture(logo);
	CloseWindow();
	return 0;
}
