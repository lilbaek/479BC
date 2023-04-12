#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#include <string.h>
#include "settings_general.h"
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
#include "sound/speech.h"
#include "sound/music.h"
#include "sound/city.h"
#include "sound/effect.h"

typedef struct {
    int width;
    int height;
} resolution;

static struct {
    int current_resolution_index;

    struct {
        int original_value;
        int new_value;
        int changed;
    } config_values[CONFIG_MAX_ENTRIES];
} settings;

static void save_changes();

static void handle_fullscreen_change(int fullscreen);

void find_current_resolution();

static void create_graphics_options(struct nk_context *ctx);

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

static void create_row_label(struct nk_context *ctx, char* label) {
    nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 3);
    nk_layout_row_push(ctx, 0.01f);
    nk_spacer(ctx);
    nk_layout_row_push(ctx, 0.3f);
    nk_label(ctx, label, NK_TEXT_LEFT);
    nk_layout_row_push(ctx, 0.69f);
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

static void create_audio_row(struct nk_context *ctx, char* label, config_key checkbox, config_key volume) {
    {
        create_row(ctx);
        nk_checkbox_label(ctx, label, &settings.config_values[checkbox].new_value);
    }
    {
        nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 3);
        nk_layout_row_push(ctx, 0.01f);
        nk_spacer(ctx);
        nk_layout_row_push(ctx, 0.5f);
        nk_labelf(ctx, NK_TEXT_LEFT, gettext("Volume: %u"),
                  (int) settings.config_values[volume].new_value);
        nk_layout_row_push(ctx, 0.49f);
        nk_slider_int(ctx, 0, &settings.config_values[volume].new_value, 100, 1);
    }
}

static void draw_foreground(void) {
    struct nk_context *ctx = ui_context();
    int w = 677;
    int h = 500;
    ui_font_change(FONT_TYPE_LARGE_BOLD);
    if (nk_begin_titled(ctx, "settings_general", gettext("General Settings"),
                        nk_recti((screen_width() / 2) - w / 2, (screen_height() / 2) - h / 2, w, h),
                        NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE)) {
        ui_font_change(FONT_TYPE_STANDARD);
        nk_layout_space_begin(ctx, NK_STATIC, h - 40, 3);
        nk_layout_space_push(ctx, nk_rect(0, 0, 330, h - 105));
        if (nk_group_begin(ctx, "group_left", NK_WINDOW_BORDER)) {
            create_graphics_options(ctx);
        }
        nk_layout_space_push(ctx, nk_rect(335, 0, 328, h - 105));
        if (nk_group_begin(ctx, "group_right", NK_WINDOW_BORDER)) {
            {
                ui_font_change(FONT_TYPE_LARGE_BOLD);;
                create_row(ctx);
                nk_label(ctx, gettext("Sound"), NK_TEXT_LEFT);
                ui_font_change(FONT_TYPE_STANDARD);
            }
            create_audio_row(ctx, gettext("Enable audio"), CONFIG_GENERAL_ENABLE_AUDIO, CONFIG_GENERAL_MASTER_VOLUME);
            create_audio_row(ctx, gettext("Enable music"), CONFIG_GENERAL_ENABLE_MUSIC, CONFIG_GENERAL_MUSIC_VOLUME);
            create_audio_row(ctx, gettext("Enable speech"), CONFIG_GENERAL_ENABLE_SPEECH, CONFIG_GENERAL_SPEECH_VOLUME);
            create_audio_row(ctx, gettext("Enable effects"), CONFIG_GENERAL_ENABLE_EFFECTS, CONFIG_GENERAL_EFFECTS_VOLUME);
            create_audio_row(ctx, gettext("Enable city sounds"), CONFIG_GENERAL_ENABLE_CITY, CONFIG_GENERAL_CITY_VOLUME);
            nk_group_end(ctx);
        }
        ui_font_change(FONT_TYPE_LARGE_BOLD);
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

static void create_graphics_options(struct nk_context *ctx) {
    {
        ui_font_change(FONT_TYPE_LARGE_BOLD);;
        create_row(ctx);
        nk_label(ctx, gettext("Graphics"), NK_TEXT_LEFT);
        ui_font_change(FONT_TYPE_STANDARD);
    }
    {
        create_row(ctx);
        nk_checkbox_label(ctx, gettext("Full screen"), &settings.config_values[CONFIG_SCREEN_FULLSCREEN].new_value);
    }
    {
        create_row_label(ctx, gettext("Resolution"));
        create_resolution_combo(ctx);
    }
    {
        nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 3);
        nk_layout_row_push(ctx, 0.01f);
        nk_spacer(ctx);
        nk_layout_row_push(ctx, 0.5f);
        nk_labelf(ctx, NK_TEXT_LEFT, gettext("Display scale: %u") , (int)settings.config_values[CONFIG_SCREEN_DISPLAY_SCALE].new_value);
        nk_layout_row_push(ctx, 0.49f);
        nk_slider_int(ctx, 50, &settings.config_values[CONFIG_SCREEN_DISPLAY_SCALE].new_value, 180, 5);
    }
    {
        nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 3);
        nk_layout_row_push(ctx, 0.01f);
        nk_spacer(ctx);
        nk_layout_row_push(ctx, 0.5f);
        nk_labelf(ctx, NK_TEXT_LEFT, gettext("Cursor scale: %u") , (int)settings.config_values[CONFIG_SCREEN_CURSOR_SCALE].new_value);
        nk_layout_row_push(ctx, 0.49f);
        nk_slider_int(ctx, 100, &settings.config_values[CONFIG_SCREEN_CURSOR_SCALE].new_value, 200, 50);
    }
    nk_group_end(ctx);
}

