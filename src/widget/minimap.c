#include "minimap.h"

#include "assets/assets.h"
#include "building/building.h"
#include "building/monument.h"
#include "city/view.h"
#include "core/calc.h"
#include "figure/figure.h"
#include "figure/formation.h"
#include "graphics/graphics.h"
#include "graphics/image.h"
#include "graphics/renderer.h"
#include "map/building.h"
#include "map/figure.h"
#include "map/grid.h"
#include "map/property.h"
#include "map/random.h"
#include "map/terrain.h"
#include "scenario/property.h"

#include <stdlib.h>
#include <string.h>

enum {
    FIGURE_COLOR_NONE = 0,
    FIGURE_COLOR_SOLDIER = 1,
    FIGURE_COLOR_SELECTED_SOLDIER = 2,
    FIGURE_COLOR_ENEMY = 3,
    FIGURE_COLOR_WOLF = 4
};

typedef struct {
    color_t left;
    color_t right;
} tile_color;

typedef struct {
    scenario_climate(*climate)(void);
    building *(*building)(int id);
    struct {
        int (*width)(void);
        int (*height)(void);
    } map;
    struct {
        int (*figure)(int grid_offset, int (*callback)(figure *f));
        int (*terrain)(int grid_offset);
        int (*building_id)(int grid_offset);
        int (*is_draw_tile)(int grid_offset);
        int (*tile_size)(int grid_offset);
        int (*random)(int grid_offset);
    } offset;
    void (*viewport)(int *x, int *y, int *width, int *height);
} minimap_functions;

static void get_viewport(int *x, int *y, int *width, int *height);

static minimap_functions functions = {
    .climate = scenario_property_climate,
    .map.width = map_grid_width,
    .map.height = map_grid_height,
    .offset.figure = map_figure_foreach_until,
    .offset.terrain = map_terrain_get,
    .offset.building_id = map_building_at,
    .offset.is_draw_tile = map_property_is_draw_tile,
    .offset.tile_size = map_property_multi_tile_size,
    .offset.random = map_random_get,
    .building = building_get,
    .viewport = get_viewport
};

typedef struct {
    color_t soldier;
    color_t selected_soldier;
    color_t enemy;
    color_t wolf;
    tile_color water[4];
    tile_color tree[4];
    tile_color rock[4];
    tile_color meadow[4];
    tile_color grass[8];
    tile_color road;
    tile_color wall;
    tile_color aqueduct;
    tile_color reservoir[2];
    tile_color house[2];
    tile_color building[2];
    tile_color monument[2];
} tile_color_set;

