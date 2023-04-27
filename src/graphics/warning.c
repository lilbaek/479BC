#include "warning.h"

#include "city/warning.h"
#include "game/state.h"
#include "graphics/image.h"
#include "graphics/panel.h"
#include "graphics/screen.h"
#include "graphics/text.h"
#include "graphics/window.h"

static const int TOP_OFFSETS[] = {35, 60, 85, 110, 135};

static int determine_width(const uint8_t *text)
{
    int width = text_get_width(text, FONT_NORMAL_BLACK);
    if (width <= 100) {
        return 200;
    } else if (width <= 200) {
        return 300;
    } else if (width <= 300) {
        return 400;
    } else {
        return 460;
    }
}

void warning_draw(void)
{
    if (!window_is(WINDOW_CITY) && !window_is(WINDOW_EDITOR_MAP)) {
        city_warning_clear_all();
        return;
    }

    for (int i = 0; i < 5; i++) {
        const uint8_t *text = city_warning_get(i);
        if (!text) {
            continue;
        }
        int top_offset = TOP_OFFSETS[i];
        if (game_state_is_paused()) {
            top_offset += 70;
        }
        int box_width = determine_width(text);
        int center = (screen_width() / 2);
        label_draw(center - box_width / 2 + 1, top_offset, box_width / BLOCK_SIZE + 1, 1);
        text_draw_centered(text, center - box_width / 2 + 1, top_offset + 4, box_width, FONT_NORMAL_WHITE, 0);
    }
    city_warning_clear_outdated();
}
