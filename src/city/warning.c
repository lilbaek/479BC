#include <vcruntime_string.h>
#include <string.h>
#include "warning.h"

#include "city/view.h"
#include "core/time.h"
#include "graphics/window.h"
#include "translation/translation.h"
#include "core/direction.h"

#define MAX_WARNINGS 5
#define MAX_TEXT 100
#define TIMEOUT_MS 15000
#define TIMEOUT_FLASH 30

typedef struct {
    int in_use;
    time_millis time;
    int id;
    int flashing;
    char text[MAX_TEXT];
} warning;

static warning warnings[MAX_WARNINGS];
static int current_id;

static char *get_text_for_warning(warning_type type)
{
    switch (type) {
        case WARNING_ORIENTATION:
            switch (city_view_orientation()) {
                case DIR_0_TOP:
                    return gettext("North");
                case DIR_2_RIGHT:
                    return gettext("East");
                case DIR_4_BOTTOM:
                    return gettext("South");
                case DIR_6_LEFT:
                    return gettext("West");
            }
            return "";
        case WARNING_NO_MESS_HALL:
            return gettext("You must first build a supply post to feed your soldiers.");
        case WARNING_MAX_GRAND_TEMPLES:
            return gettext("No more Grand temples can be built.");
        case WARNING_WATER_NEEDED_FOR_LIGHTHOUSE:
            return gettext("Lighthouse must be placed near water");
        case WARNING_RESOURCES_NOT_AVAILABLE:
            return gettext("You cannot obtain the needed materials");
        case WARNING_VARIANT_TOGGLE:
            return gettext("Press the rotation key to cycle variants of this building");
        case WARNING_SECESSION:
            return gettext("Plebians are refusing to work");
        case WARNING_WAREHOUSE_BREAKIN:
            return gettext("A warehouse was looted!");
        case WARNING_GRANARY_BREAKIN:
            return gettext("A granary was looted!");
        case WARNING_THEFT:
            return gettext("Funds were stolen from a forum or senate!");
        case WARNING_WOLF_NEARBY:
            return gettext("Cannot build when wolves are nearby");
        case WARNING_BET_VICTORY:
            return gettext("Your horse won the race at your city's Hippodrome");
        case WARNING_BET_DEFEAT:
            return gettext("Your horse lost the race at your city's Hippodrome");
        case WARNING_DATA_COPY_SUCCESS:
            return gettext("Settings copied");
        case WARNING_DATA_COPY_NOT_SUPPORTED:
            return gettext("Can't copy from this building");
        case WARNING_DATA_PASTE_FAILURE:
            return gettext("Can't paste");
        case WARNING_DATA_PASTE_SUCCESS:
            return gettext("Settings pasted");
        case WARNING_DATA_MOTHBALL_ON:
            return gettext("Building is now mothballed");
        case WARNING_DATA_MOTHBALL_OFF:
            return gettext("Building is now active");
        case WARNING_GOLD_NEEDED:
            return gettext("This building needs gold");
        case WARNING_BUILD_GOLD_MINE:
            return gettext("Build a gold mine");
        case WARNING_SENATE_NEEDED:
            return gettext("This building needs a working senate");
        case WARNING_BUILD_SENATE:
            return gettext("Build a senate");
        case WARNING_CLEAR_LAND_NEEDED:
            return gettext("Requires clear land to build");
        case WARNING_OUT_OF_MONEY:
            return gettext("Out of money");
        case WARNING_ONE_BUILDING_OF_TYPE:
            return gettext("It is only possible to build one building of this type");
        case WARNING_ROAD_ACCESS_NEEDED:
            return gettext("Building needs road access");
        case WARNING_NOT_AVAILABLE:
            return gettext("Not available");
        case WARNING_NOT_AVAILABLE_YET:
            return gettext("Currently not available");
        case WARNING_WORKERS_NEEDED:
            return gettext("Your city requires more workers");
        case WARNING_MORE_FOOD_NEEDED:
            return gettext("People are eating more food than what is produced");
        case WARNING_MEADOW_NEEDED:
            return gettext("Building requires farmland");
        case WARNING_WATER_NEEDED:
            return gettext("Building should be placed close to water");
        case WARNING_ROCK_NEEDED:
            return gettext("Building requires rocks");
        case WARNING_TREE_NEEDED:
            return gettext("Building requires trees");
        case WARNING_SHORE_NEEDED:
            return gettext("Building requires shore");
        case WARNING_IRON_NEEDED:
            return gettext("Building requires iron ore");
        case WARNING_VINES_NEEDED:
            return gettext("Building requires grapes");
        case WARNING_OLIVES_NEEDED:
            return gettext("Building requires olives");
        case WARNING_CLAY_NEEDED:
            return gettext("Building requires clay");
        case WARNING_TIMBER_NEEDED:
            return gettext("Building requires timber");
        case WARNING_OPEN_TRADE_TO_IMPORT:
            return gettext("Open trade route to import this good");
        case WARNING_TRADE_IMPORT_RESOURCE:
            return gettext("Open trade route to import this good");
        case WARNING_BUILD_IRON_MINE:
            return gettext("Consider building an iron mine");
        case WARNING_BUILD_VINES_FARM:
            return gettext("Consider building a vines farm");
        case WARNING_BUILD_OLIVE_FARM:
            return gettext("Consider building an olive farm");
        case WARNING_BUILD_CLAY_PIT:
            return gettext("Consider building a clay pit");
        case WARNING_BUILD_TIMBER_YARD:
            return gettext("Consider building a timber yard");
        case WARNING_WATER_PIPE_ACCESS_NEEDED:
            return gettext("Needs to be placed inside area of a reservoir");
        case WARNING_SENTRIES_NEED_WALL:
            return "Unknown warning - wall";
        case WARNING_LEGION_MORALE_TOO_LOW:
            return "Unknown warning - moral";
        case WARNING_MAX_LEGIONS_REACHED:
            return "Unknown warning - reached";
        case WARNING_PEOPLE_ON_BRIDGE:
            return gettext("Cannot remove bride while people are on it");
        case WARNING_DOCK_OPEN_WATER_NEEDED:
            return gettext("Dock needs access to the sea to function");
        case WARNING_EDITOR_NEED_MAP_EDGE:
            return "Unknown warning - edge";
        case WARNING_EDITOR_NEED_OPEN_WATER:
            return "Unknown warning - water";
        case WARNING_EDITOR_CANNOT_PLACE:
            return "Unknown warning - editor";
        case WARNING_HOUSE_TOO_FAR_FROM_ROAD:
            return gettext("Some houses placed too far from a road");
        case WARNING_CITY_BOXED_IN:
            return gettext("Part of the city is unreachable");
        case WARNING_CITY_BOXED_IN_PEOPLE_WILL_PERISH:
            return gettext("People living in unreachable city parts will soon perish");
        case WARNING_DATA_LIMIT_REACHED:
            return gettext("Not possible to create new buildings. Maximum limit reached");
        case WARNING_BUILD_BARRACKS:
            return "Unknown warning - barracks";
        case WARNING_WEAPONS_NEEDED:
            return "Unknown warning - weapons";
        case WARNING_BUILD_ACTOR_COLONY:
            return gettext("Build actor school to train performers");
        case WARNING_BUILD_GLADIATOR_SCHOOL:
            return gettext("Build gladiator school to train fighters");
        case WARNING_BUILD_LION_HOUSE:
            return gettext("Build lion house");
        case WARNING_BUILD_CHARIOT_MAKER:
            return gettext("Build chariot maker");
        case WARNING_WALL_NEEDED:
            return "Unknown warning - wall needed";
        case WARNING_ENEMY_NEARBY:
            return "Unknown warning - enemy";
    }
    return "Unknown warning";
}

