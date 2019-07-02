#include "sidebar_editor.h"

#include "building/menu.h"
#include "city/message.h"
#include "city/view.h"
#include "city/warning.h"
#include "core/direction.h"
#include "core/image_group_editor.h"
#include "game/orientation.h"
#include "game/state.h"
#include "game/undo.h"
#include "graphics/graphics.h"
#include "graphics/image.h"
#include "graphics/image_button.h"
#include "graphics/lang_text.h"
#include "graphics/panel.h"
#include "graphics/screen.h"
#include "graphics/text.h"
#include "graphics/window.h"
#include "input/scroll.h"
#include "map/orientation.h"
#include "scenario/editor.h"
#include "scenario/editor_events.h"
#include "scenario/editor_map.h"
#include "scenario/map.h"
#include "scenario/property.h"
#include "sound/effect.h"
#include "widget/minimap.h"
#include "window/advisors.h"
#include "window/build_menu.h"
#include "window/city.h"
#include "window/empire.h"
#include "window/message_dialog.h"
#include "window/message_list.h"
#include "window/mission_briefing.h"
#include "window/overlay_menu.h"

#include "window/editor/attributes.h"
#include "window/editor/map.h"

#define SIDEBAR_WIDTH 162
#define SIDEBAR_BORDER ((screen_width() + 20) % 60)
#define BOTTOM_BORDER ((screen_height() - 24) % 15)

static void button_build(int submenu, int param2);

static void button_attributes(int show, int param2);

static image_button buttons_build[] = {
    {7, 123, 71, 23, IB_NORMAL, 137, 45, button_attributes, button_none, 0, 0, 1},
    {84, 123, 71, 23, IB_NORMAL, 137, 48, button_attributes, button_none, 1, 0, 1},
    {13, 267, 39, 26, IB_NORMAL, 137, 0, button_build, button_none, 0, 0, 1},
    {63, 267, 39, 26, IB_NORMAL, 137, 3, button_build, button_none, 1, 0, 1},
    {113, 267, 39, 26, IB_NORMAL, 137, 6, button_build, button_none, 2, 0, 1},
    {13, 303, 39, 26, IB_BUILD, 137, 21, button_build, button_none, 7, 0, 1},
    {63, 303, 39, 26, IB_NORMAL, 137, 12, button_build, button_none, 4, 0, 1},
    {113, 303, 39, 26, IB_NORMAL, 137, 15, button_build, button_none, 5, 0, 1},
    {13, 339, 39, 26, IB_NORMAL, 137, 18, button_build, button_none, 6, 0, 1},
    {63, 339, 39, 26, IB_NORMAL, 137, 30, button_build, button_none, 10, 0, 1},
    {113, 339, 39, 26, IB_BUILD, 137, 24, button_build, button_none, 8, 0, 1},
    {13, 375, 39, 26, IB_NORMAL, 137, 9, button_build, button_none, 3, 0, 1},
    {63, 375, 39, 26, IB_BUILD, 137, 39, button_build, button_none, 13, 0, 1},
    {113, 375, 39, 26, IB_BUILD, 137, 42, button_build, button_none, 14, 0, 1},
    {13, 411, 39, 26, IB_BUILD, 137, 33, button_build, button_none, 17, 0, 1},
    {63, 411, 39, 26, IB_BUILD, 137, 27, button_build, button_none, 20, 0, 1},
    {113, 411, 39, 26, IB_BUILD, 137, 51, button_build, button_none, 24, 0, 1},
};

static int get_x_offset(void)
{
    int s_width = screen_width();
    return (s_width - (s_width + 20) % 60 - SIDEBAR_WIDTH);
}

static void draw_minimap(int force)
{
    widget_minimap_draw(get_x_offset() + 8, 30, 73, 111, force);
}

static void draw_buttons(void)
{
    image_buttons_draw(get_x_offset(), 24, buttons_build, 17);
}

