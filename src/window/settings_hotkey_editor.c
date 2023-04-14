#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#include "settings_hotkey_editor.h"
#include "graphics/window.h"
#include "graphics/image.h"
#include "assets/assets.h"
#include "nuklear.h"
#include "ui_window.h"
#include "graphics/screen.h"
#include "translation/translation.h"

static struct {
    hotkey_action action;
    int index;
    key_type key;
    key_modifier_type modifiers;
    void (*callback)(hotkey_action, int, key_type, key_modifier_type);
    int focus_button;
} settings_hotkey_editor_data;

static void settings_hotkey_editor_init(hotkey_action action, int index,
                                        void (*callback)(hotkey_action, int, key_type, key_modifier_type)) {
    settings_hotkey_editor_data.action = action;
    settings_hotkey_editor_data.index = index;
    settings_hotkey_editor_data.callback = callback;
    settings_hotkey_editor_data.key = KEY_TYPE_NONE;
    settings_hotkey_editor_data.modifiers = KEY_MOD_NONE;
    settings_hotkey_editor_data.focus_button = 0;
}


static void draw_background(void) {
    window_draw_underlying_window();
}


static void save_data() {
    window_go_back();
    settings_hotkey_editor_data.callback(settings_hotkey_editor_data.action, settings_hotkey_editor_data.index,
                                         settings_hotkey_editor_data.key, settings_hotkey_editor_data.modifiers);
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
        save_data();
    }
    ctx->style.button = button;
}

static void close_without_saving() { window_go_back(); }

static void draw_foreground(void) {
    struct nk_context *ctx = ui_context();
    int w = 350;
    int h = 200;
    ui_font_change(FONT_TYPE_LARGE_BOLD);
    if (nk_begin_titled(ctx, "settings_hotkey_editor", gettext("Press new hotkey"),
                        nk_recti((screen_width() / 2) - w / 2, (screen_height() / 2) - h / 2, w, h),
                        NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE)) {
        ui_font_change(FONT_TYPE_STANDARD);
        nk_layout_space_begin(ctx, NK_STATIC, h - 40, 2);
        nk_layout_space_push(ctx, nk_rect(0, 0, w - 15, h - 100));
        if (nk_group_begin(ctx, "group_top", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(ctx, 90, 1);
            ui_font_change(FONT_TYPE_LARGE_BOLD);
            nk_label(ctx, key_combination_display_name(settings_hotkey_editor_data.key,
                                                       settings_hotkey_editor_data.modifiers), NK_TEXT_CENTERED);
            ui_font_change(FONT_TYPE_STANDARD);
            nk_group_end(ctx);
        }
        ui_font_change(FONT_TYPE_LARGE_BOLD);
        nk_layout_space_push(ctx, nk_rect(0, h - 95, w - 15, 65));
        if (nk_group_begin(ctx, "group_bottom_left", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 3);
            {
                nk_layout_row_push(ctx, 0.3f);
                if (nk_button_label(ctx, gettext("Back"))) {
                    close_without_saving();
                }
                nk_layout_row_push(ctx, 0.4f);
                nk_spacer(ctx);
                nk_layout_row_push(ctx, 0.3f);
                create_ok_button(ctx);
                nk_layout_row_end(ctx);
            }
            nk_group_end(ctx);
        }
    }
    nk_end(ctx);
    ui_font_change(FONT_TYPE_STANDARD);
}


void window_settings_hotkey_editor_key_pressed(key_type key, key_modifier_type modifiers) {
    if (key == KEY_TYPE_ENTER && modifiers == KEY_MOD_NONE) {
        save_data();
    } else if (key == KEY_TYPE_ESCAPE && modifiers == KEY_MOD_NONE) {
        close_without_saving();
    } else {
        if (key != KEY_TYPE_NONE) {
            settings_hotkey_editor_data.key = key;
        }
        settings_hotkey_editor_data.modifiers = modifiers;
    }
}

void window_settings_hotkey_editor_key_released(key_type key, key_modifier_type modifiers) {
// update modifiers as long as we don't have a proper keypress
    if (settings_hotkey_editor_data.key == KEY_TYPE_NONE && key == KEY_TYPE_NONE) {
        settings_hotkey_editor_data.modifiers = modifiers;
    }
}


static void handle_input(const mouse *m, const hotkeys *h) {

}

void window_settings_hotkey_editor_show(hotkey_action action, int index,
                                        void (*callback)(hotkey_action, int, key_type, key_modifier_type)) {
    window_type window = {
            WINDOW_SETTINGS_HOTKEY_EDITOR,
            draw_background,
            draw_foreground,
            handle_input
    };
    settings_hotkey_editor_init(action, index, callback);
    window_show(&window);
}