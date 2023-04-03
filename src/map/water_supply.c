#include "water_supply.h"

#include "assets/assets.h"
#include "building/building.h"
#include "building/image.h"
#include "building/monument.h"
#include "building/list.h"
#include "core/image.h"
#include "map/building_tiles.h"
#include "map/data.h"
#include "map/desirability.h"
#include "map/building.h"
#include "map/grid.h"
#include "map/image.h"
#include "map/property.h"
#include "map/terrain.h"
#include "map/tiles.h"
#include "scenario/property.h"

#include <string.h>

#define OFFSET(x,y) (x + GRID_SIZE * y)

#define MAX_QUEUE 1000
#define RESERVOIR_RADIUS 10
#define WELL_RADIUS 2
#define FOUNTAIN_RADIUS 4

static const int ADJACENT_OFFSETS[] = { -GRID_SIZE, 1, GRID_SIZE, -1 };

static struct {
    int items[MAX_QUEUE];
    int head;
    int tail;
} queue;

static void mark_well_access(int well_id, int radius)
{
    building *well = building_get(well_id);
    int x_min, y_min, x_max, y_max;
    map_grid_get_area(well->x, well->y, 1, radius, &x_min, &y_min, &x_max, &y_max);

    for (int yy = y_min; yy <= y_max; yy++) {
        for (int xx = x_min; xx <= x_max; xx++) {
            int building_id = map_building_at(map_grid_offset(xx, yy));
            if (building_id) {
                building_get(building_id)->has_well_access = 1;
            }
        }
    }
}

void map_water_supply_update_houses(void)
{
    for (building_type type = BUILDING_HOUSE_SMALL_TENT; type <= BUILDING_HOUSE_LUXURY_PALACE; type++) {
        for (building *b = building_first_of_type(type); b; b = b->next_of_type) {
            if (b->state != BUILDING_STATE_IN_USE || !b->house_size) {
                continue;
            }
            b->has_water_access = 0;
            b->has_well_access = 0;
            if (map_terrain_exists_tile_in_area_with_type(
                b->x, b->y, b->size, TERRAIN_FOUNTAIN_RANGE)) {
                b->has_water_access = 1;
            }
        }
    }
    for (building *b = building_first_of_type(BUILDING_WELL); b; b = b->next_of_type) {
        if (b->state == BUILDING_STATE_IN_USE) {
            mark_well_access(b->id, map_water_supply_well_radius());
        }
    }
}

