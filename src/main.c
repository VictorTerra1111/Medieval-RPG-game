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

#define MENU_FONT_SIZE 40

int main(void){
	SetTraceLogLevel(LOG_NONE);
	InitWindow(500, 500, "Corpsia");

	Texture2D logo = LoadTexture("assets/sprites/corpsia_logo_black.png");

	ToggleFullscreen();
	SetTargetFPS(60);
	int sel = 0;

	while (!WindowShouldClose()) {
        	BeginDrawing();
        	switch (current) { // current state of the game looping
			case GAME_INTRO: // intro screen with logo and enter only
				printf("ENTERED GAME_INTRO STATE\n");
				ClearBackground(RAYWHITE);

        		        double scale = 4.0f;
	

				//calculate logo position
                		int width = logo.width * scale; 
                		int height = logo.height * scale;

                		int x = (GetScreenWidth() - width) / 2;
                		int y = (GetScreenHeight() - height) / 2;
		
                		DrawTextureEx(logo, (Vector2){x, y}, 0, scale, WHITE);
				
				const char *start = "START";
				
				int t_wid = MeasureText(start, MENU_FONT_SIZE);
				int x_text = (GetScreenWidth() - t_wid) / 2;
				
				DrawText(start, x_text, 960, 40, BLACK);
				if (IsKeyPressed(KEY_ENTER)) current = GAME_MENU; // goes to the next screen MENU
			break;

    			case GAME_MENU:
				ClearBackground(YELLOW);
				printf("ENTERED GAME_MENU STATE\n");
				
				const char *start_op = "SAVE FILES";
				const char *options_op = "OPTIONS";
				const char *exit_op = "EXIT";

                                int sta_wid = MeasureText(start_op, MENU_FONT_SIZE);
				int opt_wid = MeasureText(options_op, MENU_FONT_SIZE);
                                int exi_wid = MeasureText(exit_op, MENU_FONT_SIZE);

                                int x_sta = (GetScreenWidth() - sta_wid) / 2;
                                int x_opt = (GetScreenWidth() - opt_wid) / 2;
                                int x_exi = (GetScreenWidth() - exi_wid) / 2;

                                DrawText(start_op, x_sta, 300, 40, BLACK);
                                DrawText(options_op, x_opt, 400, 40, BLACK);
                                DrawText(exit_op, x_exi, 500, 40, BLACK);

				if (IsKeyPressed(KEY_S)) {
					if(sel < 2) sel++;
					else sel = 0;
				}
				else if (IsKeyPressed(KEY_W)){   
					if(sel == 0) sel = 3;
					else sel--;
				}
				if (IsKeyPressed(KEY_ENTER)){
					switch(sel){
						case 0: // start
							printf("PRESSED START\n");
							current = GAME_SAVES;
						break;

						case 1: // options
							printf("PRESSED OPTIONS\n");
							current = GAME_OPTIONS;
						break;

						case 2: // out
							printf("PRESSED OUT\n");
							current = GAME_EXIT;

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
				printf("ENTERED GAME_PLAYING STATE\n");
			break;

			case GAME_OPTIONS:
				ClearBackground(BLUE);
		
			
				printf("ENTERED GAME_OPTIONS STATE\n");
			break;

			case GAME_EXIT:
				ClearBackground(RED);
				printf("ENTERED GAME_EXIT STATE\n");
			break;
		}

		EndDrawing();
    	}
	
	UnloadTexture(logo);
	CloseWindow();
	return 0;
}
