#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#include "settings_gameplay.h"
#include "graphics/image.h"
#include "assets/assets.h"
#include "input/mouse.h"
#include "input/hotkey.h"
#include "graphics/window.h"
#include "core/config.h"
#include "ui_window.h"
#include "nuklear.h"
#include "translation/translation.h"
#include "graphics/screen.h"

static struct {
    struct {
        int original_value;
        int new_value;
        int changed;
    } config_values[CONFIG_MAX_ENTRIES];
} settings_gameplay;

static void draw_background(void) {
    image_draw_fullscreen_background(assets_get_image_id("mainmenu", "start-screen"));
}

static void cancel_values(void) {
    for (int i = 0; i < CONFIG_MAX_ENTRIES; i++) {
        settings_gameplay.config_values[i].new_value = settings_gameplay.config_values[i].original_value;
    }
}

static void handle_input(const mouse *m, const hotkeys *h) {
    if (h->escape_pressed) {
        cancel_values();
        window_go_back();
    }
}

void settings_gameplay_init() {
    for (int i = 0; i < CONFIG_MAX_ENTRIES; i++) {
        settings_gameplay.config_values[i].original_value = config_get(i);
        settings_gameplay.config_values[i].new_value = config_get(i);
    }
}

static void create_row(struct nk_context *ctx) {
    nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 2);
    nk_layout_row_push(ctx, 0.01f);
    nk_spacer(ctx);
    nk_layout_row_push(ctx, 0.99f);
}

static void save_changes() {
    // Save changes first
    for (int i = 0; i < CONFIG_MAX_ENTRIES; ++i) {
        if (settings_gameplay.config_values[i].original_value == settings_gameplay.config_values[i].new_value)
            continue;
        settings_gameplay.config_values[i].changed = 1;
        config_set(i, settings_gameplay.config_values[i].new_value);
        settings_gameplay.config_values[i].original_value = settings_gameplay.config_values[i].new_value;
    }
    config_save();
    window_go_back();
}

static void create_ok_button(struct nk_context *ctx) {
    struct nk_style_button button;
    button = ctx->style.button;
    ctx->style.button.normal.data.color = nk_rgb(156, 39, 176);
    ctx->style.button.hover.data.color = nk_rgb(164, 56, 182);
    ctx->style.button.text_normal = nk_rgb(255, 255, 255);
    ctx->style.button.text_hover = nk_rgb(255, 255, 255);
    ctx->style.button.text_active = nk_rgb(255, 255, 255);
    if (nk_button_label(ctx, gettext("Confirm"))) {
        save_changes();
    }
    ctx->style.button = button;
}

