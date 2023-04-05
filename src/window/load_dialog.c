#include "load_dialog.h"
#include <stdio.h>
#include <string.h>
#include "graphics/window.h"
#include "graphics/image.h"
#include "assets/assets.h"
#include "ui_window.h"
#include "nuklear.h"
#include "graphics/screen.h"
#include "translation/translation.h"
#include "city.h"
#include "core/file.h"
#include "platform/file_manager.h"
#include "game/save_version.h"
#include "game/file_io.h"
#include "file_dialog.h"
#include "core/string.h"
#include "widget/minimap.h"
#include "game/file.h"
#include "popup_dialog.h"

typedef struct {
    char extension[5];
} file_type_data;

static struct {
    time_millis message_not_exist_start_time;
    file_type type;
    int minimap_x;
    int minimap_y;
    const dir_listing *file_list;
    file_type_data *file_data;
    uint8_t typed_name[FILE_NAME_MAX];
    char selected_file[FILE_NAME_MAX];
    union {
        saved_game_info save_game;
    } info;
    savegame_load_status savegame_info_status;
} load_dialog_data;

static file_type_data saved_game_data_expanded = {"fvx"};

static int find_first_file_with_prefix(const char *prefix) {
    int len = (int) strlen(prefix);
    if (len == 0) {
        return -1;
    }
    int left = 0;
    int right = load_dialog_data.file_list->num_files;
    while (left < right) {
        int middle = (left + right) / 2;
        if (platform_file_manager_compare_filename_prefix(load_dialog_data.file_list->files[middle], prefix, len) >= 0) {
            right = middle;
        } else {
            left = middle + 1;
        }
    }
    if (platform_file_manager_compare_filename_prefix(load_dialog_data.file_list->files[left], prefix, len) == 0) {
        return left;
    } else {
        return -1;
    }
}

void init_load_dialog() {
    load_dialog_data.file_data = &saved_game_data_expanded;
    load_dialog_data.typed_name[0] = 0;
    load_dialog_data.file_list = dir_find_files_with_extension(".", saved_game_data_expanded.extension);
}

static void draw_background(void) {
    image_draw_fullscreen_background(assets_get_image_id("mainmenu", "start-screen"));
}

static void nothing(int _) {

}

static void load_map() {
    if (load_dialog_data.savegame_info_status != SAVEGAME_STATUS_OK) {
        window_popup_dialog_show_ex(gettext("Unable to load game"),
                                    gettext("Select game to load"), nothing, 0);
        return;
    }
    if (NULL == file_open_save_folder(load_dialog_data.selected_file, "rb")) {
        window_popup_dialog_show_ex(gettext("Unable to load game"),
                                    gettext("File does not exist"), nothing, 0);
        return;
    }
    int result = game_file_load_saved_game(load_dialog_data.selected_file);
    if (result == 1) {
        window_city_show();
    } else if (result == 0) {
        window_popup_dialog_show_ex(gettext("Unable to load game"),
                                    gettext("File does not exist"), nothing, 0);
        return;
    } else if (result == -1) {
        window_popup_dialog_show_ex(gettext("Unable to load game"),
                                    gettext("Upgrade game to load this file"), nothing, 0);
        return;
    }
}

static void create_selectable(struct nk_context *ctx) {
    char file[FILE_NAME_MAX];
    static int selected = -1;
    nk_layout_row_static(ctx, 18, 230, 1);
    for (int i = 0; i < load_dialog_data.file_list->num_files; ++i) {
        encoding_from_utf8(load_dialog_data.file_list->files[i], file, FILE_NAME_MAX);
        nk_bool is_selected = (i == selected);
        if (nk_selectable_label(ctx, file, NK_TEXT_ALIGN_LEFT, &is_selected)) {
            selected = (selected == i) ? -1 : i;
            strncpy(load_dialog_data.selected_file, load_dialog_data.file_list->files[i], FILE_NAME_MAX - 1);
            load_dialog_data.savegame_info_status = game_file_io_read_saved_game_info(load_dialog_data.selected_file,
                                                                          &load_dialog_data.info.save_game);
        }
    }
}

