
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "mapgenerator.h"
#include "noise/module/perlin.h"
#include "map/data.h"
#include "map/terrain.h"
#include "map/grid.h"
#include "map/tiles.h"
#include "scenario/scenario.h"
#include "building/construction_routed.h"
#include "map/image_context.h"

#define TERRAIN_PAINT_MASK ~(TERRAIN_TREE | TERRAIN_ROCK | TERRAIN_WATER | TERRAIN_BUILDING |\
                            TERRAIN_SHRUB | TERRAIN_GARDEN | TERRAIN_ROAD | TERRAIN_MEADOW)

static struct {
    int seed;
    int water_direction;
} data;

static double dist_from_axis(double x, int modifier) {
    return fabs(x) / modifier;
}

static double clamp(double lowerBound, double upperBound, double value) {
    if (value < lowerBound) {
        return lowerBound;
    } else if (value > upperBound) {
        return upperBound;
    }
    return value;
}

static double add(double value1, double value2) {
    return value1 + value2;
}

double multiply(double value, double value2) {
    return value
           * value2;
}

double bias(double bias, double scale, double value) {
    return value * bias + scale;
}

static void create_open_water() {
    perlin_setup(0.029999999, 2.0, 0.5, 3, data.seed, 1);
    int dist_modifier = 40;
    int i = map_data.width;
    for (int z = 0; z < i; z++) {
        int grid_offset = map_data.start_offset;
        int height = map_data.height;
        int borderSize = map_data.border_size;
        for (int y = 0; y < height; y++, grid_offset += borderSize) {
            int width = map_data.width;
            for (int x = 0; x < width; x++, grid_offset++) {
                double value = 0;
                switch (data.water_direction) {
                    case 0: // North
                        value = clamp(-1.0, 2.5,
                                      bias(1.0, -1.0, dist_from_axis(x + y, dist_modifier)));
                        break;
                    case 1: // South to east
                        value = clamp(-1.0, 2.5,
                                      bias(1.0, -1.0, dist_from_axis(x - map_data.height, dist_modifier)));
                        break;
                    case 2: // North to east
                        value = clamp(-1.0, 2.5,
                                      bias(1.0, -1.0, dist_from_axis(y, dist_modifier)));
                        break;
                    case 3: // West to east
                        value = clamp(-1.0, 2.5,
                                      bias(1.0, -1.0, dist_from_axis(y - map_data.width, dist_modifier)));
                        break;
                    case 4: // North to west
                        value = clamp(-1.0, 2.5, bias(1.0, -1.0, dist_from_axis(x, dist_modifier)));
                        break;
                    case 5: // side to side
                        value = clamp(-1.0, 2.5,
                                      bias(1.0, -1.0, dist_from_axis(y - map_data.width, dist_modifier)));
                        break;
                }
                double noise = add(bias(0.5, 0.5, perlin_get_value(x, y, z)),
                                   value);
                if (data.water_direction == 0) {
                    if (noise < -0.15) {
                        map_terrain_add(grid_offset, TERRAIN_WATER);
                    }
                } else {
                    if (noise < -0.45) {
                        map_terrain_add(grid_offset, TERRAIN_WATER);
                    }
                }
            }
        }
    }
    map_image_context_reset_water();
    map_tiles_update_all_water();
}

static void set_trees(int grid_offset) {
    int terrain = 0;
    terrain &= TERRAIN_PAINT_MASK;
    terrain |= TERRAIN_TREE;
    map_terrain_set(grid_offset, terrain);
}

static void set_shrub(int grid_offset) {
    int terrain = 0;
    terrain &= TERRAIN_PAINT_MASK;
    terrain |= TERRAIN_SHRUB;
    map_terrain_set(grid_offset, terrain);
}

