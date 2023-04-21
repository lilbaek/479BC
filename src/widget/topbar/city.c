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

struct nk_rect top_bar_bounds() {
    return nk_recti(0, 0, screen_width(), 30);
}

void widget_top_bar_city_draw_foreground(void) {
    struct nk_context *ctx = ui_context();
    if (nk_begin(ctx, "city_top_bar", top_bar_bounds(),
                 NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 30, 5);
    }
    nk_end(ctx);
}

int widget_top_bar_city_handle_mouse(const mouse *m) {
    if (NK_INBOX(m->x, m->y, top_bar_bounds().x, top_bar_bounds().y, top_bar_bounds().w,
                 top_bar_bounds().h)) {
        return 1;
    }
    return 0;
}
