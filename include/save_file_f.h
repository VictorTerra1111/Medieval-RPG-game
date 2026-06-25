#ifndef SAVE_FILE_F_H
#define SAVE_FILE_F_H

#include "player.h"

int create_save_file(int save_slot);

int delete_save_file(int save_slot);

int save_game(MainChar *player, int save_slot);

int save_exists(int save_slot);

MainChar load_save_file(int save_slot);

#endif