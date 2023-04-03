#include "cheats.h"

#include "building/menu.h"
#include "building/monument.h"
#include "city/gods.h"
#include "city/finance.h"
#include "city/data_private.h"
#include "city/sentiment.h"
#include "city/warning.h"
#include "core/lang.h"
#include "empire/city.h"
#include "figure/figure.h"
#include "figuretype/crime.h"
#include "game/tick.h"
#include "graphics/window.h"
#include "scenario/invasion.h"
#include "scenario/scenario.h"
#include "translation/translation.h"

static struct {
    int is_cheating;
    int tooltip_enabled;
} data;

void game_cheat_activate(void) {
    if (data.is_cheating) {
        data.is_cheating = 0;
    } else {
        data.is_cheating = 1;
    }
}

int game_cheat_tooltip_enabled(void) {
    return data.tooltip_enabled;
}

int game_cheat_is_cheating(void) {
    return data.is_cheating;
}

static void show_warning(translation_key key) {
    city_warning_show_custom(lang_get_string(CUSTOM_TRANSLATION, key), NEW_WARNING_SLOT);
}

void game_cheat_add_money(int money) {
    city_finance_process_console(money);
    window_invalidate();
    show_warning(TR_CHEAT_ADDED_MONEY);
}

void game_cheat_start_invasion(int type, int size, int invasion_point) {
    scenario_invasion_start_from_console(type, size, invasion_point);
    show_warning(TR_CHEAT_STARTED_INVASION);
}

void game_cheat_advance_year() {
    game_tick_cheat_year();
    show_warning(TR_CHEAT_YEAR_ADVANCED);
}

void game_cheat_cast_blessing(int god_id) {
    city_god_blessing_cheat(god_id);
    show_warning(TR_CHEAT_CASTED_BLESSING);
}

void game_cheat_show_tooltip(int show) {
    data.tooltip_enabled = show;
    show_warning(TR_CHEAT_TOGGLE_TOOLTIPS);
}

void game_cheat_kill_all() {
    figure_kill_all();
    show_warning(TR_CHEAT_KILLED_ALL_WALKERS);
}

void game_cheat_finish_monuments() {
    building_monument_finish_monuments();
    show_warning(TR_CHEAT_FINISHED_MONUMENTS);
}

void game_cheat_set_monument_phase(int phase) {
    building_monuments_set_construction_phase(phase);
    show_warning(TR_CHEAT_UPDATED_MONUMENTS);
}

void game_cheat_unlock_all_buildings() {
    building_menu_enable_all();
    empire_unlock_all_resources();
    scenario_unlock_all_buildings();
    show_warning(TR_CHEAT_UNLOCKED_ALL_BUILDINGS);
}

void game_cheat_incite_riot() {
    city_data.sentiment.value = 0;
    city_sentiment_change_happiness(-100);
    figure_generate_criminals();
    show_warning(TR_CHEAT_INCITED_RIOT);
    city_data.sentiment.value = 50;
    city_sentiment_change_happiness(50);
}

void game_cheat_fill_markets() {
    for (building *b = building_first_of_type(BUILDING_MARKET); b; b = b->next_of_type) {
        for (resource_type r = RESOURCE_MIN_NON_FOOD; r < RESOURCE_MAX_NON_FOOD; r++) {
            if (!resource_is_inventory(r)) {
                continue;
            }
            b->resources[r] = 100;
        }
    }
    for (building *b = building_first_of_type(BUILDING_GRANARY); b; b = b->next_of_type) {
        for (resource_type r = RESOURCE_MIN_FOOD; r < RESOURCE_MAX_FOOD; r++) {
            if (!resource_is_inventory(r)) {
                continue;
            }
            b->resources[r] = 300;
        }
    }
}