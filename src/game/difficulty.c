#include "difficulty.h"

#include "core/calc.h"
#include "core/config.h"

static const struct {
    int money;
    int enemies;
    int starting_favor;
    int sentiment;
    int levies;
    int base_tax_rate;
    int food_per_soldier;
    int favor_to_pause_emperor_attack;
    int favor_to_stop_emperor_attack;
    int random_events_cooldown_months;
    int high_salary_punishment;
} data[] = {
    { 300,  40, 70, 80,   0, 10, -3, 16, 20, 36, 0 }, // very easy
    { 200,  60, 60, 75,   0,  9, -3, 17, 22, 30, 0 }, // easy
    { 150,  80, 50, 70,  50,  8, -2, 18, 24, 24, 1 }, // normal
    { 100, 100, 50, 65, 100,  7,  0, 20, 27, 18, 3 }, // hard
    {  75, 120, 40, 60, 100,  6,  1, 22, 30, 12, 5 }  // very hard
};

int difficulty_starting_favor(void)
{
    return data[config_get(CONFIG_GP_DIFFICULTY)].starting_favor;
}

int difficulty_sentiment(void)
{
    return data[config_get(CONFIG_GP_DIFFICULTY)].sentiment;
}

int difficulty_base_tax_rate(void)
{
    return data[config_get(CONFIG_GP_DIFFICULTY)].base_tax_rate;
}

int difficulty_adjust_money(int money)
{
    return calc_adjust_with_percentage(money, data[config_get(CONFIG_GP_DIFFICULTY)].money);
}

int difficulty_adjust_enemies(int enemies)
{
    return calc_adjust_with_percentage(enemies, data[config_get(CONFIG_GP_DIFFICULTY)].enemies);
}

int difficulty_adjust_levies(int amount)
{
    return calc_adjust_with_percentage(amount, data[config_get(CONFIG_GP_DIFFICULTY)].levies);
}

int difficulty_adjust_soldier_food_consumption(int amount)
{
    return amount + data[config_get(CONFIG_GP_DIFFICULTY)].food_per_soldier;
}

int difficulty_adjust_wolf_attack(int attack)
{
    switch (config_get(CONFIG_GP_DIFFICULTY)) {
        case DIFFICULTY_VERY_EASY: return 2;
        case DIFFICULTY_EASY: return 4;
        case DIFFICULTY_NORMAL: return 6;
        default: return attack;
    }
}

int difficulty_favor_to_pause_emperor_attack(void)
{
    return data[config_get(CONFIG_GP_DIFFICULTY)].favor_to_pause_emperor_attack;
}

int difficulty_favor_to_stop_emperor_attack(void)
{
    return data[config_get(CONFIG_GP_DIFFICULTY)].favor_to_stop_emperor_attack;
}

int difficulty_random_event_cooldown_months(void)
{
    return data[config_get(CONFIG_GP_DIFFICULTY)].random_events_cooldown_months;
}

int difficulty_high_salary_punishment(void)
{
    return data[config_get(CONFIG_GP_DIFFICULTY)].high_salary_punishment;
}