// Since the minimap tiles are only 25 color sets per climate, we just hardcode them.
static const tile_color_set MINIMAP_COLOR_SETS[3] = {
    // central
    {
        .soldier = COLOR_MINIMAP_SOLDIER,
        .selected_soldier = COLOR_MINIMAP_SELECTED_SOLDIER,
        .enemy = COLOR_MINIMAP_ENEMY_CENTRAL,
        .wolf = COLOR_MINIMAP_WOLF,
        .water = {{0xff394a7b, 0xff31427b}, {0xff394a7b, 0xff314273}, {0xff313973, 0xff314273}, {0xff31427b, 0xff394a7b}},
        .tree = {{0xff6b8431, 0xff102108}, {0xff103908, 0xff737b29}, {0xff103108, 0xff526b21}, {0xff737b31, 0xff084a10}},
        .rock = {{0xff948484, 0xff635a4a}, {0xffa59c94, 0xffb5ada5}, {0xffb5ada5, 0xff8c8484}, {0xff635a4a, 0xffa59c94}},
        .meadow = {{0xffd6bd63, 0xff9c8c39}, {0xff948c39, 0xffd6bd63}, {0xffd6bd63, 0xff9c9439}, {0xff848431, 0xffada54a}},
        .grass = {
            {0xff6b8c31, 0xff6b7b29}, {0xff738431, 0xff6b7b29}, {0xff6b7329, 0xff7b8c39}, {0xff527b29, 0xff6b7321},
            {0xff6b8431, 0xff737b31}, {0xff6b7b31, 0xff737b29}, {0xff636b18, 0xff526b21}, {0xff737b31, 0xff737b29}
        },
        .road = {0xff736b63, 0xff4a3121},
        .wall = {0xffd6d3c6, 0xfff7f3de},
        .aqueduct = {0xff5282bd, 0xff84baff},
        .reservoir = {{0xff5282bd, 0xff5282bd}, {0xff84baff, 0xff84baff}}, // Edges, center
        .house = {{0xffffb28c, 0xffd65110}, {0xffef824a, 0xffffa273}}, // Edges, center
        .building = {{0xfffffbde, 0xffefd34a}, {0xfffff3c6, 0xffffebb5}}, // Edges, center
        .monument = {{0xfff5deff, 0xffb84aef}, {0xffe9c6ff, 0xffdfb5ff}} // Edges, center
    },
    // northern
    {
        .soldier = COLOR_MINIMAP_SOLDIER,
        .selected_soldier = COLOR_MINIMAP_SELECTED_SOLDIER,
        .enemy = COLOR_MINIMAP_ENEMY_NORTHERN,
        .wolf = COLOR_MINIMAP_WOLF,
        .water = {{0xff394a7b, 0xff31427b}, {0xff394a7b, 0xff314273}, {0xff313973, 0xff314273}, {0xff31427b, 0xff394a7b}},
        .tree = {{0xff527b31, 0xff082108}, {0xff083908, 0xff5a7329}, {0xff082908, 0xff316b21}, {0xff527b29, 0xff084a21}},
        .rock = {{0xff8c8484, 0xff5a5252}, {0xff9c9c94, 0xffa5a5a5}, {0xffa5a5a5, 0xff848484}, {0xff5a5252, 0xff9c9c94}},
        .meadow = {{0xff427318, 0xff8c9442}, {0xffb5ad4a, 0xff738c39}, {0xff8c8c39, 0xff6b7b29}, {0xff527331, 0xff5a8442}},
        .grass = {
            {0xff4a8431, 0xff4a7329}, {0xff527b29, 0xff4a7329}, {0xff526b29, 0xff5a8439}, {0xff397321, 0xff4a6b21},
            {0xff527b31, 0xff5a7331}, {0xff4a7329, 0xff5a7329}, {0xff4a6b18, 0xff316b21}, {0xff527b29, 0xff527329}
        },
        .road = {0xff736b63, 0xff4a3121},
        .wall = {0xffd6d3c6, 0xfff7f3de},
        .aqueduct = {0xff5282bd, 0xff84baff},
        .reservoir = {{0xff5282bd, 0xff5282bd}, {0xff84baff, 0xff84baff}}, // Edges, center
        .house = {{0xffffb28c, 0xffd65110}, {0xffef824a, 0xffffa273}}, // Edges, center
        .building = {{0xfffffbde, 0xffefd34a}, {0xfffff3c6, 0xffffebb5}}, // Edges, center
        .monument = {{0xfff5deff, 0xffb84aef}, {0xffe9c6ff, 0xffdfb5ff}} // Edges, center
    },
    // desert
    {
        .soldier = COLOR_MINIMAP_SOLDIER,
        .selected_soldier = COLOR_MINIMAP_SELECTED_SOLDIER,
        .enemy = COLOR_MINIMAP_ENEMY_DESERT,
        .wolf = COLOR_MINIMAP_WOLF,
        .water = {{0xff4a84c6, 0xff4a7bc6}, {0xff4a84c6, 0xff4a7bc6}, {0xff4a84c6, 0xff5284c6}, {0xff4a7bbd, 0xff4a7bc6}},
        .tree = {{0xffa59c7b, 0xff6b7b18}, {0xff214210, 0xffada573}, {0xff526b21, 0xffcec6a5}, {0xffa59c7b, 0xff316321}},
        .rock = {{0xffa59494, 0xff736352}, {0xffa59c94, 0xffb5ada5}, {0xffb5ada5, 0xff8c847b}, {0xff736352, 0xffbdada5}},
        .meadow = {{0xff739c31, 0xff9cbd52}, {0xff7bb529, 0xff63ad21}, {0xff9cbd52, 0xff8c944a}, {0xff7ba539, 0xff739c31}},
        .grass = {
            {0xffbdbd9c, 0xffb5b594}, {0xffc6bda5, 0xffbdbda5}, {0xffbdbd9c, 0xffc6c6ad}, {0xffd6cead, 0xffc6bd9c},
            {0xffa59c7b, 0xffbdb594}, {0xffcecead, 0xffb5ad94}, {0xffc6c6a5, 0xffdedebd}, {0xffcecead, 0xffd6d6b5}
        },
        .road = {0xff6b5a52, 0xff4a4239},
        .wall = {0xffd6d3c6, 0xfff7f3de},
        .aqueduct = {0xff5282bd, 0xff84baff},
        .reservoir = {{0xff5282bd, 0xff5282bd}, {0xff84baff, 0xff84baff}}, // Edges, center
        .house = {{0xffffb28c, 0xffd65110}, {0xffef824a, 0xffffa273}}, // Edges, center
        .building = {{0xfffffbde, 0xffefd34a}, {0xfffff3c6, 0xffffebb5}}, // Edges, center
        .monument = {{0xfff5deff, 0xffb84aef}, {0xffe9c6ff, 0xffdfb5ff}} // Edges, center
    }
};

