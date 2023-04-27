#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#include "warning.h"
#include "city/warning.h"
#include "graphics/screen.h"
#include "graphics/window.h"
#include "window/ui_window.h"
#include "window/nuklear.h"

struct nk_rect message_bar_bounds(int messages) {
    return nk_recti(screen_width() / 2 - (450 / 2), 40, 450, 10 + (messages * 25));
}

void warning_draw(void)
{
    if (!window_is(WINDOW_CITY) && !window_is(WINDOW_EDITOR_MAP)) {
        city_warning_clear_all();
        return;
    }
    int number = 0;
    for (int i = 0; i < 5; i++) {
        const char *text = city_warning_get(i);
        if (!text) {
            continue;
        }
        number++;
    }
    if(number > 0) {
        struct nk_context *ctx = ui_context();
        if (nk_begin(ctx, "message_warnings", message_bar_bounds(number),
                     NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER)) {
            for (int i = 0; i < 5; i++) {
                const char *text = city_warning_get(i);
                if (!text) {
                    continue;
                }
                nk_layout_row_dynamic(ctx, 25, 1);
                nk_label(ctx, text, NK_TEXT_CENTERED);
            }
        }
        nk_end(ctx);
    }
    city_warning_clear_outdated();
}