static void handle_input(const mouse *m, const hotkeys *h) {
    if (h->escape_pressed) {
        cancel_values();
        window_go_back();
    }
}

void settings_general_init() {
    for (int i = 0; i < CONFIG_MAX_ENTRIES; i++) {
        settings.config_values[i].original_value = config_get(i);
        settings.config_values[i].new_value = config_get(i);
    }
    find_current_resolution();
}

static void save_changes() {
    // Save changes first
    for (int i = 0; i < CONFIG_MAX_ENTRIES; ++i) {
        if (settings.config_values[i].original_value == settings.config_values[i].new_value)
            continue;
        settings.config_values[i].changed = 1;
        config_set(i, settings.config_values[i].new_value);
        settings.config_values[i].original_value = settings.config_values[i].new_value;
    }
    // React to changes that requires immediate action
    for (int i = 0; i < CONFIG_MAX_ENTRIES; ++i) {
        if (settings.config_values[i].changed) {
            settings.config_values[i].changed = 0;
            switch (i) {
                case CONFIG_SCREEN_FULLSCREEN:
                case CONFIG_SCREEN_WIDTH:
                    handle_fullscreen_change(config_get(CONFIG_SCREEN_FULLSCREEN));
                    break;
                case CONFIG_SCREEN_CURSOR_SCALE:
                    system_init_cursors(config_get(CONFIG_SCREEN_CURSOR_SCALE));
                    break;
                case CONFIG_SCREEN_DISPLAY_SCALE:
                    system_scale_display(config_get(CONFIG_SCREEN_DISPLAY_SCALE));
                    break;
                case CONFIG_GENERAL_ENABLE_AUDIO:
                    if (!config_get(CONFIG_GENERAL_ENABLE_AUDIO)) {
                        sound_music_stop();
                        sound_speech_stop();
                    }
                    break;
                case CONFIG_GENERAL_ENABLE_MUSIC:
                    if (!config_get(CONFIG_GENERAL_ENABLE_MUSIC)) {
                        sound_music_stop();
                    }
                    break;
                case CONFIG_GENERAL_ENABLE_SPEECH:
                    if (!config_get(CONFIG_GENERAL_ENABLE_SPEECH)) {
                        sound_speech_stop();
                    }
                    break;
                case CONFIG_GENERAL_MASTER_VOLUME:
                case CONFIG_GENERAL_MUSIC_VOLUME:
                case CONFIG_GENERAL_SPEECH_VOLUME:
                case CONFIG_GENERAL_EFFECTS_VOLUME:
                case CONFIG_GENERAL_CITY_VOLUME:
                    sound_music_set_volume(config_get(CONFIG_GENERAL_MUSIC_VOLUME));
                    sound_speech_set_volume(config_get(CONFIG_GENERAL_SPEECH_VOLUME));
                    sound_effect_set_volume(config_get(CONFIG_GENERAL_EFFECTS_VOLUME));
                    sound_city_set_volume(config_get(CONFIG_GENERAL_CITY_VOLUME));
                    break;
            }
        }
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

void window_settings_general_show() {
    settings_general_init();
    window_type window = {
            WINDOW_SETTINGS_GENERAL_MENU,
            draw_background,
            draw_foreground,
            handle_input
    };
    window_show(&window);
}

void find_current_resolution() {
    settings.current_resolution_index = -1;
    int width = screen_width();
    int height = screen_height();
    for (int i = 0; i < sizeof(resolutions) / sizeof(resolution); i++) {
        if (resolutions[i].width == width && resolutions[i].height == height) {
            settings.current_resolution_index = i;
            break;
        }
    }
    if(settings.current_resolution_index == -1) {
        width = settings.config_values[CONFIG_SCREEN_WIDTH].original_value;
        height = settings.config_values[CONFIG_SCREEN_HEIGHT].original_value;
        for (int i = 0; i < sizeof(resolutions) / sizeof(resolution); i++) {
            if (resolutions[i].width == width && resolutions[i].height == height) {
                settings.current_resolution_index = i;
                break;
            }
        }
    }
    if(settings.current_resolution_index == -1) {
        settings.current_resolution_index = 0;
    }
}