static void create_map_info(struct nk_context *ctx) {
    nk_layout_row_static(ctx, 300, 280, 1);
    nk_layout_row_static(ctx, 25, 100, 3);
    {
        nk_spacer(ctx);

        struct nk_rect rect = nk_widget_bounds(ctx);
        load_dialog_data.minimap_x = rect.x - 45;
        load_dialog_data.minimap_y = rect.y - 300;
        nk_label(ctx, gettext("Funds:"), NK_TEXT_ALIGN_LEFT);

        char text[100];
        sprintf(text, "%d Db", load_dialog_data.info.save_game.treasury);
        nk_label(ctx, text, NK_TEXT_ALIGN_LEFT);
    }
    nk_layout_row_static(ctx, 25, 100, 3);
    {
        nk_spacer(ctx);
        nk_label(ctx, gettext("Population:"), NK_TEXT_ALIGN_LEFT);

        char text[100];
        sprintf(text, "%d", load_dialog_data.info.save_game.population);
        nk_label(ctx, text, NK_TEXT_ALIGN_LEFT);
    }
}

static void create_buttons(struct nk_context *ctx) {
    if (nk_button_label(ctx, gettext("Cancel"))) {
        window_go_back();
    }
    struct nk_style_button button;
    button = ctx->style.button;
    ctx->style.button.normal.data.color = nk_rgb(156, 39, 176);
    ctx->style.button.hover.data.color = nk_rgb(164, 56, 182);
    ctx->style.button.text_normal = nk_rgb(255, 255, 255);
    ctx->style.button.text_hover = nk_rgb(255, 255, 255);
    ctx->style.button.text_active = nk_rgb(255, 255, 255);
    if (nk_button_label(ctx, gettext("Load"))) {
        load_map();
    }
    ctx->style.button = button;
}

static void draw_foreground(void) {
    struct nk_context *ctx = ui_context();
    int w = 677;
    int h = 500;
    if (nk_begin(ctx, gettext("Load save"),
                 nk_recti((screen_width() / 2) - w / 2, (screen_height() / 2) - h / 2, w, h),
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE)) {
        nk_layout_space_begin(ctx, NK_STATIC, h - 40, 3);
        nk_layout_space_push(ctx, nk_rect(0, 0, 250, h - 40));
        if (nk_group_begin(ctx, "Group_left", NK_WINDOW_BORDER)) {
            create_selectable(ctx);
            nk_group_end(ctx);
        }
        nk_layout_space_push(ctx, nk_rect(260, 0, 405, h - 100));
        if (nk_group_begin(ctx, "Group_Right", NK_WINDOW_BORDER)) {
            if (load_dialog_data.savegame_info_status == SAVEGAME_STATUS_OK) {
                create_map_info(ctx);
            }
            nk_group_end(ctx);
        }
        nk_layout_space_push(ctx, nk_rect(455, h - 90, 405, 65));
        if (nk_group_begin(ctx, "Group_Bottom", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_static(ctx, 40, 100, 2);
            {
                create_buttons(ctx);
            }
            nk_group_end(ctx);
        }
    }
    nk_end(ctx);
    ui_font_standard();
}

static void draw_top(void) {
    if (load_dialog_data.savegame_info_status == SAVEGAME_STATUS_OK) {
        widget_minimap_draw(load_dialog_data.minimap_x, load_dialog_data.minimap_y, 266, 272);
    }
}

static void handle_input(const mouse *m, const hotkeys *h) {
    if (h->escape_pressed) {
        window_go_back();
    }
}

static void (*get_tooltip)(tooltip_context *c);

void window_load_dialog_show() {
    init_load_dialog();
    window_type window = {
            WINDOW_LOAD_DIALOG,
            draw_background,
            draw_foreground,
            handle_input,
            get_tooltip,
            draw_top,
    };
    window_show(&window);
}
