#ifndef GRAPHICS_WINDOW_H
#define GRAPHICS_WINDOW_H

#include "graphics/tooltip.h"
#include "input/hotkey.h"
#include "input/mouse.h"

typedef enum {
    WINDOW_MAIN_MENU,
    WINDOW_SETTINGS_MENU,
    WINDOW_SETTINGS_GENERAL_MENU,
    WINDOW_SETTINGS_GAMEPLAY_MENU,
    WINDOW_SETTINGS_HOTKEYS,
    WINDOW_MAP_GENERATOR,
    WINDOW_LOAD_DIALOG,
    WINDOW_CONFIG,
    WINDOW_HOTKEY_CONFIG,
    WINDOW_HOTKEY_EDITOR,
    WINDOW_CCK_SELECTION,
    WINDOW_POPUP_DIALOG,
    WINDOW_PLAIN_MESSAGE_DIALOG,
    // mission start/end
    WINDOW_INTERMEZZO,
    WINDOW_MISSION_SELECTION,
    WINDOW_MISSION_BRIEFING,
    WINDOW_VICTORY_DIALOG,
    WINDOW_VICTORY_VIDEO,
    WINDOW_MISSION_END,
    // city
    WINDOW_CITY,
    WINDOW_CITY_MILITARY,
    WINDOW_TOP_MENU,
    WINDOW_OVERLAY_MENU,
    WINDOW_MILITARY_MENU,
    WINDOW_BUILD_MENU,
    WINDOW_SLIDING_SIDEBAR,
    WINDOW_MESSAGE_DIALOG,
    WINDOW_MESSAGE_LIST,
    WINDOW_BUILDING_INFO,
    WINDOW_CITY_MAIN_MENU,
    WINDOW_HOLD_GAMES,
    WINDOW_RACE_BET,
    WINDOW_BUILD_MENU_BOTTOM_BAR_WATER,
    // advisors and dialogs
    WINDOW_ADVISORS,
    WINDOW_LABOR_PRIORITY,
    WINDOW_SET_SALARY,
    WINDOW_DONATE_TO_CITY,
    WINDOW_GIFT_TO_EMPEROR,
    WINDOW_TRADE_PRICES,
    WINDOW_RESOURCE_SETTINGS,
    WINDOW_HOLD_FESTIVAL,
    // empire and dialog
    WINDOW_EMPIRE,
    WINDOW_TRADE_OPENED,
    // utility windows
    WINDOW_SELECT_LIST,
    WINDOW_NUMERIC_INPUT,
    // editor
    WINDOW_EDITOR_MAP,
    WINDOW_EDITOR_TOP_MENU,
    WINDOW_EDITOR_BUILD_MENU,
    WINDOW_EDITOR_EMPIRE,
    WINDOW_EDITOR_ATTRIBUTES,
    WINDOW_EDITOR_ALLOWED_BUILDINGS,
    WINDOW_EDITOR_INVASIONS,
    WINDOW_EDITOR_EDIT_INVASION,
    WINDOW_EDITOR_REQUESTS,
    WINDOW_EDITOR_EDIT_REQUEST,
    WINDOW_EDITOR_STARTING_CONDITIONS,
    WINDOW_EDITOR_START_YEAR,
    WINDOW_EDITOR_SPECIAL_EVENTS,
    WINDOW_EDITOR_PRICE_CHANGES,
    WINDOW_EDITOR_EDIT_PRICE_CHANGE,
    WINDOW_EDITOR_DEMAND_CHANGES,
    WINDOW_EDITOR_EDIT_DEMAND_CHANGE,
    WINDOW_EDITOR_WIN_CRITERIA,
} window_id;

typedef struct {
    window_id id;
    void (*draw_background)(void);
    void (*draw_foreground)(void);
    void (*handle_input)(const mouse *m, const hotkeys *h);
    void (*get_tooltip)(tooltip_context *c);
    void (*draw_top)(void);
} window_type;

/**
 * Invalidates the window immediately, indicating that the current game state
 * requires a redraw before continuing
 */
void window_invalidate(void);

/**
 * Request a (soft) refresh of the window; does not invalidate the game state
 */
void window_request_refresh(void);

/**
 * Returns whether the window has been invalidated using `window_invalidate`
 */
int window_is_invalid(void);

void window_draw(int force);

void window_draw_underlying_window(void);

int window_draw_game(void);

int window_is(window_id id);

void window_show(const window_type *window);

window_id window_get_id(void);

void window_go_back(void);

#endif // GRAPHICS_WINDOW_H
