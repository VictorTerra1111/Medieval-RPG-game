#ifndef PLAYER_HPP
#define PLAYER_HPP

#define INIT_HEARTS = 5

#include <string>
using namespace std;


class player_c {
    private:
        string name;
        int lv = 1;
        int max_lv = 50;
        int max_hearts = 5;
        int current_hearts = 5;
        bool is_dead = false;
        int current_xp = 0;
        double xp_formula;
    
        int attack_points = 1;
        int defense_points = 0;
        int mana_points = 5;
        int stamina_points = 5;

        int condition = 0;

    public:
        void set_name(string v);
        string get_name();

        double get_xpToNextLevel();
        void up_hearts();
        void up_xp(int new_xp);
        void damage(int hit_points);
        void healing_hearts(int life_points);

        void buff_guy(int def);
        void power_guy(int at);
        void up_mana(int mn);
        void up_stamina(int st);

        void change_condition(int value);
};


#endif