static struct {
    struct {
        int x;
        int y;
        int width;
        int height;
        int width_tiles;
        int height_tiles;
    } screen;
    struct {
        int x;
        int y;
        int width;
        int height;
        int offset_x;
        int offset_y;
        float scale;
        float max_scale;
    } minimap;
    struct {
        int stride;
        color_t *buffer;
    } cache;
    const minimap_functions *functions;
    struct {
        int x;
        int y;
        int grid_offset;
    } mouse;
    int refresh_requested;
    struct {
        int x;
        int y;
        int width;
        int height;
    } viewport;
} data = {
    .functions = &functions
};

static void get_viewport(int *x, int *y, int *width, int *height)
{
    city_view_get_camera(x, y);
    city_view_get_viewport_size_tiles(width, height);
}

void widget_minimap_invalidate(void)
{
    data.refresh_requested = 1;
}

static void foreach_map_tile(map_callback *callback)
{
    city_view_foreach_minimap_tile(0, 0, data.minimap.x, data.minimap.y,
        data.minimap.width, data.minimap.height, callback);
}

static void set_bounds(int x_offset, int y_offset, int width, int height)
{
    data.screen.x = x_offset;
    data.screen.y = y_offset;
    data.screen.width = width;
    data.screen.height = height;
    data.screen.width_tiles = width / 2;
    data.screen.height_tiles = height;

    float max_scale_width = data.minimap.width * 2 / (float) data.screen.width;
    float max_scale_height = data.minimap.height / (float) data.screen.height;

    data.minimap.max_scale = max_scale_width > max_scale_height ? max_scale_width : max_scale_height;
}

