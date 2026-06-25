#include <stdio.h>
#include "raylib.h"

typedef enum {
    GAME_INTRO,
    GAME_MENU,
    GAME_PLAYING,
    GAME_OPTIONS,
    GAME_EXIT
} State;

State current = GAME_INTRO;

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

				if (IsKeyPressed(KEY_ENTER)) current = GAME_MENU; // goes to the next screen MENU
			break;

    			case GAME_MENU:
				ClearBackground(YELLOW);
				printf("ENTERED GAME_MENU STATE\n");
				
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
							current = GAME_PLAYING;
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
