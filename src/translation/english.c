#include "translation/common.h"
#include "translation/translation.h"

static translation_string all_strings[] = {
    {TR_NO_PATCH_TITLE, "Patch 1.0.1.0 not installed"},
    {TR_NO_PATCH_MESSAGE,
        "Your Caesar 3 installation does not have the 1.0.1.0 patch installed. "
        "You can download the patch from:\n"
        URL_PATCHES "\n"
        "Continue at your own risk."},
    {TR_MISSING_FONTS_TITLE, "Missing fonts"},
    {TR_MISSING_FONTS_MESSAGE,
        "Your Caesar 3 installation requires extra font files. "
        "You can download them for your language from:\n"
        URL_PATCHES},
    {TR_NO_EDITOR_TITLE, "Editor not installed"},
    {TR_NO_EDITOR_MESSAGE,
        "Your Caesar 3 installation does not contain the editor files. "
        "You can download them from:\n"
        URL_EDITOR},
    {TR_INVALID_LANGUAGE_TITLE, "Invalid language directory"},
    {TR_INVALID_LANGUAGE_MESSAGE,
        "The directory you selected does not contain a valid language pack. "
        "Please check the log for errors."},
    {TR_BUTTON_OK, "OK"},
    {TR_BUTTON_CANCEL, "Cancel"},
    {TR_BUTTON_RESET_DEFAULTS, "Reset defaults"},
    {TR_BUTTON_CONFIGURE_HOTKEYS, "Configure hotkeys"},
    {TR_CONFIG_TITLE, "Julius configuration options"},
    {TR_CONFIG_LANGUAGE_LABEL, "Language:"},
    {TR_CONFIG_LANGUAGE_DEFAULT, "(default)"},
    {TR_CONFIG_HEADER_UI_CHANGES, "User interface changes"},
    {TR_CONFIG_HEADER_GAMEPLAY_CHANGES, "Gameplay changes"},
    {TR_CONFIG_SHOW_INTRO_VIDEO, "Play intro videos"},
    {TR_CONFIG_SIDEBAR_INFO, "Extra information in the control panel"},
    {TR_CONFIG_SMOOTH_SCROLLING, "Enable smooth scrolling"},
    {TR_CONFIG_VISUAL_FEEDBACK_ON_DELETE, "Improve visual feedback when clearing land"},
    {TR_CONFIG_ALLOW_CYCLING_TEMPLES, "Allow building each temple in succession"},
    {TR_CONFIG_SHOW_WATER_STRUCTURE_RANGE, "Show range when building reservoirs, fountains and wells"},
    {TR_CONFIG_SHOW_CONSTRUCTION_SIZE, "Show draggable construction size"},
    {TR_CONFIG_HIGHLIGHT_LEGIONS, "Highlight legion on cursor hover"},
    {TR_CONFIG_FIX_IMMIGRATION_BUG, "Fix immigration bug on very hard"},
    {TR_CONFIG_FIX_100_YEAR_GHOSTS, "Fix 100-year-old ghosts"},
    {TR_CONFIG_FIX_EDITOR_EVENTS, "Fix Emperor change and survival time in custom missions"},
    {TR_HOTKEY_TITLE, "Julius hotkey configuration"},
    {TR_HOTKEY_LABEL, "Hotkey"},
    {TR_HOTKEY_ALTERNATIVE_LABEL, "Alternative"},
    {TR_HOTKEY_HEADER_ARROWS, "Arrow keys"},
    {TR_HOTKEY_HEADER_GLOBAL, "Global hotkeys"},
    {TR_HOTKEY_HEADER_CITY, "City hotkeys"},
    {TR_HOTKEY_HEADER_ADVISORS, "Advisors"},
    {TR_HOTKEY_HEADER_OVERLAYS, "Overlays"},
    {TR_HOTKEY_HEADER_BOOKMARKS, "City map bookmarks"},
    {TR_HOTKEY_HEADER_EDITOR, "Editor"},
    {TR_HOTKEY_HEADER_BUILD, "Construction hotkeys"},
    {TR_HOTKEY_ARROW_UP, "Up"},
    {TR_HOTKEY_ARROW_DOWN, "Down"},
    {TR_HOTKEY_ARROW_LEFT, "Left"},
    {TR_HOTKEY_ARROW_RIGHT, "Right"},
    {TR_HOTKEY_TOGGLE_FULLSCREEN, "Toggle fullscreen"},
    {TR_HOTKEY_CENTER_WINDOW, "Center window"},
    {TR_HOTKEY_RESIZE_TO_640, "Resize window to 640x480"},
    {TR_HOTKEY_RESIZE_TO_800, "Resize window to 800x600"},
    {TR_HOTKEY_RESIZE_TO_1024, "Resize window to 1024x768"},
    {TR_HOTKEY_SAVE_SCREENSHOT, "Save screenshot"},
    {TR_HOTKEY_SAVE_CITY_SCREENSHOT, "Save full city screenshot"},
    {TR_HOTKEY_LOAD_FILE, "Load file"},
    {TR_HOTKEY_SAVE_FILE, "Save file"},
    {TR_HOTKEY_INCREASE_GAME_SPEED, "Increase game speed"},
    {TR_HOTKEY_DECREASE_GAME_SPEED, "Decrease game speed"},
    {TR_HOTKEY_TOGGLE_PAUSE, "Toggle pause"},
    {TR_HOTKEY_CYCLE_LEGION, "Cycle through legions"},
    {TR_HOTKEY_ROTATE_MAP_LEFT, "Rotate map left"},
    {TR_HOTKEY_ROTATE_MAP_RIGHT, "Rotate map right"},
    {TR_HOTKEY_SHOW_ADVISOR_LABOR, "Labor advisor"},
    {TR_HOTKEY_SHOW_ADVISOR_MILITARY, "Military advisor"},
    {TR_HOTKEY_SHOW_ADVISOR_IMPERIAL, "Imperial advisor"},
    {TR_HOTKEY_SHOW_ADVISOR_RATINGS, "Ratings advisor"},
    {TR_HOTKEY_SHOW_ADVISOR_TRADE, "Trade advisor"},
    {TR_HOTKEY_SHOW_ADVISOR_POPULATION, "Population advisor"},
    {TR_HOTKEY_SHOW_ADVISOR_HEALTH, "Health advisor"},
    {TR_HOTKEY_SHOW_ADVISOR_EDUCATION, "Education advisor"},
    {TR_HOTKEY_SHOW_ADVISOR_ENTERTAINMENT, "Entertainment advisor"},
    {TR_HOTKEY_SHOW_ADVISOR_RELIGION, "Religion advisor"},
    {TR_HOTKEY_SHOW_ADVISOR_FINANCIAL, "Financial advisor"},
    {TR_HOTKEY_SHOW_ADVISOR_CHIEF, "Chief advisor"},
    {TR_HOTKEY_TOGGLE_OVERLAY, "Toggle current overlay"},
    {TR_HOTKEY_SHOW_OVERLAY_WATER, "Show water overlay"},
    {TR_HOTKEY_SHOW_OVERLAY_FIRE, "Show fire overlay"},
    {TR_HOTKEY_SHOW_OVERLAY_DAMAGE, "Damage overlay"},
    {TR_HOTKEY_SHOW_OVERLAY_CRIME, "Crime overlay"},
    {TR_HOTKEY_SHOW_OVERLAY_PROBLEMS, "Problems overlay"},
    {TR_HOTKEY_GO_TO_BOOKMARK_1, "Go to bookmark 1"},
    {TR_HOTKEY_GO_TO_BOOKMARK_2, "Go to bookmark 2"},
    {TR_HOTKEY_GO_TO_BOOKMARK_3, "Go to bookmark 3"},
    {TR_HOTKEY_GO_TO_BOOKMARK_4, "Go to bookmark 4"},
    {TR_HOTKEY_SET_BOOKMARK_1, "Set bookmark 1"},
    {TR_HOTKEY_SET_BOOKMARK_2, "Set bookmark 2"},
    {TR_HOTKEY_SET_BOOKMARK_3, "Set bookmark 3"},
    {TR_HOTKEY_SET_BOOKMARK_4, "Set bookmark 4"},
    {TR_HOTKEY_EDITOR_TOGGLE_BATTLE_INFO, "Toggle battle info"},
    {TR_HOTKEY_EDIT_TITLE, "Press new hotkey"}
};

void translation_english(const translation_string **strings, int *num_strings)
{
    *strings = all_strings;
    *num_strings = sizeof(all_strings) / sizeof(translation_string);
}
