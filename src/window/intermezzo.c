#include "intermezzo.h"

#include "core/time.h"
#include "graphics/graphics.h"
#include "graphics/image.h"
#include "graphics/screen.h"
#include "graphics/window.h"
#include "scenario/property.h"
#include "sound/music.h"
#include "sound/speech.h"
#include "assets/assets.h"

#define DISPLAY_TIME_MILLIS 1000

static struct {
    intermezzo_type type;
    void (*callback)(void);
    time_millis start_time;
} data;

static void init(intermezzo_type type, void (*callback)(void))
{
    data.type = type;
    data.callback = callback;
    data.start_time = time_get_millis();
}

static void draw_background(void)
{
    graphics_clear_screen();

    int mission = scenario_is_custom() ? 0 : scenario_campaign_mission();
    if (data.type == INTERMEZZO_MISSION_BRIEFING) {
        image_draw_fullscreen_background(assets_get_image_id("screens", "mission"));
    } else if (data.type == INTERMEZZO_WON) {
        int image = image_group(GROUP_INTERMEZZO_BACKGROUND) + 2 * mission;
        image += 2;
        image_draw_fullscreen_background(image);
    }

}

static void handle_input(const mouse *m, const hotkeys *h)
{
    time_millis current_time = time_get_millis();
    if (m->right.went_up || (m->is_touch && m->left.double_click)
        || current_time - data.start_time > DISPLAY_TIME_MILLIS) {
        data.callback();
    }
}

void window_intermezzo_show(intermezzo_type type, void (*callback)(void))
{
    window_type window = {
        WINDOW_INTERMEZZO,
        draw_background,
        0,
        handle_input
    };
    init(type, callback);
    window_show(&window);
}
