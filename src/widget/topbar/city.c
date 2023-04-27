#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#include "city.h"
#include "window/ui_window.h"
#include "window/nuklear.h"
#include "graphics/screen.h"
#include "window/icons.h"
#include "core/config.h"
#include "game/state.h"
#include "city/finance.h"
#include "translation/translation.h"
#include "city/population.h"
#include "game/time.h"

struct nk_rect top_bar_bounds() {
    return nk_recti(screen_width() / 2 - (450 / 2), 0, 450, 35);
}

void write_city_date(struct nk_context *ctx);

void widget_top_bar_city_draw_foreground(void) {

    struct nk_context *ctx = ui_context();
    if (nk_begin(ctx, "city_top_bar", top_bar_bounds(),
                 NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER)) {
        float row_layout[3];
        row_layout[0] = 0.3f;
        row_layout[1] = 0.40f;
        row_layout[2] = 0.3f;
        nk_layout_row(ctx, NK_DYNAMIC, 25, 3, row_layout);
        if (nk_group_begin(ctx, "column1", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(ctx, 20, 1);
            if (nk_widget_is_hovered(ctx)) {
                nk_tooltip(ctx, gettext("City funds"));
            }
            nk_labelf(ctx, NK_TEXT_CENTERED, "Dn %u", city_finance_treasury());
            nk_group_end(ctx);
        }
        if (nk_group_begin(ctx, "column2", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(ctx, 20, 1);
            if (nk_widget_is_hovered(ctx)) {
                nk_tooltip(ctx, gettext("City population"));
            }
            nk_labelf(ctx, NK_TEXT_CENTERED, "Pop %u", city_population());
            nk_group_end(ctx);
        }
        if (nk_group_begin(ctx, "column3", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(ctx, 20, 1);
            write_city_date(ctx);
            nk_group_end(ctx);
        }
    }
    nk_end(ctx);
}

void write_city_date(struct nk_context *ctx) {
    int month = game_time_month();
    int year = game_time_year();
    if(year >= 0) {
        switch (month) {
            case 0:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Jan %d AD", year);
                break;
            case 1:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Feb %d AD", year);
                break;
            case 2:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Mar %d AD", year);
                break;
            case 3:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Apr %d AD", year);
                break;
            case 4:
                nk_labelf(ctx, NK_TEXT_CENTERED, "May %d AD", year);
                break;
            case 5:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Jun %d AD", year);
                break;
            case 6:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Jul %d AD", year);
                break;
            case 7:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Aug %d AD", year);
                break;
            case 8:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Sep %d AD", year);
                break;
            case 9:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Oct %d AD", year);
                break;
            case 10:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Nov %d AD", year);
                break;
            case 11:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Dec %d AD", year);
                break;
        }
    } else {
        switch (month) {
            case 0:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Jan %d BC", year * -1);
                break;
            case 1:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Feb %d BC", year * -1);
                break;
            case 2:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Mar %d BC", year * -1);
                break;
            case 3:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Apr %d BC", year * -1);
                break;
            case 4:
                nk_labelf(ctx, NK_TEXT_CENTERED, "May %d BC", year * -1);
                break;
            case 5:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Jun %d BC", year * -1);
                break;
            case 6:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Jul %d BC", year * -1);
                break;
            case 7:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Aug %d BC", year * -1);
                break;
            case 8:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Sep %d BC", year * -1);
                break;
            case 9:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Oct %d BC", year * -1);
                break;
            case 10:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Nov %d BC", year * -1);
                break;
            case 11:
                nk_labelf(ctx, NK_TEXT_CENTERED, "Dec %d BC", year * -1);
                break;
        }
    }
}

int widget_top_bar_city_handle_mouse(const mouse *m) {
    if (NK_INBOX(m->x, m->y, top_bar_bounds().x, top_bar_bounds().y, top_bar_bounds().w,
                 top_bar_bounds().h)) {
        return 1;
    }
    return 0;
}
