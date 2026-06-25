#include <stdio.h>
#include <string.h>
#include "raylib.h"

typedef enum {
    GAME_INTRO,
    GAME_MENU,
    GAME_PLAYING,
    GAME_SAVES,
    GAME_SAVE_ALREADY_EXIST,
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

typedef enum {
    SWORD_NONE,
    SWORD_BURNT,
    SWORD_WOODEN,
    SWORD_METAL,
    SWORD_FALSE_TWILIGHT,
    SWORD_REAL_TWILIGHT
} SwordType;

typedef enum {
    SHIELD_NONE,
    SHIELD_BURNT,
    SHIELD_WOODEN,
    SHIELD_METAL,
    SHIELD_LIFE
} ShieldType;

typedef enum {
    JEWEL_FOREST,
    JEWEL_TIDE,
    JEWEL_PASSION,
    JEWEL_WIND,
    JEWEL_THUNDER,
    JEWEL_COUNT
} Jewel;

typedef struct{
	char name[32];
	
	int life;
	int mana;
	
	SwordType sword; // 0 for no sword, 1 for burnt sword, 2 for wooden, 3 for metal, 4 for false twilight, 5 for real tw
	ShieldType shield; // 0 for no shield, 1 for burnt shield, 2 for wooden, 3 for metal, 4 for life shield
	
	int arrows; // how many arrows, -1 if no bow 
	bool jewels[5]; // forest, tide, passion, wind, thunder
	
	bool Inventory[6]; // for armor: tiara, necklace, earrings, wrist, ring, boots
	
	int money; 
	int collectables;
	int bells;
	
	bool has_whistle;
	bool can_teleport;
} MainChar;


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


int create_save_file(int save_slot){
	char filename[32];
	snprintf(filename, sizeof(filename), "save_files/save_file_%d.sav", save_slot);

	FILE *save = fopen(filename, "r");
	
	if (save != NULL){
		fclose(save);
		return -2; // there is another save in the slot
	}

	MainChar player = {0};

    	strcpy(player.name, "Renna Legot");
    	player.life = 100;

    	save = fopen(filename, "wb");

    	if (save == NULL) return -1; // cannot create file

    	fwrite( &player, sizeof(MainChar), 1,save);

    	fclose(save);

    return 0; // ok
}


int delete_save_file(int save_slot) {
	char filename[32];
	snprintf(filename, sizeof(filename), "save_files/save_file_%d.sav", save_slot);

    	FILE *save = fopen(filename, "rb");

    	if (save == NULL) return -2; // not found

    	fclose(save);

	if (remove(filename) != 0) return -1; // couldnt erase

    	return 0; // ok
}

int save_game(MainChar *player, int save_slot){
    char filename[64];

    snprintf(
        filename,
        sizeof(filename),
        "saves/save_file_%d.sav",
        save_slot
    );

    FILE *save = fopen(filename, "wb");

    if (save == NULL)
        return -1;

    fwrite(
        player,
        sizeof(MainChar),
        1,
        save
    );

    fclose(save);

    return 0;
}

MainChar load_save_file(int save_slot)
{
    MainChar player = {0};

    char filename[64];

    snprintf(
        filename,
        sizeof(filename),
        "saves/save_file_%d.sav",
        save_slot
    );

    FILE *save = fopen(filename, "rb");

    if (save == NULL)
        return player;

    if (fread(&player, sizeof(MainChar), 1, save) != 1){
    	fclose(save);
    	return player;
    }
    
    fclose(save);
    return player;
}

int save_exists(int save_slot)
{
    char filename[64];

    snprintf(
        filename,
        sizeof(filename),
        "save_files/save_file_%d.sav",
        save_slot
    );

    FILE *save = fopen(filename, "rb");

    if (save == NULL)
        return 0;

    fclose(save);

    return 1;
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
	int sel3 = 0;
	int sel_slot = 1;

	const char *sure = "ARE YOU SURE?";
                                const char *no = "NO";
                                const char *yes = "YES";

                                int sure_wid = MeasureText(sure, MENU_FONT_SIZE);
                                int no_wid = MeasureText(no, MENU_FONT_SIZE);
                                int yes_wid = MeasureText(yes, MENU_FONT_SIZE);

                                int x_sure = (GetScreenWidth() - sure_wid) / 2;
                                int x_no = (GetScreenWidth() - no_wid) / 2;
                                int x_yes = (GetScreenWidth() - yes_wid) / 2;

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
				ClearBackground(WHITE);
                                const char *go_back = "BACK TO START";
                                int gob_wid = MeasureText(go_back, MENU_FONT_SIZE);
                                int x_gob = (GetScreenWidth() - gob_wid) / 2;
                           	char slot1_text[32] = "EMPTY SLOT";
				char slot2_text[32] = "EMPTY SLOT";
				char slot3_text[32] = "EMPTY SLOT";
			

if(save_exists(1))
{
    MainChar p = load_save_file(1);
    snprintf(slot1_text, sizeof(slot1_text),"%s", p.name);
                                

}

if(save_exists(2))
{
    MainChar p = load_save_file(2);
    snprintf(slot2_text, sizeof(slot2_text),"%s", p.name);
}

if(save_exists(3))
{
    MainChar p = load_save_file(3);
    snprintf(slot3_text, sizeof(slot3_text), "%s", p.name);
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
			
				if (IsKeyPressed(KEY_S)) {
                                        if(sel3 < 3) sel3++;
                                        else sel3 = 0;
                                }
                                else if (IsKeyPressed(KEY_W)){
                                        if(sel3 == 0) sel3 = 3;
                                        else sel3--;
                                }
                                switch(sel3){
                                        case 0:
                                                DrawText(">", x_s1 - 100, 300, 40, BLACK);
                   				
						if(IsKeyPressed(KEY_ENTER)){
							sel_slot = 1;
							if(create_save_file(1) == -2) current = GAME_PLAYING;
						}
						else if(IsKeyPressed(KEY_DELETE)){
							sel_slot = 1;
							current = GAME_SAVE_ALREADY_EXIST;
						}
						
						break;
                                        case 1:
                                                DrawText(">", x_s2 - 100, 400, 40, BLACK);
                                                if(IsKeyPressed(KEY_ENTER)){
                                               		sel_slot = 2;
					       		if(create_save_file(2) == -2) current = GAME_PLAYING;
                                                }
                                                else if(IsKeyPressed(KEY_DELETE)){
                                                        sel_slot = 2;
							current = GAME_SAVE_ALREADY_EXIST;
                                                }

						break;
                                        case 2:
						
                                                DrawText(">", x_s3 - 100, 500, 40, BLACK);
                                                if(IsKeyPressed(KEY_ENTER)){
                                                	sel_slot = 3;
							if(create_save_file(3) == -2) current = GAME_PLAYING;
                                                }
                                                else if(IsKeyPressed(KEY_DELETE)){
                                                        sel_slot = 3;
							sel2 = 0;
							current = GAME_SAVE_ALREADY_EXIST;
                                                	
						}

						break;
					case 3:
						DrawText(">", x_gob - 100, 800, 40, BLACK);
						if(IsKeyPressed(KEY_ENTER)) {
							sel = 0;	
							current = GAME_MENU;
						}
						break;
                                }

				break;
			case GAME_SAVE_ALREADY_EXIST:
				
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
                                                delete_save_file(sel_slot);
                                        }
                                }
                                else {
                                        DrawText(">", x_no + 75, 600, 40, BLACK);
                                        if(IsKeyPressed(KEY_ENTER)) current = GAME_SAVES;
                                }

				
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
