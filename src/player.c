#include <stdio.h>
#include <string.h>

#include "../include/player.h"
#include "raylib.h"

int name_confirming(char name[32], int name_wid, int x_name);

int naming_character(char name[32])
{
    static int size = 0;

    const char *default_name = "Renna Legot";
    const char *q = "Choose your name";
    
    int q_wid = MeasureText(q, MENU_FONT_SIZE);
    int x_q = (GetScreenWidth() - q_wid) / 2;

    ClearBackground(WHITE);

    DrawText(q, x_q, 300, 40, BLACK);

    int l = GetCharPressed();

    while (l > 0) {
	    if ((l >= 32) && (l <= 125) && (size < 31)){
            name[size] = (char)l;
            size++;
            name[size] = '\0';
        }

        l = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        if (size > 0)
        {
            size--;
            name[size] = '\0';
        }
    }

    const char *shown_name = (name[0] == '\0') ? default_name : name;

    int name_wid = MeasureText(shown_name, 80);
    int x_name = (GetScreenWidth() - name_wid) / 2;

    DrawText(shown_name, x_name, 500, 80, BLACK);

    if (IsKeyPressed(KEY_ENTER))
    {
        if (name[0] == '\0')
        {
            strcpy(name, default_name);
        }
        size = 0;
	
 	return name_confirming(name, name_wid, x_name);
    }

    return 0;
}


int name_confirming(char name[32], int name_wid, int x_name){
	const char *cor = "Your name shall be";
	int cor_wid = MeasureText(cor, MENU_FONT_SIZE);
	int x_cor = (GetScreenWidth() - cor_wid) / 2;

	ClearBackground(WHITE);

        DrawText(cor, x_cor, 300, 40, BLACK);

        name_wid = MeasureText(name, 80);
        x_name = (GetScreenWidth() - name_wid) / 2;

        DrawText(name, x_name, 500, 80, BLACK);

        return 1;

}
