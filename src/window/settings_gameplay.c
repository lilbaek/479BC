
#include "settings_gameplay.h"
#include "graphics/window.h"
#include "ui_window.h"
#include "translation/translation.h"
#include "nuklear.h"
#include "graphics/screen.h"
#include "graphics/image.h"
#include "assets/assets.h"
#include "game/settings.h"

static void create_ok_button(struct nk_context *ctx) {
    struct nk_style_button button;
    button = ctx->style.button;
    ctx->style.button.normal.data.color = nk_rgb(156, 39, 176);
    ctx->style.button.hover.data.color = nk_rgb(164, 56, 182);
    ctx->style.button.text_normal = nk_rgb(255, 255, 255);
    ctx->style.button.text_hover = nk_rgb(255, 255, 255);
    ctx->style.button.text_active = nk_rgb(255, 255, 255);
    if (nk_button_label(ctx, gettext("Confirm"))) {

    }
    ctx->style.button = button;
}

static void draw_background(void) {
    image_draw_fullscreen_background(assets_get_image_id("mainmenu", "start-screen"));
}

static void create_row(struct nk_context *ctx) {
    nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 2);
    nk_layout_row_push(ctx, 0.01f);
    nk_spacer(ctx);
    nk_layout_row_push(ctx, 0.99f);
}

static void draw_foreground(void) {
    struct nk_context *ctx = ui_context();
    int w = 677;
    int h = 500;
    ui_font_large_bold();
    if (nk_begin_titled(ctx, "settings_gameplay", gettext("Gameplay"),
                        nk_recti((screen_width() / 2) - w / 2, (screen_height() / 2) - h / 2, w, h),
                        NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE)) {
        ui_font_standard();
        nk_layout_space_begin(ctx, NK_STATIC, h - 40, 3);
        nk_layout_space_push(ctx, nk_rect(0, 0, 330, h - 105));
        if (nk_group_begin(ctx, "group_left", NK_WINDOW_BORDER)) {
            ui_font_large_bold();
            create_row(ctx);
            nk_label(ctx, gettext("Graphics"), NK_TEXT_LEFT);
            ui_font_standard();
            create_row(ctx);
            if(nk_check_label(ctx, gettext("Full screen"), setting_fullscreen())) {
                // TODO
            }
            create_row(ctx);
            if(nk_check_label(ctx, gettext("Full screen"), setting_fullscreen())) {
                // TODO
            }
            nk_group_end(ctx);
        }
        nk_layout_space_push(ctx, nk_rect(335, 0, 328, h - 105));
        if (nk_group_begin(ctx, "group_right", NK_WINDOW_BORDER)) {
            ui_font_large_bold();
            create_row(ctx);
            nk_label(ctx, gettext("Sound"), NK_TEXT_LEFT);
            ui_font_standard();
            nk_group_end(ctx);
        }
        ui_font_large_bold();
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
    ui_font_standard();
}

static void handle_input(const mouse *m, const hotkeys *h) {
    if (h->escape_pressed) {
        window_go_back();
    }
}

void window_settings_gameplay_show() {
    window_type window = {
            WINDOW_SETTINGS_GAMEPLAY_MENU,
            draw_background,
            draw_foreground,
            handle_input
    };
    window_show(&window);
}