int city_warning_show(warning_type type, int id)
{
    return city_warning_show_custom(get_text_for_warning(type), id);
}

static warning *get_warning_slot(int id, const char *text)
{
    if (id != 0) {
        for (int i = 0; i < MAX_WARNINGS; i++) {
            if (warnings[i].id == id && warnings[i].in_use) {
                return &warnings[i];
            }
        }
    }
    for (int i = 0; i < MAX_WARNINGS; i++) {
        if (warnings[i].in_use && strcmp(warnings[i].text, text) == 0) {
            if (warnings[i].time != time_get_millis()) {
                warnings[i].flashing = 1;
            }
            return &warnings[i];
        }
    }
    for (int i = 0; i < MAX_WARNINGS; i++) {
        if (!warnings[i].in_use) {
            warnings[i].id = ++current_id;
            warnings[i].flashing = 0;
            return &warnings[i];
        }
    }
    return 0;
}

int city_warning_show_custom(const char *text, int id)
{
    warning *w = get_warning_slot(id, text);
    if (!w) {
        return 0;
    }
    w->in_use = 1;
    w->time = time_get_millis();
    if (!w->flashing) {
        memcpy(w->text, text, MAX_TEXT);
    }
    return w->id;
}

int city_has_warnings(void)
{
    for (int i = 0; i < MAX_WARNINGS; i++) {
        if (warnings[i].in_use) {
            return 1;
        }
    }
    return 0;
}

const char *city_warning_get(int position)
{
    if (warnings[position].in_use) {
        if (warnings[position].flashing) {
            if (time_get_millis() - warnings[position].time > TIMEOUT_FLASH) {
                warnings[position].flashing = 0;
                window_request_refresh();
            }
        } else {
            return warnings[position].text;
        }
    }
    return 0;
}

void city_warning_clear_all(void)
{
    for (int i = 0; i < MAX_WARNINGS; i++) {
        warnings[i].in_use = 0;
    }
}

void city_warning_clear_outdated(void)
{
    for (int i = 0; i < MAX_WARNINGS; i++) {
        if (warnings[i].in_use && time_get_millis() - warnings[i].time > TIMEOUT_MS) {
            warnings[i].in_use = 0;
            window_request_refresh();
        }
    }
}
