
#include <string.h>
#include "settings_gameplay.h"
#include "graphics/window.h"
#include "ui_window.h"
#include "translation/translation.h"
#include "nuklear.h"
#include "graphics/screen.h"
#include "graphics/image.h"
#include "assets/assets.h"
#include "game/settings.h"
#include "core/config.h"
#include "game/system.h"

typedef struct {
    int width;
    int height;
} resolution;

static struct {
    int current_resolution_index;

    struct {
        int original_value;
        int new_value;
    } config_values[CONFIG_MAX_ENTRIES];
} settings;

static void save_changes();

static void handle_fullscreen_change(int fullscreen);

static const resolution resolutions[] = {
        {1024, 768},
        {1280, 720},
        {1280, 800},
        {1280, 1024},
        {1360, 768},
        {1366, 768},
        {1440, 900},
        {1536, 864},
        {1600, 900},
        {1680, 1050},
        {1920, 1080},
        {1920, 1200},
        {2048, 1152},
        {2560, 1080},
        {2560, 1440},
        {3440, 1440},
        {3840, 2160}
};
static char *string_resolutions[] = {
        "1024x768", "1280x720",
        "1280x800", "1280x1024", "1360x768", "1366x768",
        "1440x900", "1536x864", "1600x900", "1680x1050",
        "1920x1080", "1920x1200", "2048x1152", "2560x1080",
        "2560x1440", "3440x1440", "3840x2160"
};

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

static void draw_background(void) {
    image_draw_fullscreen_background(assets_get_image_id("mainmenu", "start-screen"));
}

static void create_row(struct nk_context *ctx) {
    nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 2);
    nk_layout_row_push(ctx, 0.01f);
    nk_spacer(ctx);
    nk_layout_row_push(ctx, 0.99f);
}

static void create_resolution_combo(struct nk_context *ctx) {
    struct nk_rect rect = nk_widget_bounds(ctx);
    int previous = settings.current_resolution_index;
    settings.current_resolution_index = nk_combo(ctx, (const char **) string_resolutions, NK_LEN(string_resolutions), settings.current_resolution_index, 25,
                                                 nk_vec2(rect.w, 300));
    if (previous != settings.current_resolution_index) {
        settings.config_values[CONFIG_SCREEN_WIDTH].new_value = resolutions[settings.current_resolution_index].width;
        settings.config_values[CONFIG_SCREEN_HEIGHT].new_value = resolutions[settings.current_resolution_index].height;
    }
}

static void cancel_values(void) {
    for (int i = 0; i < CONFIG_MAX_ENTRIES; i++) {
        settings.config_values[i].new_value = settings.config_values[i].original_value;
    }
}

static void draw_foreground(void) {
    struct nk_context *ctx = ui_context();
    int w = 677;
    int h = 500;
    ui_font_change(FONT_TYPE_LARGE_BOLD);;
    if (nk_begin_titled(ctx, "settings_gameplay", gettext("Gameplay"),
                        nk_recti((screen_width() / 2) - w / 2, (screen_height() / 2) - h / 2, w, h),
                        NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE)) {
        ui_font_change(FONT_TYPE_STANDARD);
        nk_layout_space_begin(ctx, NK_STATIC, h - 40, 3);
        nk_layout_space_push(ctx, nk_rect(0, 0, 330, h - 105));
        if (nk_group_begin(ctx, "group_left", NK_WINDOW_BORDER)) {
            ui_font_change(FONT_TYPE_LARGE_BOLD);;
            create_row(ctx);
            nk_label(ctx, gettext("Graphics"), NK_TEXT_LEFT);
            ui_font_change(FONT_TYPE_STANDARD);
            create_row(ctx);
            int *active = &settings.config_values[CONFIG_SCREEN_FULLSCREEN].new_value;
            nk_checkbox_label(ctx, gettext("Full screen"), active);
            create_row(ctx);
            create_resolution_combo(ctx);

            nk_group_end(ctx);
        }
        nk_layout_space_push(ctx, nk_rect(335, 0, 328, h - 105));
        if (nk_group_begin(ctx, "group_right", NK_WINDOW_BORDER)) {
            ui_font_change(FONT_TYPE_LARGE_BOLD);;
            create_row(ctx);
            nk_label(ctx, gettext("Sound"), NK_TEXT_LEFT);
            ui_font_change(FONT_TYPE_STANDARD);
            nk_group_end(ctx);
        }
        ui_font_change(FONT_TYPE_LARGE_BOLD);;
        nk_layout_space_push(ctx, nk_rect(0, h - 95, 665, 65));
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

static void handle_input(const mouse *m, const hotkeys *h) {
    if (h->escape_pressed) {
        cancel_values();
        window_go_back();
    }
}

void settings_gameplay_init() {
    for (int i = 0; i < CONFIG_MAX_ENTRIES; i++) {
        settings.config_values[i].original_value = config_get(i);
        settings.config_values[i].new_value = config_get(i);
    }
    int width = screen_width();
    int height = screen_height();
    for (int i = 0; i < sizeof(resolutions) / sizeof(resolution); i++) {
        if (resolutions[i].width == width && resolutions[i].height == height) {
            settings.current_resolution_index = i;
            break;
        }
    }
}

static void save_changes() {
    int change_screen = 0;
    for (int i = 0; i < CONFIG_MAX_ENTRIES; ++i) {
        if (settings.config_values[i].original_value == settings.config_values[i].new_value)
            continue;

        config_set(i, settings.config_values[i].new_value);
        settings.config_values[i].original_value = settings.config_values[i].new_value;
        switch (i) {
            case CONFIG_SCREEN_FULLSCREEN:
            case CONFIG_SCREEN_WIDTH:
                change_screen = 1;
                break;
        }
    }
    if (change_screen) {
        handle_fullscreen_change(settings.config_values[CONFIG_SCREEN_FULLSCREEN].new_value);
    }
    config_save();
    window_go_back();
}

static void handle_fullscreen_change(int fullscreen) {
    if (!system_is_fullscreen_only()) {
        const resolution *r = &resolutions[settings.current_resolution_index];

        int width = r->width;
        int height = r->height;
        resolution max;
        system_get_max_resolution(&max.width, &max.height);
        if (width > max.width || height > max.height) {
            width = max.width;
            height = max.height;
        }
        if (!fullscreen) {
            system_set_fullscreen(fullscreen);
            system_resize(width, height);
        } else {
            system_set_fullscreen(fullscreen);
        }
    }
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
