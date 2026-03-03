#include <iostream>
#include <string>
#include "../hpps/player.hpp"

using namespace std;

void player_c::set_name(string v = "Renna Legot")
{
    name = v;
}

string player_c::get_name()
{
    return name;
}

void player_c::up_hearts()
{
    current_hearts = max_hearts;
    max_hearts++;
}

void player_c::healing_hearts(int life_points)
{
    if ((current_hearts + life_points) >= max_hearts)
    {
        current_hearts = max_hearts;
    }
    else
    {
        current_hearts += life_points;
    }
}

void player_c::damage(int hit_points)
{
    if (current_hearts <= hit_points)
    {
        is_dead = true;
        // game_over();
    }
    else
    {
        current_hearts -= hit_points;
    }
}
double player_c::get_xpToNextLevel()
{
    return 100 * lv * (lv / 2); // provisorio
}

void player_c::up_xp(int new_xp)
{
    while (new_xp > 0)
    {
        if ((current_xp + new_xp) < get_xpToNextLevel())
        {
            current_xp += new_xp;
            new_xp = 0;
        }
        else
        {
            lv++;
            new_xp -= get_xpToNextLevel();
        }
    }
}

void player_c::buff_guy(int def)
{
    defense_points += def;
}

void player_c::power_guy(int at)
{
    attack_points += at;
}

void player_c::up_mana(int mn)
{
    mana_points += mn;
}

void player_c::up_stamina(int st)
{
    stamina_points += st;
}

void player_c::change_condition(int value){ 
    // provisorio
    switch(value){
        case 1:
        // congelado
            break;
        case 2:
        // eletrecutado
            break;
        case 3:
        // pegando fogo
            break;
        case 4:
        // camuflado
            break;

        default: condition = 0; // de boa
    } 
}