void map_water_supply_update_reservoir_fountain(void)
{
    map_terrain_remove_all(TERRAIN_FOUNTAIN_RANGE | TERRAIN_RESERVOIR_RANGE);
    // reservoirs
    for (building *b = building_first_of_type(BUILDING_RESERVOIR); b; b = b->next_of_type) {
        if (b->state != BUILDING_STATE_IN_USE) {
            continue;
        }
        b->has_water_access = 2;
    }
    // mark reservoir ranges
    for (building *b = building_first_of_type(BUILDING_RESERVOIR); b; b = b->next_of_type) {
        if (b->state == BUILDING_STATE_IN_USE && b->has_water_access) {
            map_terrain_add_with_radius(b->x, b->y, 3, map_water_supply_reservoir_radius(), TERRAIN_RESERVOIR_RANGE);
        }
    }

    // Neptune GT module 2 bonus
    if (building_monument_gt_module_is_active(NEPTUNE_MODULE_2_CAPACITY_AND_WATER)) {
        building *b = building_get(building_monument_get_neptune_gt());
        map_terrain_add_with_radius(b->x, b->y, 7, map_water_supply_reservoir_radius(), TERRAIN_RESERVOIR_RANGE);
    }

    // fountains
    for (building *b = building_first_of_type(BUILDING_FOUNTAIN); b; b = b->next_of_type) {
        if (b->state != BUILDING_STATE_IN_USE) {
            continue;
        }
        if (b->desirability > 60) {
            b->upgrade_level = 3;
        } else if (b->desirability > 40) {
            b->upgrade_level = 2;
        } else if (b->desirability > 20) {
            b->upgrade_level = 1;
        } else {
            b->upgrade_level = 0;
        }
        map_building_tiles_add(b->id, b->x, b->y, 1, building_image_get(b), TERRAIN_BUILDING);
        if (map_terrain_is(b->grid_offset, TERRAIN_RESERVOIR_RANGE) && b->num_workers) {
            b->has_water_access = 1;
            map_terrain_add_with_radius(b->x, b->y, 1,
                map_water_supply_fountain_radius(), TERRAIN_FOUNTAIN_RANGE);
        } else {
            b->has_water_access = 0;
        }
    }
    // Ponds
    static const building_type ponds[] = { BUILDING_SMALL_POND, BUILDING_LARGE_POND };
    for (int i = 0; i < 2; i++) {
        for (building *b = building_first_of_type(ponds[i]); b; b = b->next_of_type) {
            if (b->state != BUILDING_STATE_IN_USE) {
                continue;
            }
            if (map_terrain_exists_tile_in_area_with_type(b->x, b->y, b->size, TERRAIN_RESERVOIR_RANGE)) {
                b->has_water_access = 1;
            } else {
                b->has_water_access = 0;
            }
            map_building_tiles_add(b->id, b->x, b->y, b->size, building_image_get(b), TERRAIN_BUILDING);
        }
    }

    // Large statues, to check if they should play the animation
    for (building *b = building_first_of_type(BUILDING_LARGE_STATUE); b; b = b->next_of_type) {
        if (b->state != BUILDING_STATE_IN_USE) {
            continue;
        }
        if (map_terrain_exists_tile_in_area_with_type(b->x, b->y, b->size, TERRAIN_RESERVOIR_RANGE)) {
            b->has_water_access = 1;
        } else {
            b->has_water_access = 0;
        }
    }

    for (building *b = building_first_of_type(BUILDING_COLOSSEUM); b; b = b->next_of_type) {
        if (b->state != BUILDING_STATE_IN_USE) {
            continue;
        }
        if (map_terrain_exists_tile_in_area_with_type(b->x, b->y, b->size, TERRAIN_RESERVOIR_RANGE)) {
            b->has_water_access = 1;
        } else {
            b->has_water_access = 0;
        }
    }
}

int map_water_supply_is_well_unnecessary(int well_id, int radius)
{
    building *well = building_get(well_id);
    int num_houses = 0;
    int x_min, y_min, x_max, y_max;
    map_grid_get_area(well->x, well->y, 1, radius, &x_min, &y_min, &x_max, &y_max);

    for (int yy = y_min; yy <= y_max; yy++) {
        for (int xx = x_min; xx <= x_max; xx++) {
            int grid_offset = map_grid_offset(xx, yy);
            int building_id = map_building_at(grid_offset);
            if (building_id && building_get(building_id)->house_size) {
                num_houses++;
                if (!map_terrain_is(grid_offset, TERRAIN_FOUNTAIN_RANGE)) {
                    return WELL_NECESSARY;
                }
            }
        }
    }
    return num_houses ? WELL_UNNECESSARY_FOUNTAIN : WELL_UNNECESSARY_NO_HOUSES;
}

int map_water_supply_fountain_radius(void)
{
    int radius = scenario_property_climate() == CLIMATE_DESERT ? FOUNTAIN_RADIUS - 1 : FOUNTAIN_RADIUS;
    if (building_monument_working(BUILDING_GRAND_TEMPLE_NEPTUNE)) {
        radius++;
    }

    return radius;
}

int map_water_supply_reservoir_radius(void)
{
    int radius = RESERVOIR_RADIUS;
    if (building_monument_working(BUILDING_GRAND_TEMPLE_NEPTUNE)) {
        radius += 2;
    }

    return radius;
}

int map_water_supply_well_radius(void)
{
    int radius = WELL_RADIUS;
    if (building_monument_working(BUILDING_GRAND_TEMPLE_NEPTUNE)) {
        radius++;
    }

    return radius;
}
