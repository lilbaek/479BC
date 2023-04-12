#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#include "save_load_dialog.h"
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
#include "core/string.h"
#include "widget/minimap.h"
#include "game/file.h"
#include "popup_dialog.h"

typedef struct {
    char extension[5];
} file_type_data;

static struct {
    int selected_file_index;
    int save_dialog;
    int minimap_x;
    int minimap_y;
    const dir_listing *file_list;
    file_type_data *file_data;
    char typed_name[FILE_NAME_MAX];
    char selected_file[FILE_NAME_MAX];
    union {
        saved_game_info save_game;
    } info;
    savegame_load_status savegame_info_status;
} load_dialog_data;

static file_type_data saved_game_data_expanded = {"fvx"};

void init_dialog(int save_dialog) {
    load_dialog_data.file_data = &saved_game_data_expanded;
    load_dialog_data.file_list = dir_find_files_with_extension(SAVE_DIRECTORY, saved_game_data_expanded.extension);
    load_dialog_data.save_dialog = save_dialog;
    load_dialog_data.selected_file_index = -1;
}

static void draw_background(void) {
    image_draw_fullscreen_background(assets_get_image_id("mainmenu", "start-screen"));
}

static void nothing(int _) {

}

static char *get_chosen_filename(void) {
    // Check if we should work with the selected file
    uint8_t selected_name[FILE_NAME_MAX];
    encoding_from_utf8(load_dialog_data.selected_file, selected_name, FILE_NAME_MAX);

    if (string_equals(selected_name, load_dialog_data.typed_name)) {
        // User has not modified the string after selecting it: use filename
        return load_dialog_data.selected_file;
    }

    // We should use the typed name, which needs to be converted to UTF-8...
    static char typed_file[FILE_NAME_MAX];
    encoding_to_utf8(load_dialog_data.typed_name, typed_file, FILE_NAME_MAX, encoding_system_uses_decomposed());
    return typed_file;
}

static void save_map() {
    char filename[FILE_NAME_MAX];
    char *chosen_filename = get_chosen_filename();
    memset(filename, 0, sizeof(filename));
    strncat(filename, chosen_filename, sizeof(filename) - strlen(filename) - 1);
    if (!file_has_extension(filename, saved_game_data_expanded.extension)) {
        file_append_extension(filename, saved_game_data_expanded.extension);
    }
    if (!game_file_write_saved_game(filename)) {
        window_popup_dialog_show_ex(gettext("Unable to save game"),
                                    "", nothing, 0);
        return;
    }
    strncpy(load_dialog_data.selected_file, filename, FILE_NAME_MAX - 1);
    window_city_show();
}

static void load_map() {
    if (load_dialog_data.savegame_info_status != SAVEGAME_STATUS_OK) {
        window_popup_dialog_show_ex(gettext("Unable to load game"),
                                    gettext("Select game to load"), nothing, 0);
        return;
    }
    window_popup_dialog_show_ex(gettext("Unable to load game"),
                                gettext("File does not exist"), nothing, 0);
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
    if (load_dialog_data.save_dialog) {
        nk_layout_row_static(ctx, 18, 210, 1);
    } else {
        nk_layout_row_static(ctx, 18, 230, 1);
    }
    for (int i = 0; i < load_dialog_data.file_list->num_files; ++i) {
        encoding_from_utf8(load_dialog_data.file_list->files[i], file, FILE_NAME_MAX);
        nk_bool is_selected = (i == load_dialog_data.selected_file_index);
        if (nk_selectable_label(ctx, file, NK_TEXT_ALIGN_LEFT, &is_selected)) {
            load_dialog_data.selected_file_index = (load_dialog_data.selected_file_index == i) ? -1 : i;
            strncpy(load_dialog_data.selected_file, load_dialog_data.file_list->files[i], FILE_NAME_MAX - 1);
            strncpy(load_dialog_data.typed_name, load_dialog_data.file_list->files[i], FILE_NAME_MAX - 1);
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

static void create_ok_button(struct nk_context *ctx) {
    struct nk_style_button button;
    button = ctx->style.button;
    ctx->style.button.normal.data.color = nk_rgb(156, 39, 176);
    ctx->style.button.hover.data.color = nk_rgb(164, 56, 182);
    ctx->style.button.text_normal = nk_rgb(255, 255, 255);
    ctx->style.button.text_hover = nk_rgb(255, 255, 255);
    ctx->style.button.text_active = nk_rgb(255, 255, 255);
    if (load_dialog_data.save_dialog) {
        if (nk_button_label(ctx, gettext("Save"))) {
            save_map();
        }
    } else {
        if (nk_button_label(ctx, gettext("Load"))) {
            load_map();
        }
    }
    ctx->style.button = button;
}

static void draw_foreground(void) {
    struct nk_context *ctx = ui_context();
    int w = 677;
    int h = 500;
    char *title = gettext("Load save");
    if (load_dialog_data.save_dialog) {
        title = gettext("Save game");
    }
    ui_font_change(FONT_TYPE_LARGE_BOLD);;
    if (nk_begin_titled(ctx, "file_dialog", title,
                        nk_recti((screen_width() / 2) - w / 2, (screen_height() / 2) - h / 2, w, h),
                        NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE)) {
        ui_font_change(FONT_TYPE_STANDARD);
        nk_layout_space_begin(ctx, NK_STATIC, h - 40, 3);
        nk_layout_space_push(ctx, nk_rect(0, 0, 250, h - 105));
        if (nk_group_begin(ctx, "Group_left", NK_WINDOW_BORDER)) {
            if (load_dialog_data.save_dialog) {
                nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 3);
                nk_layout_row_push(ctx, 0.3f);
                nk_label(ctx, gettext("Save as:"), NK_TEXT_LEFT);
                nk_layout_row_push(ctx, 0.7f);
                nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, load_dialog_data.typed_name, FILE_NAME_MAX,
                                               nk_filter_default);
                nk_layout_row_end(ctx);
            }
            create_selectable(ctx);
            nk_group_end(ctx);
        }
        nk_layout_space_push(ctx, nk_rect(260, 0, 405, h - 105));
        if (nk_group_begin(ctx, "Group_Right", NK_WINDOW_BORDER)) {
            if (load_dialog_data.savegame_info_status == SAVEGAME_STATUS_OK) {
                create_map_info(ctx);
            }
            nk_group_end(ctx);
        }
        ui_font_change(FONT_TYPE_LARGE_BOLD);;
        nk_layout_space_push(ctx, nk_rect(0, h - 95, 665, 65));
        if (nk_group_begin(ctx, "group_bottom_left", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 3);
            {
                nk_layout_row_push(ctx, 0.2f);
                if (nk_button_label(ctx, gettext("Back"))) {
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

void window_save_load_dialog_show(int save_dialog) {
    init_dialog(save_dialog);
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