static void position_minimap(void)
{
    if (data.functions->viewport) {
        data.functions->viewport(&data.viewport.x, &data.viewport.y, &data.viewport.width, &data.viewport.height);

        float scale_width = data.viewport.width / (float) data.screen.width_tiles;
        float scale_height = data.viewport.height / (float) data.screen.height_tiles;

        data.minimap.scale = scale_width > scale_height ? scale_width : scale_height;

        if (data.minimap.scale > data.minimap.max_scale) {
            data.minimap.scale = data.minimap.max_scale;
        }
    }

    if (data.minimap.scale < 1.0f) {
        data.minimap.scale = SCALE_NONE;
    }

    int adjusted_camera_x = (data.viewport.x - data.minimap.x) * 2;
    int adjusted_camera_y = data.viewport.y - data.minimap.y;
    int minimap_width_pixels = (int) ((data.minimap.width * 2) / data.minimap.scale);
    int minimap_height_pixels = (int) (data.minimap.height / data.minimap.scale);

    data.minimap.offset_x = (minimap_width_pixels - data.screen.width) / 2;
    data.minimap.offset_y = (minimap_height_pixels - data.screen.height) / 2;

    if (data.functions->viewport) {
        if (minimap_width_pixels > data.screen.width) {
            if (data.minimap.offset_x > adjusted_camera_x) {
                data.minimap.offset_x = adjusted_camera_x;
                if (data.minimap.offset_x < 0) {
                    data.minimap.offset_x = 0;
                }
            } else if ((adjusted_camera_x + data.viewport.width * 2) / data.minimap.scale >
                data.minimap.offset_x + data.screen.width) {
                data.minimap.offset_x = (int) ((adjusted_camera_x + data.viewport.width * 2) / data.minimap.scale -
                    data.screen.width);
            }
        }
        if (minimap_height_pixels > data.screen.height) {
            if (data.minimap.offset_y > adjusted_camera_y) {
                data.minimap.offset_y = adjusted_camera_y;
                if (data.minimap.offset_y < 0) {
                    data.minimap.offset_y = 0;
                }
            } else if ((adjusted_camera_y + data.viewport.height) / data.minimap.scale >
                data.minimap.offset_y + data.screen.height) {
                data.minimap.offset_y = (int) ((adjusted_camera_y + data.viewport.height) / data.minimap.scale -
                    data.screen.height);
            }
        }
    }
}

static int has_figure_color(figure *f)
{
    int type = f->type;
    if (figure_is_legion(f)) {
        return formation_get_selected() == f->formation_id ?
            FIGURE_COLOR_SELECTED_SOLDIER : FIGURE_COLOR_SOLDIER;
    }
    if (figure_is_enemy(f)) {
        return FIGURE_COLOR_ENEMY;
    }
    if (f->type == FIGURE_INDIGENOUS_NATIVE &&
        f->action_state == FIGURE_ACTION_159_NATIVE_ATTACKING) {
        return FIGURE_COLOR_ENEMY;
    }
    if (type == FIGURE_WOLF) {
        return FIGURE_COLOR_WOLF;
    }
    return FIGURE_COLOR_NONE;
}

static inline void draw_pixel(int x, int y, color_t color)
{
    data.cache.buffer[y * data.cache.stride + x] = color;
}

static inline void draw_tile(int x_offset, int y_offset, const tile_color *colors)
{
    draw_pixel(x_offset, y_offset, colors->left);
    draw_pixel(x_offset + 1, y_offset, colors->right);
}

static int draw_figure(int x_view, int y_view, int grid_offset, const tile_color_set *set)
{
    if (!data.functions->offset.figure) {
        return 0;
    }
    int color_type = data.functions->offset.figure(grid_offset, has_figure_color);
    if (color_type == FIGURE_COLOR_NONE) {
        return 0;
    }
    color_t color = set->wolf;
    if (color_type == FIGURE_COLOR_SOLDIER) {
        color = set->soldier;
    } else if (color_type == FIGURE_COLOR_SELECTED_SOLDIER) {
        color = set->selected_soldier;
    } else if (color_type == FIGURE_COLOR_ENEMY) {
        color = set->enemy;
    }
    draw_pixel(x_view, y_view, color);
    draw_pixel(x_view + 1, y_view, color);
    return 1;
}