void create_road() {
    int placed_entry = 0;
    int placed_exit = 0;
    int target_x = map_data.width / 2 + rand() % 10;
    int target_y = map_data.height / 2 + rand() % 10;
    for (int z = 0; z < map_data.width; z++) {
        int grid_offset = map_data.start_offset;
        for (int y = 0; y < map_data.height; y++, grid_offset += map_data.border_size) {
            for (int x = 0; x < map_data.width; x++, grid_offset++) {
                switch (data.water_direction) {
                    case 0:
                    case 2:
                    case 3:
                        // Create road from north to south
                        if (x == 0 && target_y == y && !placed_entry) {
                            scenario.entry_point.x = x;
                            scenario.entry_point.y = y;
                            placed_entry = 1;
                        }
                        if (x == map_grid_width() - 1 && target_y == y && !placed_exit) {
                            scenario.exit_point.x = x;
                            scenario.exit_point.y = y;
                            placed_exit = 1;
                        }
                        break;
                    case 1:
                    case 4:
                    case 5:
                        // Create road from west to east
                        if (y == 0 && target_x == x && !placed_entry) {
                            scenario.entry_point.x = x;
                            scenario.entry_point.y = y;
                            placed_entry = 1;
                        }
                        if (y == map_grid_height() - 1 && target_x == x && !placed_exit) {
                            scenario.exit_point.x = x;
                            scenario.exit_point.y = y;
                            placed_exit = 1;
                        }
                        break;
                }
            }
        }
    }
    building_mapgen_place_road(scenario.entry_point.x, scenario.entry_point.y, scenario.exit_point.x,
                               scenario.exit_point.y);
}

static void create_rock_trees(double when_to_add_rock, int z, int grid_offset, int y, int x) {
    double value = multiply(bias(0.5, 0.5, perlin_get_value(x, y, z)), 0.8);
    if (map_grid_is_valid_offset(grid_offset)) {
        int terrain = map_terrain_get(grid_offset);
        if (terrain != TERRAIN_WATER && terrain != TERRAIN_ROAD) {
            if (value >= when_to_add_rock) {
                terrain &= TERRAIN_PAINT_MASK;
                terrain = TERRAIN_ROCK;
                map_terrain_set(grid_offset, terrain);
            }
        }
        if (terrain == 0) {
            if (value >= 0.65 && value < 0.68) {
                set_shrub(grid_offset);
            } else if (value >= 0.64) {
                set_trees(grid_offset);
            }
        }
    }
}

static void create_meadow(int z, int grid_offset, int y, int x) {
    double value = multiply(bias(0.5, 0.3, perlin_get_value(x, y, z)), 0.8);
    if (map_grid_is_valid_offset(grid_offset)) {
        int terrain = map_terrain_get(grid_offset);
        if (value >= 0.50) {
            if (terrain & TERRAIN_MEADOW) {
                set_trees(grid_offset);
            }
        }
        terrain = map_terrain_get(grid_offset);
        if (terrain == 0 && (value >= 0.45)) {
            terrain &= TERRAIN_PAINT_MASK;
            terrain |= TERRAIN_MEADOW;
            map_terrain_set(grid_offset, terrain);
        }
    }
}

static void create_noise_pass_1() {
    perlin_setup(0.021, 2.0, 0.5, 6, data.seed, 2);
    double when_to_add_rock = 0.70;
    if (map_data.width >= 120) {
        when_to_add_rock = 0.80;
    }
    for (int z = 0; z < map_data.width; z++) {
        int grid_offset = map_data.start_offset;
        for (int y = 0; y < map_data.height; y++, grid_offset += map_data.border_size) {
            for (int x = 0; x < map_data.width; x++, grid_offset++) {
                create_rock_trees(when_to_add_rock, z, grid_offset, y, x);
            }
        }
    }
}

static void create_noise_pass_2() {
    perlin_setup(0.020, 1.0, 0.5, 5, data.seed + 1000, 1);
    for (int z = 0; z < map_data.width; z++) {
        int grid_offset = map_data.start_offset;
        for (int y = 0; y < map_data.height; y++, grid_offset += map_data.border_size) {
            for (int x = 0; x < map_data.width; x++, grid_offset++) {
                create_meadow(z, grid_offset, y, x);
            }
        }
    }
}

void noise_fill_map(int seed) {
    data.seed = seed;
    data.water_direction = data.seed % 5;
    create_road();
    create_open_water();
    create_noise_pass_1();
    create_noise_pass_2();

    map_tiles_update_all_rocks();
    map_tiles_update_all_trees();
    map_tiles_update_all_shrubs();
}

