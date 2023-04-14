#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#include "settings_hotkey.h"
#include "core/hotkey_config.h"
#include "translation/translation.h"
#include "nuklear.h"
#include "graphics/screen.h"
#include "ui_window.h"
#include "graphics/window.h"
#include "graphics/image.h"
#include "assets/assets.h"
#include "settings_hotkey_editor.h"
#include "popup_dialog.h"

static struct {
    int focus_button;
    int bottom_focus_button;
    hotkey_mapping mappings[HOTKEY_MAX_ITEMS][2];
} hotkey_data;

static void settings_hotkey_init(void) {
    for (int i = 0; i < HOTKEY_MAX_ITEMS; i++) {
        hotkey_mapping empty = { KEY_TYPE_NONE, KEY_MOD_NONE, i };

        const hotkey_mapping *mapping = hotkey_for_action(i, 0);
        hotkey_data.mappings[i][0] = mapping ? *mapping : empty;

        mapping = hotkey_for_action(i, 1);
        hotkey_data.mappings[i][1] = mapping ? *mapping : empty;
    }
}

static void draw_background(void) {
    image_draw_fullscreen_background(assets_get_image_id("mainmenu", "start-screen"));
}

static void handle_input(const mouse *m, const hotkeys *h) {
    if (h->escape_pressed) {
        window_go_back();
    }
}

static void save_changes(void) {
    hotkey_config_clear();
    for (int action = 0; action < HOTKEY_MAX_ITEMS; action++) {
        for (int index = 0; index < 2; index++) {
            if (hotkey_data.mappings[action][index].key != KEY_TYPE_NONE) {
                hotkey_config_add_mapping(&hotkey_data.mappings[action][index]);
            }
        }
    }
    hotkey_config_save();
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

static void settings_hotkey_noop(int _) {

}

static void set_hotkey(hotkey_action action, int index, key_type key, key_modifier_type modifiers)
{
    int is_duplicate_hotkey = 0;
    // check if new key combination already assigned to another action
    if (key != KEY_TYPE_NONE) {
        for (int test_action = 0; test_action < HOTKEY_MAX_ITEMS; test_action++) {
            for (int test_index = 0; test_index < 2; test_index++) {
                if (hotkey_data.mappings[test_action][test_index].key == key
                    && hotkey_data.mappings[test_action][test_index].modifiers == modifiers) {
                    is_duplicate_hotkey = 1;
                    // example explanation next "if" check:
                    // "Fire overlay" already has hotkey "F" and user tries set same hotkey "F" again to "Fire overlay"
                    // we must skip show warning window for better user experience
                    if (!(test_action == action && test_index == index)) {
                        window_popup_dialog_show_ex(
                                gettext("Hotkey already used"), gettext("This key combination is already assigned to another action."),
                                settings_hotkey_noop, 0);
                    }
                    break;
                }
            }
            if (is_duplicate_hotkey) {
                break;
            }
        }
    }
    if (!is_duplicate_hotkey) {
        hotkey_data.mappings[action][index].key = key;
        hotkey_data.mappings[action][index].modifiers = modifiers;
    }
}

static void create_hotkey_widget(struct nk_context *ctx, hotkey_action action, char* label) {
    nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 4);
    nk_layout_row_push(ctx, 0.01f);
    nk_spacer(ctx);
    nk_layout_row_push(ctx, 0.3f);
    nk_label(ctx, label, NK_TEXT_LEFT);
    nk_layout_row_push(ctx, 0.34f);
    const hotkey_mapping *mapping1 = &hotkey_data.mappings[action][0];
    if (mapping1->key) {
        if (nk_button_label(ctx, key_combination_display_name(mapping1->key, mapping1->modifiers))) {
            window_settings_hotkey_editor_show(action, 0, set_hotkey);
        }
    } else {
        if (nk_button_label(ctx, gettext("Not set"))) {
            window_settings_hotkey_editor_show(action, 0, set_hotkey);
        }
    }
    nk_layout_row_push(ctx, 0.34f);
    const hotkey_mapping *mapping2 = &hotkey_data.mappings[action][1];
    if (mapping2->key) {
        if (nk_button_label(ctx, key_combination_display_name(mapping2->key, mapping2->modifiers))) {
            window_settings_hotkey_editor_show(action, 1, set_hotkey);
        }
    } else {
        if (nk_button_label(ctx, gettext("Not set"))) {
            window_settings_hotkey_editor_show(action, 1, set_hotkey);
        }
    }
}

