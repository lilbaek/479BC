#include "config.h"

#include "core/file.h"
#include "core/log.h"
#include "platform/prefs.h"
#include "game/settings.h"
#include "city/constants.h"

#include <stdio.h>
#include <string.h>

#define MAX_LINE 100

static const char *INI_FILENAME = "tiberius.ini";

// Keep this in the same order as the config_keys in config.h
static const char *ini_keys[] = {
        "config_screen_fullscreen",
        "config_screen_width",
        "config_screen_height",
        "config_screen_display_scale",
        "config_screen_cursor_scale",
        "config_sound_enable_audio",
        "config_sound_master_volume",
        "config_sound_enable_music",
        "config_sound_music_volume",
        "config_sound_enable_speech",
        "config_sound_speech_volume",
        "config_sound_enable_effects",
        "config_sound_effects_volume",
        "config_sound_enable_city",
        "config_sound_city_volume",
        "ui_sidebar_info",
        "ui_smooth_scrolling",
        "ui_disable_mouse_edge_scrolling",
        "ui_show_water_structure_range",
        "ui_show_water_structure_range_houses",
        "ui_show_market_range",
        "ui_show_construction_size",
        "ui_highlight_legions",
        "ui_show_military_sidebar",
        "ui_disable_map_drag",
        "ui_show_max_prosperity",
        "ui_digit_separator",
        "ui_inverse_map_drag",
        "ui_message_alerts",
        "ui_show_grid",
        "ui_show_partial_grid_around_construction",
        "ui_show_roaming_path",
        "gameplay_game_speed",
        "gameplay_scroll_speed",
        "gameplay_difficulty",
        "gameplay_gods_enabled",
        "gameplay_last_advisor",
        "gameplay_change_max_grand_temples",
        "gameplay_change_jealous_gods",
        "gameplay_change_only_deliver_to_accepting_granaries",
        "gameplay_change_warehouses_dont_accept",
        "gameplay_change_yearly_autosave",
};

static int values[CONFIG_MAX_ENTRIES];

static int default_values[CONFIG_MAX_ENTRIES] = {
        [CONFIG_GENERAL_ENABLE_AUDIO] = 1,
        [CONFIG_GENERAL_MASTER_VOLUME] = 100,
        [CONFIG_GENERAL_ENABLE_MUSIC] = 1,
        [CONFIG_GENERAL_MUSIC_VOLUME] = 80,
        [CONFIG_GENERAL_ENABLE_SPEECH] = 1,
        [CONFIG_GENERAL_SPEECH_VOLUME] = 90,
        [CONFIG_GENERAL_ENABLE_EFFECTS] = 1,
        [CONFIG_GENERAL_EFFECTS_VOLUME] = 90,
        [CONFIG_GENERAL_ENABLE_CITY] = 1,
        [CONFIG_GENERAL_CITY_VOLUME] = 90,
        [CONFIG_UI_SIDEBAR_INFO] = 1,
        [CONFIG_UI_SMOOTH_SCROLLING] = 1,
        [CONFIG_UI_HIGHLIGHT_LEGIONS] = 1,
        [CONFIG_SCREEN_DISPLAY_SCALE] = 100,
        [CONFIG_SCREEN_CURSOR_SCALE] = 100,
        [CONFIG_GP_GAME_SPEED] = 100,
        [CONFIG_GP_SCROLL_SPEED] = 100,
        [CONFIG_GP_DIFFICULTY] = DIFFICULTY_NORMAL,
        [CONFIG_GP_GODS] = 1,
        [CONFIG_GP_LAST_ADVISOR] = ADVISOR_LABOR,
        [CONFIG_GP_CH_MAX_GRAND_TEMPLES] = 3,
        [CONFIG_SCREEN_FULLSCREEN] = 0,
        [CONFIG_SCREEN_WIDTH] = 1024,
        [CONFIG_SCREEN_HEIGHT] = 768,
        [CONFIG_UI_SHOW_PARTIAL_GRID_AROUND_CONSTRUCTION] = 1,
        [CONFIG_UI_SHOW_WATER_STRUCTURE_RANGE] = 1,
        [CONFIG_UI_SHOW_WATER_STRUCTURE_RANGE_HOUSES] = 1,
        [CONFIG_UI_SHOW_MARKET_RANGE] = 1,
        [CONFIG_UI_SHOW_CONSTRUCTION_SIZE] = 1,
        [CONFIG_UI_SHOW_ROAMING_PATH] = 1,
        [CONFIG_GP_CH_YEARLY_AUTOSAVE] = 1,
};

int config_get(config_key key) {
    return values[key];
}

void config_set(config_key key, int value) {
    values[key] = value;
}

int config_get_default_value(config_key key) {
    return default_values[key];
}

static void set_defaults(void) {
    for (int i = 0; i < CONFIG_MAX_ENTRIES; ++i) {
        values[i] = default_values[i];
    }
}

void config_load(void) {
    set_defaults();
    FILE *fp = pref_file_open(INI_FILENAME, "settings", "rt");
    if (!fp) {
        return;
    }
    char line_buffer[MAX_LINE];
    char *line;
    while ((line = fgets(line_buffer, MAX_LINE, fp))) {
        // Remove newline from string
        size_t size = strlen(line);
        while (size > 0 && (line[size - 1] == '\n' || line[size - 1] == '\r')) {
            line[--size] = 0;
        }
        char *equals = strchr(line, '=');
        if (equals) {
            *equals = 0;
            for (int i = 0; i < CONFIG_MAX_ENTRIES; i++) {
                if (strcmp(ini_keys[i], line) == 0) {
                    int value = atoi(&equals[1]);
                    log_info("Config key", ini_keys[i], value);
                    values[i] = value;
                    break;
                }
            }
        }
    }
    file_close(fp);
}

void config_save(void) {
    FILE *fp = pref_file_open(INI_FILENAME, "settings", "wt");
    if (!fp) {
        log_error("Unable to write configuration file", INI_FILENAME, 0);
        return;
    }
    for (int i = 0; i < CONFIG_MAX_ENTRIES; i++) {
        fprintf(fp, "%s=%d\n", ini_keys[i], values[i]);
    }
    file_close(fp);
}
