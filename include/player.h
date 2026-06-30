#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "structs.h"
#include <string.h>
#include "defines.h"

typedef struct
{
	char name[32];

	int life;
	int mana;
	int arrows; // how many arrows, -1 if no bow
	int money;
	int collectables;
	int bells;

	SwordType sword;   // 0 for no sword, 1 for burnt sword, 2 for wooden, 3 for metal, 4 for false twilight, 5 for real tw
	ShieldType shield; // 0 for no shield, 1 for burnt shield, 2 for wooden, 3 for metal, 4 for life shield

	bool jewels[5];	   // forest, tide, passion, wind, thunder
	bool Inventory[6]; // for armor: tiara, necklace, earrings, wrist, ring, boots
	bool has_whistle;
	bool can_teleport;

} MainChar;

int naming_character(char name[32]);

#endif
