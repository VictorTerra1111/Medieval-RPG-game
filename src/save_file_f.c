#include <stdio.h>
#include <string.h>

#include "../include/save_file_f.h"
#include "../include/structs.h"
#include "../include/player.h"

int create_save_file(int save_slot, MainChar *player)
{
    char filename[32];
    snprintf(filename, sizeof(filename), "save_files/save_file_%d.sav", save_slot);

    FILE *save = fopen(filename, "r");

    if (save != NULL)
    {
        fclose(save);
        return -2; // there is another save in the slot
    }

    save = fopen(filename, "wb");

    if (save == NULL)
        return -1; // cannot create file

    fwrite(player, sizeof(MainChar), 1, save);

    fclose(save);

    return 0; // ok
}

int delete_save_file(int save_slot)
{
    char filename[32];
    snprintf(filename, sizeof(filename), "save_files/save_file_%d.sav", save_slot);

    FILE *save = fopen(filename, "rb");

    if (save == NULL)
        return -2; // not found

    fclose(save);

    if (remove(filename) != 0)
        return -1; // couldnt erase

    return 0; // ok
}

int save_game(MainChar *player, int save_slot)
{
    char filename[64];

    snprintf(filename, sizeof(filename), "saves_files/save_file_%d.sav", save_slot);

    FILE *save = fopen(filename, "wb");

    if (save == NULL)
        return -1;

    fwrite(player, sizeof(MainChar), 1, save);

    fclose(save);

    return 0;
}

int save_exists(int save_slot)
{
    char filename[64];

    snprintf(filename, sizeof(filename), "save_files/save_file_%d.sav", save_slot);

    FILE *save = fopen(filename, "rb");

    if (save == NULL)
        return 0;

    fclose(save);

    return 1;
}

MainChar load_save_file(int save_slot)
{
    MainChar player = {0};

    char filename[64];

    snprintf(filename, sizeof(filename), "save_files/save_file_%d.sav", save_slot);

    FILE *save = fopen(filename, "rb");

    if (save == NULL)
        return player;

    if (fread(&player, sizeof(MainChar), 1, save) != 1)
    {
        fclose(save);
        return player;
    }

    fclose(save);
    return player;
}
