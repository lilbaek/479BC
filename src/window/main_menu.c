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
#include "window/file_dialog.h"
#include "window/plain_message_dialog.h"
#include "window/popup_dialog.h"
#include "window/nuklear.h"
#include "window/ui_window.h"
#include "map_generator.h"

static struct {
    int focus_button_id;
    int logo_image_id;
} data;

static void draw_version_string(void) {
    uint8_t version_string[100] = "tiberius v";
    int version_prefix_length = string_length(version_string);
    int text_y = screen_height() - 54;

    string_copy(string_from_ascii(system_version()), version_string + version_prefix_length, 99);

    int text_width = text_get_width(version_string, FONT_NORMAL_WHITE);
    int width = calc_value_in_step(text_width + 20, 16);

    text_draw_centered(version_string, 20, text_y + 11, width, FONT_NORMAL_WHITE, 0);
}

static void draw_background(void) {
    graphics_reset_dialog();
    graphics_reset_clip_rectangle();
    image_draw_fullscreen_background(assets_get_image_id("mainmenu", "start-screen"));
    draw_version_string();
}

static void confirm_exit(int accepted) {
    if (accepted) {
        system_exit();
    }
}

static void draw_foreground(void) {
    struct nk_context *ctx = ui_context();
    ui_font_large_bold();
    if (nk_begin(ctx, "main_menu", nk_recti(screen_dialog_offset_x() + 192, screen_dialog_offset_y() + 70, 260, 258),
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_begin(ctx, NK_STATIC, 45, 1);
        {
            float width = 247;
            nk_layout_row_push(ctx, width);
            if (nk_button_label(ctx, gettext("New game"))) {
                window_map_generator_show();
            }
            nk_layout_row_push(ctx, width);
            if (nk_button_label(ctx, gettext("Load saved game"))) {
                window_file_dialog_show(FILE_TYPE_SAVED_GAME, FILE_DIALOG_LOAD);
            }
            nk_layout_row_push(ctx, width);
            if (nk_button_label(ctx, gettext("Map editor"))) {
                game_init_editor();
                sound_music_play_editor();
            }
            nk_layout_row_push(ctx, width);
            if (nk_button_label(ctx, gettext("Options"))) {
                window_config_show(CONFIG_FIRST_PAGE, 1);
            }
            nk_layout_row_push(ctx, width);
            if (nk_button_label(ctx, gettext("Exit"))) {
                window_popup_dialog_show_ex(gettext("Quit"), gettext("Are you sure you want to quit?"), confirm_exit, 1);
            }
        }
        nk_layout_row_end(ctx);
    }
    nk_end(ctx);
    ui_font_standard();
}

static void handle_input(const mouse *m, const hotkeys *h) {
    if (h->escape_pressed) {
        hotkey_handle_escape();
    }
    if (h->load_file) {
        window_file_dialog_show(FILE_TYPE_SAVED_GAME, FILE_DIALOG_LOAD);
    }
}

void window_main_menu_show(int restart_music) {
    if (restart_music) {
        sound_music_play_intro();
    }
    window_type window = {
            WINDOW_MAIN_MENU,
            draw_background,
            draw_foreground,
            handle_input
    };
    window_show(&window);
}
