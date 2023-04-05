#include "popup_dialog.h"
#include "core/lang.h"
#include "graphics/generic_button.h"
#include "graphics/window.h"
#include "input/input.h"
#include "translation/translation.h"
#include "window/ui_window.h"
#include "window/nuklear.h"
#include "graphics/screen.h"

#define GROUP 5

#define PROCEED_GROUP 43
#define PROCEED_TEXT 5
#define CHECKBOX_CHECK_SIZE 20

static void button_ok(int param1, int param2);

static void button_cancel(int param1, int param2);

static void confirm(void);

static struct {
    int ok_clicked;

    void (*close_func)(int accepted);

    int has_buttons;
    int translation_key;
    int checked;
    int has_focus;
    const uint8_t *custom_title;
    const uint8_t *custom_text;
} data;

static int init(const char *custom_title, const char *custom_text, void (*close_func)(int accepted),
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
    data.checked = 0;
    if (!data.custom_text) {
        data.custom_text = lang_get_string(PROCEED_GROUP, PROCEED_TEXT);
    }
    return 1;
}

static void draw_background(void) {
    window_draw_underlying_window();
}

static void draw_foreground(void) {
    struct nk_context *ctx = ui_context();
    if (nk_begin(ctx, "popup_dialog", nk_recti(screen_dialog_offset_x() + 80, screen_dialog_offset_y() + 80, 480, 180),
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 60, 1);
        ui_font_extra_large_bold();
        nk_label(ctx, (char *) data.custom_title, NK_TEXT_CENTERED);
        ui_font_standard();
        nk_layout_row_dynamic(ctx, 20, 1);
        nk_label(ctx, (char *) data.custom_text, NK_TEXT_CENTERED);
        nk_layout_row_dynamic(ctx, 20, 2);
        ui_font_extra_large_bold();
        nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 3);
        {
            ui_font_large_bold();
            nk_layout_row_push(ctx, 0.1);
            nk_spacer(ctx);
            nk_layout_row_push(ctx, 0.3);
            if (data.has_buttons >= 1) {
                if (nk_button_label(ctx, (char *) lang_get_string(CUSTOM_TRANSLATION, TR_BUTTON_CANCEL))) {
                    button_cancel(0, 0);
                }
            } else {
                nk_spacer(ctx);
            }
            nk_layout_row_push(ctx, 0.2);
            nk_spacer(ctx);
            nk_layout_row_push(ctx, 0.3);
            struct nk_style_button button;
            button = ctx->style.button;
            ctx->style.button.normal.data.color = nk_rgb(156, 39, 176);
            ctx->style.button.hover.data.color = nk_rgb(164, 56, 182);
            ctx->style.button.text_normal = nk_rgb(255, 255, 255);
            ctx->style.button.text_hover = nk_rgb(255, 255, 255);
            ctx->style.button.text_active = nk_rgb(255, 255, 255);
            if (nk_button_label(ctx, (char *) lang_get_string(CUSTOM_TRANSLATION, TR_BUTTON_OK))) {
                button_ok(0, 0);
            }
            ctx->style.button = button;
            nk_layout_row_push(ctx, 0.1);
            nk_layout_row_end(ctx);
        }
    }
    nk_end(ctx);
    ui_font_standard();
}

static void handle_input(const mouse *m, const hotkeys *h) {
    if (input_go_back_requested(m, h)) {
        data.close_func(0);
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
    data.close_func(0);
}

static void confirm(void) {
    window_go_back();
    data.close_func(1);
}

void window_popup_dialog_show_confirmation(const char *custom_title, const char *custom_text,
                                           void (*close_func)(int accepted)) {
    if (init(custom_title, custom_text, close_func, 1)) {
        window_type window = {
                WINDOW_POPUP_DIALOG,
                draw_background,
                draw_foreground,
                handle_input
        };
        window_show(&window);
    }
}

void window_popup_dialog_show_ex(const char *custom_title, const char *custom_text,
                                 void (*close_func)(int accepted), int has_ok_cancel_buttons) {
    if (init(custom_title, custom_text, close_func, has_ok_cancel_buttons)) {
        window_type window = {
                WINDOW_POPUP_DIALOG,
                draw_background,
                draw_foreground,
                handle_input
        };
        window_show(&window);
    }
}