static void draw_building(int size, int x_offset, int y_offset, const tile_color *colors)
{
    if (size == 1) {
        draw_tile(x_offset, y_offset, &colors[1]);
        return;
    }
    int width = size * 2;
    int height = width - 1;
    y_offset -= size - 1;
    int start_y = y_offset < 0 ? -y_offset : 0;
    int end_y = height / 2 + 1;

    for (int y = start_y; y < end_y; y++) {
        int x_start = height / 2 - y;
        int x_end = width - x_start - 1;
        draw_pixel(x_start + x_offset, y + y_offset, colors[1].left);
        draw_pixel(x_end + x_offset, y + y_offset, colors[1].right);
        if (x_start + x_offset < 0) {
            x_start = -x_offset - 1;
        }
        color_t *value = &data.cache.buffer[(y_offset + y) * data.cache.stride + x_start + x_offset + 1];
        for (int x = x_start; x < x_end - 1; x++) {
            *value++ = ((x + y) & 1) ? colors[0].left : colors[0].right;
        }
    }
    y_offset += height / 2 + 1;
    start_y = y_offset < 0 ? -y_offset : 0;
    end_y = height / 2;

    for (int y = start_y; y < end_y; y++) {
        int x_start = y + 1;
        int x_end = width - x_start - 1;
        draw_pixel(x_start + x_offset, y + y_offset, colors[1].left);
        draw_pixel(x_end + x_offset, y + y_offset, colors[1].right);
        if (x_start + x_offset < 0) {
            x_start = -x_offset - 1;
        }
        color_t *value = &data.cache.buffer[(y_offset + y) * data.cache.stride + x_start + x_offset + 1];
        for (int x = x_start; x < x_end - 1; x++) {
            *value++ = ((x + y) & 1) ? colors[0].right : colors[0].left;
        }
    }
}

static void draw_minimap_tile(int x_view, int y_view, int grid_offset)
{
    const tile_color_set *set = &MINIMAP_COLOR_SETS[data.functions->climate()];

    if (grid_offset < 0) {
        return;
    }

    if (draw_figure(x_view, y_view, grid_offset, set)) {
        return;
    }
    int terrain = data.functions->offset.terrain(grid_offset);
    // exception for fort ground: display as empty land
    if (terrain & TERRAIN_BUILDING && data.functions->building) {
        if (data.functions->building(data.functions->offset.building_id(grid_offset))->type == BUILDING_FORT_GROUND) {
            terrain = 0;
        }
    }

    if (terrain & TERRAIN_BUILDING) {
        if (data.functions->offset.is_draw_tile(grid_offset)) {
            const tile_color *colors;
            if (data.functions->building) {
                building *b = data.functions->building(data.functions->offset.building_id(grid_offset));
                if (b->house_size) {
                    colors = set->house;
                } else if (b->type == BUILDING_RESERVOIR) {
                    colors = set->reservoir;
                } else if (building_monument_is_monument(b)) {
                    colors = set->monument;
                } else {
                    colors = set->building;
                }
            } else {
                colors = set->building;
            }
            int size = data.functions->offset.tile_size(grid_offset);
            draw_building(size, x_view, y_view, colors);
        }
        return;
    }
    int rand = data.functions->offset.random(grid_offset);
    const tile_color *colors;
    if (terrain & TERRAIN_ROAD) {
        colors = &set->road;
    } else if (terrain & TERRAIN_WATER) {
        colors = &set->water[rand & 3];
    } else if (terrain & (TERRAIN_SHRUB | TERRAIN_TREE)) {
        colors = &set->tree[rand & 3];
    } else if (terrain & (TERRAIN_ROCK | TERRAIN_ELEVATION)) {
        colors = &set->rock[rand & 3];
    } else if (terrain & TERRAIN_AQUEDUCT) {
        colors = &set->aqueduct;
    } else if (terrain & TERRAIN_WALL) {
        colors = &set->wall;
    } else if (terrain & TERRAIN_MEADOW) {
        colors = &set->meadow[rand & 3];
    } else {
        colors = &set->grass[rand & 7];
    }
    draw_tile(x_view, y_view, colors);
}

static void draw_viewport_rectangle(void)
{
    if (!data.functions->viewport) {
        return;
    }
    int x_offset = (int) ((2 * (data.viewport.x - data.minimap.x) - 2 / 30) / data.minimap.scale);
    x_offset += data.screen.x - data.minimap.offset_x;
    if (x_offset < data.screen.x) {
        x_offset = data.screen.x;
    }
    if (x_offset + 2 * data.viewport.width + 4 > data.screen.x + data.screen.width) {
        x_offset -= 2;
    }
    int y_offset = (int) ((data.viewport.y - data.minimap.y + 2) / data.minimap.scale);
    y_offset += data.screen.y - data.minimap.offset_y;
    graphics_draw_rect(x_offset, y_offset,
        (int) ((data.viewport.width * 2) / data.minimap.scale + 4),
        (int) (data.viewport.height / data.minimap.scale - 4),
        COLOR_MINIMAP_VIEWPORT);
}

