#include <assert.h>
#include "popup_dialog.h"

#include "core/image_group.h"
#include "core/lang.h"
#include "graphics/generic_button.h"
#include "graphics/graphics.h"
#include "graphics/window.h"
#include "input/input.h"
#include "translation/translation.h"
#include "window/ui_window.h"
#include "window/nuklear.h"
#include "graphics/screen.h"
#include "graphics/image.h"
#include "assets/assets.h"

#define GROUP 5

#define PROCEED_GROUP 43
#define PROCEED_TEXT 5
#define CHECKBOX_CHECK_SIZE 20

static void button_checkbox(int param1, int param2);

static void button_ok(int param1, int param2);

static void button_cancel(int param1, int param2);

static void confirm(void);

static generic_button checkbox = {160, 180, 360, 20, button_checkbox, button_none};

static struct {
    int ok_clicked;

    void (*close_func)(int accepted, int checked);

    int has_buttons;
    int translation_key;
    int checked;
    int has_focus;
    const uint8_t *custom_title;
    const uint8_t *custom_text;
    const uint8_t *checkbox_text;
} data;

static int init(const uint8_t *custom_title, const uint8_t *custom_text,
                const uint8_t *checkbox_text, void (*close_func)(int accepted, int checked),
                int has_ok_cancel_buttons) {
    if (window_is(WINDOW_POPUP_DIALOG)) {
        // don't show popup over popup
        return 0;
    }
    data.ok_clicked = 0;
    data.close_func = close_func;
    data.has_buttons = has_ok_cancel_buttons;
    data.custom_title = custom_title;
    data.custom_text = custom_text;
    data.checkbox_text = checkbox_text;
    data.checked = 0;
    if (!data.custom_text) {
        data.custom_text = lang_get_string(PROCEED_GROUP, PROCEED_TEXT);
    }
    return 1;
}

static void draw_background(void) {
    image_draw_fullscreen_background(assets_get_image_id("mainmenu", "start-screen"));
    if (data.checkbox_text) {
        assert(1 != 0);
    }
/*
    graphics_in_dialog();
    outer_panel_draw(80, 80, 30, data.checkbox_text ? 11 : 10);
    if (data.custom_title) {
        text_draw_centered(data.custom_title, 80, 100, 480, FONT_LARGE_BLACK, 0);
    }
    if (text_get_width(data.custom_text, FONT_NORMAL_BLACK) >= 420) {
        text_draw_multiline(data.custom_text, 110, 140, 420, FONT_NORMAL_BLACK, 0);
    } else {
        text_draw_centered(data.custom_text, 80, 140, 480, FONT_NORMAL_BLACK, 0);
    }
    if (data.checkbox_text) {
        assert(1 != 0);
        if (data.checked) {
            text_draw(string_from_ascii("x"), data.checkbox_start_width + 6, 183, FONT_NORMAL_BLACK, 0);
        }
        text_draw(data.checkbox_text, data.checkbox_start_width + 30, 184, FONT_NORMAL_BLACK, 0);
    }
    graphics_reset_dialog();*/
}

static void draw_foreground(void) {
    if (data.checkbox_text) {
        assert(1 != 0);
        // button_border_draw(data.checkbox_start_width, 180, CHECKBOX_CHECK_SIZE, CHECKBOX_CHECK_SIZE, data.has_focus);
    }
    /*
    if (data.has_buttons) {
        image_buttons_draw(80, data.checkbox_text ? 110 : 80, buttons, 2);
    } else {
        lang_text_draw_centered(13, 1, 80, 208, 480, FONT_NORMAL_BLACK);
    }*/
    struct nk_context *ctx = ui_context();
    if (nk_begin(ctx, "popup_dialog", nk_recti(screen_dialog_offset_x() + 80, screen_dialog_offset_y() + 80, 480, 160),
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 60, 1);
        ui_font_extra_large_bold();
        nk_label(ctx, (char *) data.custom_title, NK_TEXT_CENTERED);
        nk_layout_row_dynamic(ctx, 20, 2);
        nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 3);
        {
            ui_font_large_bold();
            nk_layout_row_push(ctx, 0.1);
            nk_spacer(ctx);
            nk_layout_row_push(ctx, 0.3);
            if (nk_button_label(ctx, (char *) lang_get_string(CUSTOM_TRANSLATION, TR_BUTTON_OK))) {
                button_ok(0, 0);
            }
            nk_layout_row_push(ctx, 0.2);
            nk_spacer(ctx);
            nk_layout_row_push(ctx, 0.3);
            if (nk_button_label(ctx, (char *) lang_get_string(CUSTOM_TRANSLATION, TR_BUTTON_CANCEL))) {
                button_cancel(0, 0);
            }
            nk_layout_row_push(ctx, 0.1);
            nk_layout_row_end(ctx);
        }
    }
    nk_end(ctx);
    ui_font_standard();
}

static void handle_input(const mouse *m, const hotkeys *h) {
    if (input_go_back_requested(m, h)) {
        data.close_func(0, 0);
        window_go_back();
    }
    if (h->enter_pressed) {
        confirm();
    }
}

static void button_ok(int param1, int param2) {
    confirm();
}

static void button_cancel(int param1, int param2) {
    window_go_back();
    data.close_func(0, 0);
}

static void button_checkbox(int param1, int param2) {
    data.checked ^= 1;
    window_request_refresh();
}

static void confirm(void) {
    window_go_back();
    data.close_func(1, data.checked);
}

void window_popup_dialog_show(popup_dialog_type type,
                              void (*close_func)(int accepted, int checked), int has_ok_cancel_buttons) {
    if (init(lang_get_string(GROUP, type), lang_get_string(GROUP, type + 1), 0, close_func, has_ok_cancel_buttons)) {
        window_type window = {
                WINDOW_POPUP_DIALOG,
                draw_background,
                draw_foreground,
                handle_input
        };
        window_show(&window);
    }
}

void window_popup_dialog_show_confirmation(const uint8_t *custom_title, const uint8_t *custom_text,
                                           const uint8_t *checkbox_text,
                                           void (*close_func)(int accepted, int checked)) {
    if (init(custom_title, custom_text, checkbox_text, close_func, 1)) {
        window_type window = {
                WINDOW_POPUP_DIALOG,
                draw_background,
                draw_foreground,
                handle_input
        };
        window_show(&window);
    }
}
