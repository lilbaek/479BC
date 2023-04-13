#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#include "settings.h"
#include "main_menu.h"
#include "assets/assets.h"
#include "core/calc.h"
#include "core/string.h"
#include "game/game.h"
#include "game/system.h"
#include "graphics/generic_button.h"
#include "graphics/graphics.h"
#include "graphics/image.h"
#include "graphics/lang_text.h"
#include "graphics/text.h"
#include "graphics/screen.h"
#include "graphics/window.h"
#include "sound/music.h"
#include "window/config.h"

#include "window/plain_message_dialog.h"
#include "window/popup_dialog.h"
#include "window/nuklear.h"
#include "window/ui_window.h"
#include "map_generator.h"
#include "save_load_dialog.h"
#include "settings_general.h"
#include "settings_gameplay.h"
#include "hotkey_config.h"
#include "settings_hotkey.h"

static void draw_background(void) {
    image_draw_fullscreen_background(assets_get_image_id("mainmenu", "start-screen"));
}

static void confirm_exit(int accepted) {
    if (accepted) {
        system_exit();
    }
}

static void draw_foreground(void) {
    struct nk_context *ctx = ui_context();
    ui_font_change(FONT_TYPE_LARGE_BOLD);;
    int h = 253;
    int w = 270;
    if (nk_begin(ctx, "Settings", nk_recti((screen_width() / 2) - w / 2, (screen_height() / 2) - h / 2, w, h),
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE)) {
        nk_layout_space_begin(ctx, NK_STATIC, h - 40, 3);
        nk_layout_space_push(ctx, nk_rect(0, 0, w- 13, h - 50));
        if (nk_group_begin(ctx, "Group_left", NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR))
        {
            nk_layout_row_begin(ctx, NK_STATIC, 45, 1);
            {
                float width = 249;
                nk_layout_row_push(ctx, width);
                if (nk_button_label(ctx, gettext("General"))) {
                    window_settings_general_show();
                }
                nk_layout_row_push(ctx, width);
                if (nk_button_label(ctx, gettext("Gameplay"))) {
                    window_settings_gameplay_show();
                }
                nk_layout_row_push(ctx, width);
                if (nk_button_label(ctx, gettext("Controls"))) {
                    window_settings_hotkey_show();
                }
                nk_layout_row_push(ctx, width);
                if (nk_button_label(ctx, gettext("Back"))) {
                    window_go_back();
                }
            }
            nk_layout_row_end(ctx);
            nk_group_end(ctx);
        }
    }
    nk_end(ctx);
    ui_font_change(FONT_TYPE_STANDARD);
}

static void handle_input(const mouse *m, const hotkeys *h) {
    if (h->escape_pressed) {
        window_go_back();
    }
}

void window_settings_menu_show() {
    window_type window = {
            WINDOW_SETTINGS_MENU,
            draw_background,
            draw_foreground,
            handle_input
    };
    window_show(&window);
}