static void prepare_minimap_cache(void)
{
    if (data.functions->map.width() != data.minimap.width || data.functions->map.height() * 2 != data.minimap.height ||
        !graphics_renderer()->has_custom_image(CUSTOM_IMAGE_MINIMAP)) {
        data.minimap.width = data.functions->map.width();
        data.minimap.height = data.functions->map.height() * 2;
        data.minimap.x = (VIEW_X_MAX - data.minimap.width) / 2;
        data.minimap.y = (VIEW_Y_MAX - data.minimap.height) / 2;

        graphics_renderer()->create_custom_image(CUSTOM_IMAGE_MINIMAP, data.minimap.width * 2, data.minimap.height, 0);
    }
    data.cache.buffer = graphics_renderer()->get_custom_image_buffer(CUSTOM_IMAGE_MINIMAP, &data.cache.stride);
}

static void clear_minimap(void)
{
    memset(data.cache.buffer, 0, data.minimap.height * data.cache.stride * sizeof(color_t));
}

void widget_minimap_draw(int x_offset, int y_offset, int width, int height, int force)
{
    graphics_set_clip_rectangle(x_offset, y_offset, width, height);
    graphics_fill_rect(x_offset, y_offset, width, height, COLOR_BLACK);

    if (data.refresh_requested || force) {
        prepare_minimap_cache();
        if (!data.cache.buffer) {
            return;
        }
        clear_minimap();
        set_bounds(x_offset, y_offset, width, height);
        foreach_map_tile(draw_minimap_tile);
        graphics_renderer()->update_custom_image(CUSTOM_IMAGE_MINIMAP);
        data.refresh_requested = 0;
    }

    position_minimap();
    graphics_renderer()->draw_custom_image(CUSTOM_IMAGE_MINIMAP,
        (int) ((data.screen.x - data.minimap.offset_x) * data.minimap.scale),
        (int) ((data.screen.y - data.minimap.offset_y) * data.minimap.scale), data.minimap.scale);
    draw_viewport_rectangle();
    graphics_reset_clip_rectangle();
    
    graphics_draw_line(x_offset - 1, x_offset - 1 + width, y_offset - 1, y_offset - 1, COLOR_MINIMAP_DARK);
    graphics_draw_line(x_offset - 1, x_offset - 1, y_offset, y_offset + height, COLOR_MINIMAP_DARK);
    graphics_draw_line(x_offset - 1 + width, x_offset - 1 + width, y_offset,
        y_offset + height, COLOR_MINIMAP_LIGHT);
}

static void update_mouse_grid_offset(int x_view, int y_view, int grid_offset)
{
    if (data.mouse.y == y_view && (data.mouse.x == x_view || data.mouse.x == x_view + 1)) {
        data.mouse.grid_offset = grid_offset < 0 ? 0 : grid_offset;
    }
}

static int get_mouse_grid_offset(const mouse *m)
{
    data.mouse.x = (int) ((m->x - data.screen.x) * data.minimap.scale + data.minimap.offset_x);
    data.mouse.y = (int) ((m->y - data.screen.y) * data.minimap.scale + data.minimap.offset_y);
    data.mouse.grid_offset = 0;
    foreach_map_tile(update_mouse_grid_offset);
    return data.mouse.grid_offset;
}

static int is_in_minimap(const mouse *m)
{
    if (m->x >= data.screen.x && m->x < data.screen.x + data.screen.width &&
        m->y >= data.screen.y && m->y < data.screen.y + data.screen.height) {
        return 1;
    }
    return 0;
}

int widget_minimap_handle_mouse(const mouse *m)
{
    if ((m->left.went_down || m->right.went_down) && is_in_minimap(m)) {
        int grid_offset = get_mouse_grid_offset(m);
        if (grid_offset > 0) {
            city_view_go_to_grid_offset(grid_offset);
            return 1;
        }
    }
    return 0;
}