static void draw_status(void)
{
    int x_offset = get_x_offset();
    inner_panel_draw(x_offset + 1, 175, 10, 7);
    int text_offset = x_offset + 6;

    // TODO selected tool + brush
    int selected_tool = 0;
    int brush_size = 0;
    lang_text_draw(49, selected_tool, text_offset, 178, FONT_NORMAL_WHITE);
    switch (selected_tool) {
        case 0: // grass
        case 1: // trees
        case 2: // water
        case 4: // scrub
        case 5: // rocks
        case 6: // meadow
        case 11: // raise land
        case 12: // lower land
            lang_text_draw(48, brush_size, text_offset, 194, FONT_NORMAL_GREEN);
            break;
        default:
            break;
    }

    map_point entry = scenario_map_entry();
    map_point exit = scenario_map_exit();
    int people_text;
    font_t people_font = FONT_NORMAL_RED;
    if (entry.x == -1) {
        if (exit.x == -1) {
            people_text = 60;
        } else {
            people_text = 59;
        }
    } else if (exit.x == -1) {
        people_text = 61;
    } else {
        people_text = 62;
        people_font = FONT_NORMAL_GREEN;
    }
    lang_text_draw(44, people_text, text_offset, 224, people_font);

    entry = scenario_map_river_entry();
    exit = scenario_map_river_exit();
    if (entry.x != -1 || entry.x != -1) {
        if (entry.x == -1) {
            lang_text_draw(44, 137, text_offset, 239, FONT_NORMAL_RED);
        } else if (exit.x == -1) {
            lang_text_draw(44, 138, text_offset, 239, FONT_NORMAL_RED);
        } else {
            lang_text_draw(44, 67, text_offset, 239, FONT_NORMAL_GREEN);
        }
    }

    int invasion_points = scenario_editor_count_invasion_points();
    if (invasion_points == 1) {
        lang_text_draw(44, 64, text_offset, 254, FONT_NORMAL_GREEN);
    } else if (invasion_points > 1) {
        int width = text_draw_number(invasion_points, '@', " ", text_offset - 2, 254, FONT_NORMAL_GREEN);
        lang_text_draw(44, 65, text_offset + width - 8, 254, FONT_NORMAL_GREEN);
    } else {
        editor_invasion first_invasion;
        scenario_editor_invasion_get(0, &first_invasion);
        if (first_invasion.type) {
            lang_text_draw(44, 63, text_offset, 254, FONT_NORMAL_RED);
        }
    }

    if (scenario_editor_earthquake_severity() > 0) {
        map_point earthquake = scenario_editor_earthquake_point();
        if (earthquake.x == -1 || earthquake.y == -1) {
            lang_text_draw(44, 57, text_offset, 269, FONT_NORMAL_RED);
        } else {
            lang_text_draw(44, 58, text_offset, 269, FONT_NORMAL_GREEN);
        }
    }
}

static void draw_sidebar(void)
{
    int image_base = image_group(GROUP_EDITOR_SIDE_PANEL);
    int x_offset = get_x_offset();
    image_draw(image_base, x_offset, 24);
    draw_buttons();
    draw_minimap(1);
    draw_status();

    // relief images below panel
    int y_offset = 474;
    int y_max = screen_height() - BOTTOM_BORDER;
    while (y_offset < y_max) {
        if (y_max - y_offset <= 120) {
            image_draw(image_base + 1, x_offset, y_offset);
            y_offset += 120;
        } else {
            image_draw(image_base + 2, x_offset, y_offset);
            y_offset += 285;
        }
    }
}

static void draw_filler_borders(void)
{
    int border_right_width = SIDEBAR_BORDER;
    if (border_right_width) {
        int image_id = image_group(GROUP_TOP_MENU_SIDEBAR) + 13;
        if (border_right_width > 24) {
            // larger border
            image_id -= 1;
        }
        if (border_right_width > 40) {
            int x_offset = screen_width() - 35;
            int y_max = screen_height();
            for (int y_offset = 24; y_offset < y_max; y_offset += 24) {
                image_draw(image_id, x_offset, y_offset);
            }
        }
        int x_offset = screen_width() - border_right_width;
        int y_max = screen_height();
        for (int y_offset = 24; y_offset < y_max; y_offset += 24) {
            image_draw(image_id, x_offset, y_offset);
        }
    }

    int border_bottom_height = BOTTOM_BORDER;
    graphics_fill_rect(0, screen_height() - border_bottom_height, screen_width(), border_bottom_height, COLOR_BLACK);
}

void widget_sidebar_editor_draw_background(void)
{
    draw_sidebar();
    draw_filler_borders();
}

void widget_sidebar_editor_draw_foreground(void)
{
    draw_buttons();
    draw_minimap(0);
}

int widget_sidebar_editor_handle_mouse(const mouse *m)
{
    if (widget_minimap_handle_mouse(m)) {
        return 1;
    }
    return image_buttons_handle_mouse(m, get_x_offset(), 24, buttons_build, 17, 0);
}

void widget_sidebar_editor_handle_mouse_build_menu(const mouse *m)
{
    image_buttons_handle_mouse(m, get_x_offset(), 24, buttons_build, 17, 0);
}

void widget_sidebar_editor_handle_mouse_attributes(const mouse *m)
{
    image_buttons_handle_mouse(m, get_x_offset(), 24, buttons_build, 2, 0);
}

static void button_attributes(int show, int param2)
{
    if (show) {
        if (!window_is(WINDOW_EDITOR_ATTRIBUTES)) {
            window_editor_attributes_show();
        }
    } else {
        if (!window_is(WINDOW_EDITOR_MAP)) {
            window_editor_map_show();
        }
    }
}

static void button_build(int submenu, int param2)
{
    // TODO implement
}
