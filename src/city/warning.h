#ifndef CITY_WARNING_H
#define CITY_WARNING_H

#include <stdint.h>

typedef enum {
    WARNING_ORIENTATION = 1,
    WARNING_CLEAR_LAND_NEEDED = 2,
    WARNING_OUT_OF_MONEY = 3,
    WARNING_ONE_BUILDING_OF_TYPE = 4,
    WARNING_ROAD_ACCESS_NEEDED = 5,
    WARNING_NOT_AVAILABLE = 6,
    WARNING_NOT_AVAILABLE_YET = 7,
    WARNING_WORKERS_NEEDED = 8,
    WARNING_MORE_FOOD_NEEDED = 9,
    WARNING_MEADOW_NEEDED = 11,
    WARNING_WATER_NEEDED = 12,
    WARNING_ROCK_NEEDED = 13,
    WARNING_TREE_NEEDED = 14,
    WARNING_SHORE_NEEDED = 15,
    WARNING_IRON_NEEDED = 16,
    WARNING_VINES_NEEDED = 17,
    WARNING_OLIVES_NEEDED = 18,
    WARNING_CLAY_NEEDED = 19,
    WARNING_TIMBER_NEEDED = 20,
    WARNING_OPEN_TRADE_TO_IMPORT = 21,
    WARNING_TRADE_IMPORT_RESOURCE = 22,
    WARNING_BUILD_IRON_MINE = 23,
    WARNING_BUILD_VINES_FARM = 24,
    WARNING_BUILD_OLIVE_FARM = 25,
    WARNING_BUILD_CLAY_PIT = 26,
    WARNING_BUILD_TIMBER_YARD = 27,
    WARNING_WATER_PIPE_ACCESS_NEEDED = 28,
    WARNING_SENTRIES_NEED_WALL = 29,
    WARNING_LEGION_MORALE_TOO_LOW = 30,
    WARNING_MAX_LEGIONS_REACHED = 31,
    WARNING_PEOPLE_ON_BRIDGE = 32,
    WARNING_DOCK_OPEN_WATER_NEEDED = 33,
    WARNING_EDITOR_NEED_MAP_EDGE = 34,
    WARNING_EDITOR_NEED_OPEN_WATER = 35,
    WARNING_EDITOR_CANNOT_PLACE = 36,
    WARNING_HOUSE_TOO_FAR_FROM_ROAD = 37,
    WARNING_CITY_BOXED_IN = 38,
    WARNING_CITY_BOXED_IN_PEOPLE_WILL_PERISH = 39,
    WARNING_DATA_LIMIT_REACHED = 40,
    WARNING_NO_MESS_HALL = 41,
    WARNING_MAX_GRAND_TEMPLES = 42,
    WARNING_WATER_NEEDED_FOR_LIGHTHOUSE = 43,
    WARNING_RESOURCES_NOT_AVAILABLE = 44,
    WARNING_VARIANT_TOGGLE = 45,
    WARNING_SECESSION = 46,
    WARNING_WAREHOUSE_BREAKIN = 47,
    WARNING_GRANARY_BREAKIN = 48,
    WARNING_THEFT = 49,
    WARNING_WOLF_NEARBY = 50,
    WARNING_BET_VICTORY = 51,
    WARNING_BET_DEFEAT = 52,
    WARNING_DATA_COPY_SUCCESS = 53,
    WARNING_DATA_COPY_NOT_SUPPORTED = 54,
    WARNING_DATA_PASTE_SUCCESS = 55,
    WARNING_DATA_PASTE_FAILURE = 56,
    WARNING_DATA_MOTHBALL_ON = 57,
    WARNING_DATA_MOTHBALL_OFF = 58,
    WARNING_GOLD_NEEDED = 59,
    WARNING_BUILD_GOLD_MINE = 80,
    WARNING_SENATE_NEEDED = 81,
    WARNING_BUILD_SENATE = 82,
    WARNING_BUILD_BARRACKS = 83,
    WARNING_WEAPONS_NEEDED = 84,
    WARNING_BUILD_ACTOR_COLONY = 85,
    WARNING_BUILD_GLADIATOR_SCHOOL = 86,
    WARNING_BUILD_LION_HOUSE = 87,
    WARNING_BUILD_CHARIOT_MAKER = 88,
    WARNING_WALL_NEEDED = 89,
    WARNING_ENEMY_NEARBY = 90,
} warning_type;

#define NEW_WARNING_SLOT 0

int city_warning_show(warning_type type, int id);
int city_warning_show_custom(const char *text, int id);

int city_has_warnings(void);

const char *city_warning_get(int position);

void city_warning_clear_all(void);
void city_warning_clear_outdated(void);

#endif // CITY_WARNING_H