static void draw_foreground(void) {
    struct nk_context *ctx = ui_context();
    int w = 877;
    int h = 500;
    ui_font_change(FONT_TYPE_LARGE_BOLD);
    if (nk_begin_titled(ctx, "settings_gameplay", gettext("Gameplay Settings"),
                        nk_recti((screen_width() / 2) - w / 2, (screen_height() / 2) - h / 2, w, h),
                        NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE)) {
        ui_font_change(FONT_TYPE_STANDARD);
        nk_layout_space_begin(ctx, NK_STATIC, h - 40, 3);
        nk_layout_space_push(ctx, nk_rect(0, 0, 360, h - 105));
        if (nk_group_begin(ctx, "group_general", NK_WINDOW_BORDER)) {
            {
                ui_font_change(FONT_TYPE_LARGE_BOLD);;
                create_row(ctx);
                nk_label(ctx, gettext("General"), NK_TEXT_LEFT);
                ui_font_change(FONT_TYPE_STANDARD);
            }
            {
                nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 3);
                nk_layout_row_push(ctx, 0.01f);
                nk_spacer(ctx);
                nk_layout_row_push(ctx, 0.5f);
                nk_labelf(ctx, NK_TEXT_LEFT, gettext("Game speed: %u") , (int)settings_gameplay.config_values[CONFIG_GP_GAME_SPEED].new_value);
                nk_layout_row_push(ctx, 0.49f);
                nk_slider_int(ctx, 50, &settings_gameplay.config_values[CONFIG_GP_GAME_SPEED].new_value, 180, 5);
            }
            {
                nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 3);
                nk_layout_row_push(ctx, 0.01f);
                nk_spacer(ctx);
                nk_layout_row_push(ctx, 0.5f);
                nk_labelf(ctx, NK_TEXT_LEFT, gettext("Scroll speed: %u") , (int)settings_gameplay.config_values[CONFIG_GP_SCROLL_SPEED].new_value);
                nk_layout_row_push(ctx, 0.49f);
                nk_slider_int(ctx, 50, &settings_gameplay.config_values[CONFIG_GP_SCROLL_SPEED].new_value, 180, 5);
            }
            create_row(ctx);
            nk_checkbox_label(ctx, gettext("Disable map scrolling on window edge"), &settings_gameplay.config_values[CONFIG_UI_DISABLE_MOUSE_EDGE_SCROLLING].new_value);
            create_row(ctx);
            nk_checkbox_label(ctx, gettext("Disable dragging with right mouse button"), &settings_gameplay.config_values[CONFIG_UI_DISABLE_RIGHT_CLICK_MAP_DRAG].new_value);
            create_row(ctx);
            nk_checkbox_label(ctx, gettext("Inverse dragging map with right mouse button"), &settings_gameplay.config_values[CONFIG_UI_INVERSE_MAP_DRAG].new_value);
            nk_group_end(ctx);
        }
        nk_layout_space_push(ctx, nk_rect(365, 0, 499, h - 105));
        if (nk_group_begin(ctx, "group_visulization", NK_WINDOW_BORDER)) {
            {
                ui_font_change(FONT_TYPE_LARGE_BOLD);;
                create_row(ctx);
                nk_label(ctx, gettext("Visualization"), NK_TEXT_LEFT);
                ui_font_change(FONT_TYPE_STANDARD);
                create_row(ctx);
                nk_checkbox_label(ctx, gettext("Show grid"), &settings_gameplay.config_values[CONFIG_UI_SHOW_GRID].new_value);
                create_row(ctx);
                nk_checkbox_label(ctx, gettext("Show partial grid around construction"), &settings_gameplay.config_values[CONFIG_UI_SHOW_PARTIAL_GRID_AROUND_CONSTRUCTION].new_value);
                create_row(ctx);
                nk_checkbox_label(ctx, gettext("Show range when building reservoirs and wells"), &settings_gameplay.config_values[CONFIG_UI_SHOW_WATER_STRUCTURE_RANGE].new_value);
                create_row(ctx);
                nk_checkbox_label(ctx, gettext("Show range of reservoirs and wells when building houses"), &settings_gameplay.config_values[CONFIG_UI_SHOW_WATER_STRUCTURE_RANGE_HOUSES].new_value);
                create_row(ctx);
                nk_checkbox_label(ctx, gettext("Show range when building markets"), &settings_gameplay.config_values[CONFIG_UI_SHOW_MARKET_RANGE].new_value);
                create_row(ctx);
                nk_checkbox_label(ctx, gettext("Show draggable construction size"), &settings_gameplay.config_values[CONFIG_UI_SHOW_CONSTRUCTION_SIZE].new_value);
                create_row(ctx);
                nk_checkbox_label(ctx, gettext("Preview paths travelled by roaming walkers"), &settings_gameplay.config_values[CONFIG_UI_SHOW_ROAMING_PATH].new_value);
            }
            nk_group_end(ctx);
        }
        ui_font_change(FONT_TYPE_LARGE_BOLD);
        nk_layout_space_push(ctx, nk_rect(0, h - 95, 865, 65));
        if (nk_group_begin(ctx, "group_bottom_left", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 3);
            {
                nk_layout_row_push(ctx, 0.2f);
                if (nk_button_label(ctx, gettext("Back"))) {
                    cancel_values();
                    window_go_back();
                }
                nk_layout_row_push(ctx, 0.6f);
                nk_spacer(ctx);
                nk_layout_row_push(ctx, 0.2f);
                create_ok_button(ctx);
                nk_layout_row_end(ctx);
            }
            nk_group_end(ctx);
        }
    }
    nk_end(ctx);
    ui_font_change(FONT_TYPE_STANDARD);
}

void window_settings_gameplay_show() {
    settings_gameplay_init();
    window_type window = {
            WINDOW_SETTINGS_GAMEPLAY_MENU,
            draw_background,
            draw_foreground,
            handle_input
    };
    window_show(&window);
}