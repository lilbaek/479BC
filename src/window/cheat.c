#include "cheat.h"
#include "game/cheats.h"
#include "window/nuklear.h"
#include "window/ui_window.h"
#include "game/state.h"


void window_cheat_render() {
    if (!game_cheat_is_cheating() || game_state_is_paused()) {
        return;
    }
    struct nk_context *ctx = ui_context();
    if (nk_begin(ctx, "Cheats", nk_rect(10, 10, 300, 180),
                 NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_BORDER)) {
        nk_layout_row_dynamic(ctx, 30, 2);
        if (nk_button_label(ctx, "Add 5000 DB")) {
            game_cheat_add_money(5000);
        }
        if (nk_button_label(ctx, "Increment year")) {
            game_cheat_advance_year();
        }
        nk_layout_row_dynamic(ctx, 30, 2);
        if (nk_button_label(ctx, "Enable tooltips")) {
            game_cheat_show_tooltip(1);
        }
        if (nk_button_label(ctx, "Disable tooltips")) {
            game_cheat_show_tooltip(0);
        }
        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_button_label(ctx, "Add to granary")) {
            game_cheat_add_granary();
        }
    }
    nk_end(ctx);
}