
#include "Window.h"

#include "game/settings.h"
#include "graphics/arrow_button.h"
#include "graphics/graphics.h"
#include "graphics/lang_text.h"
#include "graphics/panel.h"
#include "graphics/window.h"

static void arrowButtonDifficulty(int param1, int param2);
static void arrowButtonGods(int param1, int param2);

static arrow_button arrowButtons[] = {
	{0, 54, 15, 24, arrowButtonDifficulty, 0, 0},
	{24, 54, 17, 24, arrowButtonDifficulty, 1, 0},
	{0, 102, 21, 24, arrowButtonGods, 2, 0}
};

void UI_DifficultyOptions_drawForeground()
{
    graphics_in_dialog();

	outer_panel_draw(48, 80, 24, 12);
	
	lang_text_draw_centered(153, 0, 48, 94, 384, FONT_LARGE_BLACK);
	
	lang_text_draw_centered(153, setting_difficulty() + 1, 80, 142, 224, FONT_NORMAL_BLACK);
	lang_text_draw_centered(153, setting_gods_enabled() ? 7 : 6, 80, 190, 224, FONT_NORMAL_BLACK);
	arrow_buttons_draw(288, 80, arrowButtons, 3);
	lang_text_draw_centered(153, 8, 48, 246, 384, FONT_NORMAL_BLACK);

    graphics_reset_dialog();
}

void UI_DifficultyOptions_handleMouse(const mouse *m)
{
	if (m->right.went_up) {
		// cancel dialog
		UI_Window_goTo(Window_City);
	} else {
		arrow_buttons_handle_mouse(mouse_in_dialog(m), 288, 80, arrowButtons, 4);
	}
}

static void arrowButtonDifficulty(int param1, int param2)
{
    if (param1) {
        setting_decrease_difficulty();
    } else {
        setting_increase_difficulty();
    }
	window_invalidate();
}

static void arrowButtonGods(int param1, int param2)
{
    setting_toggle_gods_enabled();
	window_invalidate();
}

