#ifndef CORE_CONFIG_H
#define CORE_CONFIG_H

typedef enum {
    DIFFICULTY_VERY_EASY = 0,
    DIFFICULTY_EASY = 1,
    DIFFICULTY_NORMAL = 2,
    DIFFICULTY_HARD = 3,
    DIFFICULTY_VERY_HARD = 4
} set_difficulty;

typedef enum {
    CONFIG_SCREEN_FULLSCREEN,
    CONFIG_SCREEN_WIDTH,
    CONFIG_SCREEN_HEIGHT,
    CONFIG_SCREEN_DISPLAY_SCALE,
    CONFIG_SCREEN_CURSOR_SCALE,
    CONFIG_GENERAL_ENABLE_AUDIO,
    CONFIG_GENERAL_MASTER_VOLUME,
    CONFIG_GENERAL_ENABLE_MUSIC,
    CONFIG_GENERAL_MUSIC_VOLUME,
    CONFIG_GENERAL_ENABLE_SPEECH,
    CONFIG_GENERAL_SPEECH_VOLUME,
    CONFIG_GENERAL_ENABLE_EFFECTS,
    CONFIG_GENERAL_EFFECTS_VOLUME,
    CONFIG_GENERAL_ENABLE_CITY,
    CONFIG_GENERAL_CITY_VOLUME,
    CONFIG_UI_SIDEBAR_INFO,
    CONFIG_UI_SMOOTH_SCROLLING,
    CONFIG_UI_DISABLE_MOUSE_EDGE_SCROLLING,
    CONFIG_UI_SHOW_WATER_STRUCTURE_RANGE,
    CONFIG_UI_SHOW_WATER_STRUCTURE_RANGE_HOUSES,
    CONFIG_UI_SHOW_MARKET_RANGE,
    CONFIG_UI_SHOW_CONSTRUCTION_SIZE,
    CONFIG_UI_HIGHLIGHT_LEGIONS,
    CONFIG_UI_SHOW_MILITARY_SIDEBAR,
    CONFIG_UI_DISABLE_RIGHT_CLICK_MAP_DRAG,
    CONFIG_UI_SHOW_MAX_PROSPERITY,
    CONFIG_UI_DIGIT_SEPARATOR,
    CONFIG_UI_INVERSE_MAP_DRAG,
    CONFIG_UI_MESSAGE_ALERTS,
    CONFIG_UI_SHOW_GRID,
    CONFIG_UI_SHOW_PARTIAL_GRID_AROUND_CONSTRUCTION,
    CONFIG_UI_SHOW_ROAMING_PATH,
    CONFIG_GP_GAME_SPEED,
    CONFIG_GP_SCROLL_SPEED,
    CONFIG_GP_DIFFICULTY,
    CONFIG_GP_GODS,
    CONFIG_GP_LAST_ADVISOR,
    CONFIG_GP_CH_MAX_GRAND_TEMPLES,
    CONFIG_GP_CH_JEALOUS_GODS,
    CONFIG_GP_CH_DELIVER_ONLY_TO_ACCEPTING_GRANARIES,
    CONFIG_GP_CH_WAREHOUSES_DONT_ACCEPT,
    CONFIG_GP_CH_YEARLY_AUTOSAVE,
    CONFIG_MAX_ENTRIES
} config_key;

/**
 * Get an integer config value
 * @param key Integer key
 * @return Config value
 */
int config_get(config_key key);

/**
 * Set an integer config value
 * @param key Integer key
 * @param value Value to set
 */
void config_set(config_key key, int value);

/**
 * Set a default config value
 * @param key Integer key
 * @return Default config value
 */
int config_get_default_value(config_key key);

/**
 * Load config from file
 */
void config_load(void);

/**
 * Save config to file
 */
void config_save(void);

void setting_set_default_game_speed(void);
int setting_game_speed(void);
void setting_increase_game_speed(void);
void setting_decrease_game_speed(void);
#endif // CORE_CONFIG_H
