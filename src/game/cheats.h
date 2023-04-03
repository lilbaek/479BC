#ifndef GAME_CHEATS_H
#define GAME_CHEATS_H

#include <stdint.h>

void game_cheat_add_money(int money);

void game_cheat_start_invasion(int type, int size, int invasion_point);

void game_cheat_advance_year();

void game_cheat_cast_blessing(int god_id);

void game_cheat_show_tooltip(int show);

void game_cheat_kill_all();

void game_cheat_finish_monuments();

void game_cheat_set_monument_phase(int phase);

void game_cheat_unlock_all_buildings();

void game_cheat_incite_riot();

void game_cheat_activate();

int game_cheat_tooltip_enabled();
int game_cheat_is_cheating();

void game_cheat_fill_markets();
#endif // GAME_CHEATS_H
