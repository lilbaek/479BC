#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "map_generator.h"
#include "graphics/window.h"
#include "graphics/graphics.h"
#include "graphics/image.h"
#include "assets/assets.h"
#include "ui_window.h"
#include "nuklear.h"
#include "graphics/screen.h"
#include "translation/translation.h"
#include "icons.h"
#include "game/game.h"
#include "sound/music.h"
#include "city.h"

static struct {
    char *map_size[6];
    int current_map;
    char *seed;
    int init;
} data;

void init() {
    data.map_size[0] = gettext("Tiny map");
    data.map_size[1] = gettext("Small map");
    data.map_size[2] = gettext("Medium map");
    data.map_size[3] = gettext("Large map");
    data.map_size[4] = gettext("Huge map");
    data.map_size[5] = gettext("Enormous map");
    data.current_map = 4;
    data.init = 1;
}

static void draw_background(void) {
    graphics_reset_dialog();
    graphics_reset_clip_rectangle();
    image_draw_fullscreen_background(assets_get_image_id("mainmenu", "start-screen"));
}

static void add_header_row(struct nk_context *ctx, char *seed, int *seed_len) {
    nk_layout_row_begin(ctx, NK_DYNAMIC, 25, 5);
    nk_layout_row_push(ctx, 0.01f);
    nk_spacer(ctx);
    nk_layout_row_push(ctx, 0.25f);
    data.current_map = nk_combo(ctx, (const char **) data.map_size, NK_LEN(data.map_size), data.current_map, 25,
                                nk_vec2(200, 200));
    nk_layout_row_push(ctx, 0.33f);
    nk_spacer(ctx);
    nk_layout_row_push(ctx, 0.1f);
    nk_label(ctx, gettext("Seed"), NK_TEXT_RIGHT);
    nk_layout_row_push(ctx, 0.2f);
    nk_edit_string(ctx, NK_EDIT_SIMPLE, seed, seed_len, 10, nk_filter_decimal);
    nk_layout_row_push(ctx, 0.1f);
    ui_font_icons();
    if (nk_button_label(ctx, ICON_FA_RANDOM)) {
        int number = 543354323 - (rand() % 2000);
        sprintf(seed, "%d", number);
    }
    nk_layout_row_push(ctx, 0.01f);
    nk_spacer(ctx);
    nk_layout_row_end(ctx);
}

static void draw_foreground(void) {
    struct nk_context *ctx = ui_context();
    int w = 777;
    int h = 800;
    static char seed[10];
    static int seed_len;
    if (data.init) {
        srand(time(NULL));
        int number = 543354323 - (rand() % 2000);
        sprintf(seed, "%d", number);
        seed_len = 10;
        data.init = 0;
    }
    if (nk_begin(ctx, gettext("Map generator"),
                 nk_recti((screen_width() / 2) - w / 2, (screen_height() / 2) - h / 2, w, h),
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE)) {
        add_header_row(ctx, seed, &seed_len);

        nk_layout_row_dynamic(ctx, 35, 4);
        if (nk_button_label(ctx, gettext("Play"))) {
            game_create_random_scenario(data.current_map, atoi(seed));
            sound_music_update(1);
            window_city_show();
        }
    }
    nk_end(ctx);
    ui_font_standard();
}

static void handle_input(const mouse *m, const hotkeys *h) {
    if (h->escape_pressed) {
        hotkey_handle_escape();
    }
}

void window_map_generator_show() {
    init();
    window_type window = {
            WINDOW_MAP_GENERATOR,
            draw_background,
            draw_foreground,
            handle_input
    };
    window_show(&window);
}