static void reset_bindings(void) {
    for (int action = 0; action < HOTKEY_MAX_ITEMS; action++) {
        for (int index = 0; index < 2; index++) {
            hotkey_data.mappings[action][index] = *hotkey_default_for_action(action, index);
        }
    }
}

static void draw_foreground(void) {
    struct nk_context *ctx = ui_context();
    int w = 877;
    int h = screen_height() - 30;
    ui_font_change(FONT_TYPE_LARGE_BOLD);
    if (nk_begin_titled(ctx, "settings_hotkeys", gettext("Controls Settings"),
                        nk_recti((screen_width() / 2) - w / 2, 15, w, h),
                        NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE)) {
        ui_font_change(FONT_TYPE_STANDARD);
        nk_layout_space_begin(ctx, NK_STATIC, h - 40, 2);
        nk_layout_space_push(ctx, nk_rect(0, 0, w - 15, h - 110));
        if (nk_group_begin(ctx, "group_general", NK_WINDOW_BORDER)) {
            create_hotkey_widget(ctx, HOTKEY_ARROW_UP, gettext("Arrow up"));
            create_hotkey_widget(ctx, HOTKEY_ARROW_DOWN, gettext("Arrow down"));
            create_hotkey_widget(ctx, HOTKEY_ARROW_LEFT, gettext("Arrow left"));
            create_hotkey_widget(ctx, HOTKEY_ARROW_RIGHT, gettext("Arrow right"));
            create_hotkey_widget(ctx, HOTKEY_TOGGLE_FULLSCREEN, gettext("Toggle fullscreen"));
            create_hotkey_widget(ctx, HOTKEY_CENTER_WINDOW, gettext("Center window"));
            create_hotkey_widget(ctx, HOTKEY_SAVE_SCREENSHOT, gettext("Screenshot"));
            create_hotkey_widget(ctx, HOTKEY_LOAD_FILE, gettext("Load game"));
            create_hotkey_widget(ctx, HOTKEY_SAVE_FILE, gettext("Save game"));
            create_hotkey_widget(ctx, HOTKEY_INCREASE_GAME_SPEED, gettext("Increase game speed"));
            create_hotkey_widget(ctx, HOTKEY_DECREASE_GAME_SPEED, gettext("Decrease game speed"));
            create_hotkey_widget(ctx, HOTKEY_TOGGLE_PAUSE, gettext("Toggle pause"));
            create_hotkey_widget(ctx, HOTKEY_ROTATE_MAP_LEFT, gettext("Rotate map left"));
            create_hotkey_widget(ctx, HOTKEY_ROTATE_MAP_RIGHT, gettext("Rotate map right"));
            create_hotkey_widget(ctx, HOTKEY_ROTATE_MAP_NORTH, gettext("Rotate map north"));
            create_hotkey_widget(ctx, HOTKEY_ROTATE_BUILDING, gettext("Rotate building"));
            create_hotkey_widget(ctx, HOTKEY_ROTATE_BUILDING_BACK, gettext("Rotate building back"));
            create_hotkey_widget(ctx, HOTKEY_SHOW_EMPIRE_MAP, gettext("Show map"));
            create_hotkey_widget(ctx, HOTKEY_SHOW_MESSAGES, gettext("Show messages"));
            create_hotkey_widget(ctx, HOTKEY_BUILD_CLONE, gettext("Build clone"));
            create_hotkey_widget(ctx, HOTKEY_COPY_BUILDING_SETTINGS, gettext("Copy building settings"));
            create_hotkey_widget(ctx, HOTKEY_PASTE_BUILDING_SETTINGS, gettext("Paste building settings"));
            create_hotkey_widget(ctx, HOTKEY_MOTHBALL_TOGGLE, gettext("Mothball/Enable building"));
            create_hotkey_widget(ctx, HOTKEY_STORAGE_ORDER, gettext("Show building special orders"));
            create_hotkey_widget(ctx, HOTKEY_BUILD_CLEAR_LAND, gettext("Clear land"));
            create_hotkey_widget(ctx, HOTKEY_BUILD_VACANT_HOUSE, gettext("Build house"));
            create_hotkey_widget(ctx, HOTKEY_BUILD_ROAD, gettext("Build road"));
            create_hotkey_widget(ctx, HOTKEY_BUILD_PLAZA, gettext("Build plaza"));
            create_hotkey_widget(ctx, HOTKEY_BUILD_GARDENS, gettext("Build gardens"));
            create_hotkey_widget(ctx, HOTKEY_BUILD_PREFECTURE, gettext("Build prefecture"));
            create_hotkey_widget(ctx, HOTKEY_BUILD_ENGINEERS_POST, gettext("Build engineers post"));
            create_hotkey_widget(ctx, HOTKEY_BUILD_DOCTOR, gettext("Build doctor"));
            create_hotkey_widget(ctx, HOTKEY_BUILD_BARBER, gettext("Build barber"));
            create_hotkey_widget(ctx, HOTKEY_BUILD_GRANARY, gettext("Build granary"));
            create_hotkey_widget(ctx, HOTKEY_BUILD_WAREHOUSE, gettext("Build warehouse"));
            create_hotkey_widget(ctx, HOTKEY_BUILD_MARKET, gettext("Build market"));
            create_hotkey_widget(ctx, HOTKEY_BUILD_RESERVOIR, gettext("Build reservoir"));
            create_hotkey_widget(ctx, HOTKEY_BUILD_ROADBLOCK, gettext("Build roadblock"));
            create_hotkey_widget(ctx, HOTKEY_BUILD_WHEAT_FARM, gettext("Build wheat farm"));
            create_hotkey_widget(ctx, HOTKEY_UNDO, gettext("Undo"));
            create_hotkey_widget(ctx, HOTKEY_TOGGLE_OVERLAY, gettext("Toggle current overlay"));
            create_hotkey_widget(ctx, HOTKEY_GO_TO_BOOKMARK_1, gettext("Bookmark 1"));
            create_hotkey_widget(ctx, HOTKEY_GO_TO_BOOKMARK_2, gettext("Bookmark 2"));
            create_hotkey_widget(ctx, HOTKEY_GO_TO_BOOKMARK_3, gettext("Bookmark 3"));
            create_hotkey_widget(ctx, HOTKEY_GO_TO_BOOKMARK_4, gettext("Bookmark 4"));
            create_hotkey_widget(ctx, HOTKEY_SET_BOOKMARK_1, gettext("Set bookmark 1"));
            create_hotkey_widget(ctx, HOTKEY_SET_BOOKMARK_2, gettext("Set bookmark 2"));
            create_hotkey_widget(ctx, HOTKEY_SET_BOOKMARK_3, gettext("Set bookmark 3"));
            create_hotkey_widget(ctx, HOTKEY_SET_BOOKMARK_4, gettext("Set bookmark 4"));
            nk_group_end(ctx);
        }
        ui_font_change(FONT_TYPE_LARGE_BOLD);
        nk_layout_space_push(ctx, nk_rect(0, h - 95, 865, 65));
        if (nk_group_begin(ctx, "group_bottom_left", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 5);
            {
                nk_layout_row_push(ctx, 0.2f);
                if (nk_button_label(ctx, gettext("Back"))) {
                    window_go_back();
                }
                nk_layout_row_push(ctx, 0.2f);
                nk_spacer(ctx);
                nk_layout_row_push(ctx, 0.2f);
                if (nk_button_label(ctx, gettext("Reset bindings"))) {
                    reset_bindings();
                }
                nk_layout_row_push(ctx, 0.2f);
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

void window_settings_hotkey_show(void) {
    settings_hotkey_init();
    window_type window = {
            WINDOW_SETTINGS_HOTKEYS,
            draw_background,
            draw_foreground,
            handle_input
    };
    window_show(&window);
}
