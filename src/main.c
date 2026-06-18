#include "raylib.h"

int main(void){
	InitWindow(500, 500, "Corpsia");
	
	ToggleFullscreen();
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Welcome to Corpsia", 0, 0, 40, RED);
        EndDrawing();
    }
	
	CloseWindow();
	return 0;
}
