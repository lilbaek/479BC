#include "sidebar.h"

#include "building/menu.h"
#include "city/message.h"
#include "city/view.h"
#include "city/warning.h"
#include "core/direction.h"
#include "game/orientation.h"
#include "game/settings.h"
#include "game/state.h"
#include "game/undo.h"
#include "graphics/arrow_button.h"
#include "graphics/graphics.h"
#include "graphics/image.h"
#include "graphics/image_button.h"
#include "graphics/lang_text.h"
#include "graphics/panel.h"
#include "graphics/screen.h"
#include "graphics/text.h"
#include "graphics/window.h"
#include "map/orientation.h"
#include "scenario/property.h"
#include "sound/effect.h"
#include "widget/city.h"
#include "widget/minimap.h"
#include "window/advisors.h"
#include "window/build_menu.h"
#include "window/city.h"
#include "window/empire.h"
#include "window/message_dialog.h"
#include "window/message_list.h"
#include "window/mission_briefing.h"
#include "window/overlay_menu.h"

#define SIDEBAR_COLLAPSED_WIDTH 42
#define SIDEBAR_EXPANDED_WIDTH 162
#define SIDEBAR_BORDER ((screen_width() + 20) % 60)
#define BOTTOM_BORDER ((screen_height() - 24) % 15)
#define FILLER_Y_OFFSET 474

// sliding sidebar progress to x offset translation
static const int PROGRESS_TO_X_OFFSET[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 16, 18, 21, 24, 27,
    30, 33, 37, 41, 45, 49, 54, 59, 64, 70, 76, 83, 91, 99, 106, 113,
    119, 125, 130, 135, 139, 143, 146, 149, 152, 154, 156, 158, 160, 162, 165
};

static void slide_sidebar(void);

static void button_overlay(int param1, int param2);
static void button_collapse_expand(int param1, int param2);
static void button_build(int submenu, int param2);
static void button_undo(int param1, int param2);
static void button_messages(int param1, int param2);
static void button_help(int param1, int param2);
static void button_go_to_problem(int param1, int param2);
static void button_advisors(int param1, int param2);
static void button_empire(int param1, int param2);
static void button_mission_briefing(int param1, int param2);
static void button_rotate_north(int param1, int param2);
static void button_rotate(int clockwise, int param2);
static void button_game_speed(int is_down, int param2);

static image_button buttons_overlays_collapse_sidebar[] = {
    {127, 5, 31, 20, IB_NORMAL, 90, 0, button_collapse_expand, button_none, 0, 0, 1},
    {4, 3, 117, 31, IB_NORMAL, 93, 0, button_overlay, button_help, 0, 0, 1}
};

static image_button button_expand_sidebar[] = {
    {6, 4, 31, 20, IB_NORMAL, 90, 4, button_collapse_expand, button_none, 0, 0, 1}
};

static image_button buttons_build_collapsed[] = {
    {2, 32, 39, 26, IB_NORMAL, GROUP_SIDEBAR_BUTTONS, 0, button_build, button_none, 0, 0, 1},
    {2, 67, 39, 26, IB_NORMAL, GROUP_SIDEBAR_BUTTONS, 8, button_build, button_none, 1, 0, 1},
    {2, 102, 39, 26, IB_NORMAL, GROUP_SIDEBAR_BUTTONS, 12, button_build, button_none, 2, 0, 1},
    {2, 137, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 4, button_build, button_none, 3, 0, 1},
    {2, 172, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 40, button_build, button_none, 4, 0, 1},
    {2, 207, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 28, button_build, button_none, 5, 0, 1},
    {2, 242, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 24, button_build, button_none, 6, 0, 1},
    {2, 277, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 20, button_build, button_none, 7, 0, 1},
    {2, 312, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 16, button_build, button_none, 8, 0, 1},
    {2, 347, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 44, button_build, button_none, 9, 0, 1},
    {2, 382, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 36, button_build, button_none, 10, 0, 1},
    {2, 417, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 32, button_build, button_none, 11, 0, 1},
};

static image_button buttons_build_expanded[] = {
    {13, 277, 39, 26, IB_NORMAL, GROUP_SIDEBAR_BUTTONS, 0, button_build, button_none, 0, 0, 1},
    {63, 277, 39, 26, IB_NORMAL, GROUP_SIDEBAR_BUTTONS, 8, button_build, button_none, 1, 0, 1},
    {113, 277, 39, 26, IB_NORMAL, GROUP_SIDEBAR_BUTTONS, 12, button_build, button_none, 2, 0, 1},
    {13, 313, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 4, button_build, button_none, 3, 0, 1},
    {63, 313, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 40, button_build, button_none, 4, 0, 1},
    {113, 313, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 28, button_build, button_none, 5, 0, 1},
    {13, 349, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 24, button_build, button_none, 6, 0, 1},
    {63, 349, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 20, button_build, button_none, 7, 0, 1},
    {113, 349, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 16, button_build, button_none, 8, 0, 1},
    {13, 385, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 44, button_build, button_none, 9, 0, 1},
    {63, 385, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 36, button_build, button_none, 10, 0, 1},
    {113, 385, 39, 26, IB_BUILD, GROUP_SIDEBAR_BUTTONS, 32, button_build, button_none, 11, 0, 1},
    {13, 421, 39, 26, IB_NORMAL, GROUP_SIDEBAR_BUTTONS, 48, button_undo, button_none, 0, 0, 1},
    {63, 421, 39, 26, IB_NORMAL, GROUP_MESSAGE_ICON, 18, button_messages, button_help, 0, 0, 1},
    {113, 421, 39, 26, IB_BUILD, GROUP_MESSAGE_ICON, 22, button_go_to_problem, button_none, 0, 0, 1},
};

static image_button buttons_top_expanded[] = {
    {7, 155, 71, 23, IB_NORMAL, GROUP_SIDEBAR_ADVISORS_EMPIRE, 0, button_advisors, button_none, 0, 0, 1},
    {84, 155, 71, 23, IB_NORMAL, GROUP_SIDEBAR_ADVISORS_EMPIRE, 3, button_empire, button_help, 0, 0, 1},
    {7, 184, 33, 22, IB_NORMAL, GROUP_SIDEBAR_BRIEFING_ROTATE_BUTTONS, 0, button_mission_briefing, button_none, 0, 0, 1},
    {46, 184, 33, 22, IB_NORMAL, GROUP_SIDEBAR_BRIEFING_ROTATE_BUTTONS, 3, button_rotate_north, button_none, 0, 0, 1},
    {84, 184, 33, 22, IB_NORMAL, GROUP_SIDEBAR_BRIEFING_ROTATE_BUTTONS, 6, button_rotate, button_none, 0, 0, 1},
    {123, 184, 33, 22, IB_NORMAL, GROUP_SIDEBAR_BRIEFING_ROTATE_BUTTONS, 9, button_rotate, button_none, 1, 0, 1},
};

static arrow_button arrow_buttons_speed[] = {
    {11, 30, 17, 24, button_game_speed, 1, 0},
    {35, 30, 15, 24, button_game_speed, 0, 0},
};

static struct {
    time_millis slide_start;
    int progress;
    int focus_button_for_tooltip;
} data;

static int get_x_offset_expanded(void)
{
    int s_width = screen_width();
    return (s_width - (s_width + 20) % 60 - SIDEBAR_EXPANDED_WIDTH);
}

static int get_x_offset_collapsed(void)
{
    int s_width = screen_width();
    return s_width - (s_width + 20) % 60 - SIDEBAR_COLLAPSED_WIDTH;
}

static void draw_minimap(int force)
{
    if (!city_view_is_sidebar_collapsed()) {
        int x_offset = get_x_offset_expanded();
        widget_minimap_draw(x_offset + 8, 59, 73, 111, force);
    }
}

static void draw_buttons(void)
{
    buttons_build_expanded[12].enabled = game_can_undo();
    if (city_view_is_sidebar_collapsed()) {
        int x_offset = get_x_offset_collapsed();
        image_buttons_draw(x_offset, 24, button_expand_sidebar, 1);
        image_buttons_draw(x_offset, 24, buttons_build_collapsed, 12);
    } else {
        int x_offset = get_x_offset_expanded();
        image_buttons_draw(x_offset, 24, buttons_overlays_collapse_sidebar, 2);
        image_buttons_draw(x_offset, 24, buttons_build_expanded, 15);
        image_buttons_draw(x_offset, 24, buttons_top_expanded, 6);
    }
}

static void draw_build_image(int x_offset, int is_sliding)
{
    if (city_view_is_sidebar_collapsed() && !is_sliding) {
        return;
    }
    image_draw(window_build_menu_image(), x_offset, 239);
}

static void draw_overlay_text(int x_offset)
{
    if (!city_view_is_sidebar_collapsed()) {
        if (game_state_overlay()) {
            lang_text_draw_centered(14, game_state_overlay(), x_offset, 32, 117, FONT_NORMAL_GREEN);
        } else {
            lang_text_draw_centered(6, 4, x_offset, 32, 117, FONT_NORMAL_GREEN);
        }
    }
}

static void draw_sidebar_filler(int x_offset, int y_offset, int is_collapsed)
{
    // relief images below panel
    int image_base = image_group(GROUP_SIDE_PANEL);
    int y_max = screen_height() - BOTTOM_BORDER;
    while (y_offset < y_max) {
        if (y_max - y_offset <= 120) {
            image_draw(image_base + 2 + is_collapsed, x_offset, y_offset);
            y_offset += 120;
        } else {
            image_draw(image_base + 4 + is_collapsed, x_offset, y_offset);
            y_offset += 285;
        }
    }
}

static void draw_sidebar_extra_info_panel(int x_offset, int is_collapsed)
{
    int y_offset = FILLER_Y_OFFSET;

    if (!is_collapsed) {
        // extra information, if it fits
        if (screen_height() - y_offset >= 64) {
            y_offset += 64;
        }
    }

    draw_sidebar_filler(x_offset, y_offset, is_collapsed);
}

static void draw_sidebar_extra_info_buttons(int x_offset, int is_collapsed)
{
    if (is_collapsed) {
        return;
    }
    int s_height = screen_height();
    int border_right_width = SIDEBAR_BORDER;
    graphics_set_clip_rectangle(x_offset, 24, screen_width() - x_offset - border_right_width, s_height - 24 - BOTTOM_BORDER);
    int y_offset = FILLER_Y_OFFSET;

    // extra information, if it fits
    if (s_height - y_offset >= 64) {
        int panel_blocks = 4;
        graphics_draw_vertical_line(x_offset, y_offset, y_offset + 64, COLOR_WHITE);
        graphics_draw_vertical_line(x_offset + SIDEBAR_EXPANDED_WIDTH - 1, y_offset, y_offset + 64, COLOR_SIDEBAR);
        inner_panel_draw(x_offset + 1, y_offset, SIDEBAR_EXPANDED_WIDTH / 16, panel_blocks);

        lang_text_draw(45, 2, x_offset + 11, y_offset + 10, FONT_NORMAL_WHITE);
        text_draw_percentage(setting_game_speed(), x_offset + 60, y_offset + 36, FONT_NORMAL_GREEN);
        arrow_buttons_draw(x_offset, y_offset, arrow_buttons_speed, 2);
    }

    graphics_reset_clip_rectangle();
}

static void draw_sidebar(void)
{
    int image_base = image_group(GROUP_SIDE_PANEL);
    int is_collapsed = city_view_is_sidebar_collapsed();
    int x_offset;
    if (is_collapsed) {
        x_offset = get_x_offset_collapsed();
        image_draw(image_base, x_offset, 24);
    } else {
        x_offset = get_x_offset_expanded();
        image_draw(image_base + 1, x_offset, 24);
    }
    draw_buttons();
    draw_overlay_text(x_offset + 4);
    draw_build_image(x_offset + 6, 0);
    draw_minimap(1);

    draw_sidebar_extra_info_panel(x_offset, is_collapsed);
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

void widget_sidebar_draw_background(void)
{
    draw_sidebar();
    draw_filler_borders();
}

static void enable_building_buttons(void)
{
    for (int i = 0; i < 12; i++) {
        buttons_build_expanded[i].enabled = 1;
        if (building_menu_count_items(buttons_build_expanded[i].parameter1) <= 0) {
            buttons_build_expanded[i].enabled = 0;
        }

        buttons_build_collapsed[i].enabled = 1;
        if (building_menu_count_items(buttons_build_collapsed[i].parameter1) <= 0) {
            buttons_build_collapsed[i].enabled = 0;
        }
    }
}

static void draw_number_of_messages(void)
{
    if (window_is(WINDOW_CITY) && !city_view_is_sidebar_collapsed()) {
        int messages = city_message_count();
        buttons_build_expanded[12].enabled = game_can_undo();
        buttons_build_expanded[13].enabled = messages > 0;
        buttons_build_expanded[14].enabled = city_message_problem_area_count();
        if (messages) {
            int x_offset = get_x_offset_expanded();
            text_draw_number_centered_colored(messages, x_offset + 74, 452, 32, FONT_SMALL_PLAIN, COLOR_BLACK);
            text_draw_number_centered_colored(messages, x_offset + 73, 453, 32, FONT_SMALL_PLAIN, COLOR_WHITE);
        }
    }
}

void widget_sidebar_draw_foreground(void)
{
    if (building_menu_has_changed()) {
        enable_building_buttons();
    }
    int x_offset;
    int is_collapsed = city_view_is_sidebar_collapsed();
    if (is_collapsed) {
        x_offset = get_x_offset_collapsed();
    } else {
        x_offset = get_x_offset_expanded();
    }
    draw_buttons();
    draw_overlay_text(x_offset + 4);
    draw_minimap(0);
    draw_number_of_messages();

    draw_sidebar_extra_info_buttons(x_offset, is_collapsed);
}

void widget_sidebar_draw_foreground_military(void)
{
    draw_minimap(0);
}

int widget_sidebar_handle_mouse(const mouse *m)
{
    if (widget_city_has_input()) {
        return 0;
    }
    int click = 0;
    int button_id;
    data.focus_button_for_tooltip = 0;
    if (city_view_is_sidebar_collapsed()) {
        int x_offset = get_x_offset_collapsed();
        click |= image_buttons_handle_mouse(m, x_offset, 24, button_expand_sidebar, 1, &button_id);
        if (button_id) {
            data.focus_button_for_tooltip = 12;
        }
        click |= image_buttons_handle_mouse(m, x_offset, 24, buttons_build_collapsed, 12, &button_id);
        if (button_id) {
            data.focus_button_for_tooltip = button_id + 19;
        }
    } else {
        if (widget_minimap_handle_mouse(m)) {
            return 1;
        }
        int x_offset = get_x_offset_expanded();
        click |= image_buttons_handle_mouse(m, x_offset, 24, buttons_overlays_collapse_sidebar, 2, &button_id);
        if (button_id) {
            data.focus_button_for_tooltip = button_id + 9;
        }
        click |= image_buttons_handle_mouse(m, x_offset, 24, buttons_build_expanded, 15, &button_id);
        if (button_id) {
            data.focus_button_for_tooltip = button_id + 19;
        }
        click |= image_buttons_handle_mouse(m, x_offset, 24, buttons_top_expanded, 6, &button_id);
        if (button_id) {
            data.focus_button_for_tooltip = button_id + 39;
        }
        // TODO only handle if we draw them
        click |= arrow_buttons_handle_mouse(m, x_offset, FILLER_Y_OFFSET, arrow_buttons_speed, 2);
    }
    return click;
}

void widget_sidebar_handle_mouse_build_menu(const mouse *m)
{
    if (city_view_is_sidebar_collapsed()) {
        image_buttons_handle_mouse(m, get_x_offset_collapsed(), 24, buttons_build_collapsed, 12, 0);
    } else {
        image_buttons_handle_mouse(m, get_x_offset_expanded(), 24, buttons_build_expanded, 15, 0);
    }
}

int widget_sidebar_get_tooltip_text(void)
{
    return data.focus_button_for_tooltip;
}

static void button_overlay(int param1, int param2)
{
    window_overlay_menu_show();
}

static void button_collapse_expand(int param1, int param2)
{
    slide_sidebar();
}

static void button_build(int submenu, int param2)
{
    window_build_menu_show(submenu);
}

static void button_undo(int param1, int param2)
{
    game_undo_perform();
    window_invalidate();
}

static void button_messages(int param1, int param2)
{
    window_message_list_show();
}

static void button_help(int param1, int param2)
{
    window_message_dialog_show(param2, window_city_draw_all);
}

static void button_go_to_problem(int param1, int param2)
{
    int grid_offset = city_message_next_problem_area_grid_offset();
    if (grid_offset) {
        city_view_go_to_grid_offset(grid_offset);
        window_city_show();
    } else {
        window_invalidate();
    }
}

static void button_advisors(int param1, int param2)
{
    window_advisors_show_checked();
}

static void button_empire(int param1, int param2)
{
    window_empire_show_checked();
}

static void button_mission_briefing(int param1, int param2)
{
    if (!scenario_is_custom()) {
        window_mission_briefing_show_review();
    }
}

static void button_rotate_north(int param1, int param2)
{
    game_orientation_rotate_north();
    window_invalidate();
}

static void button_rotate(int clockwise, int param2)
{
    if (clockwise) {
        game_orientation_rotate_right();
    } else {
        game_orientation_rotate_left();
    }
    window_invalidate();
}

static void button_game_speed(int is_down, int param2)
{
    if (is_down) {
        setting_decrease_game_speed();
    } else {
        setting_increase_game_speed();
    }
}

static void update_progress(void)
{
    time_millis now = time_get_millis();
    time_millis diff = now - data.slide_start;
    data.progress = diff / 10;
}

static void draw_sliding_foreground(void)
{
    window_request_refresh();
    update_progress();
    if (data.progress >= 47) {
        city_view_toggle_sidebar();
        window_city_show();
        window_draw(1);
        return;
    }

    int x_offset_expanded = get_x_offset_expanded();
    int x_offset_collapsed = get_x_offset_collapsed();
    graphics_set_clip_rectangle(x_offset_expanded, 24, SIDEBAR_EXPANDED_WIDTH, screen_height() - 24 - BOTTOM_BORDER);

    int image_base = image_group(GROUP_SIDE_PANEL);
    // draw collapsed sidebar
    image_draw(image_base, x_offset_collapsed, 24);
    image_buttons_draw(x_offset_collapsed, 24, button_expand_sidebar, 1);
    image_buttons_draw(x_offset_collapsed, 24, buttons_build_collapsed, 12);

    // draw expanded sidebar on top of it
    if (city_view_is_sidebar_collapsed()) {
        x_offset_expanded += PROGRESS_TO_X_OFFSET[47 - data.progress];
    } else {
        x_offset_expanded += PROGRESS_TO_X_OFFSET[data.progress];
    }
    image_draw(image_base + 1, x_offset_expanded, 24);
    image_buttons_draw(x_offset_expanded, 24, buttons_overlays_collapse_sidebar, 2);
    image_buttons_draw(x_offset_expanded, 24, buttons_build_expanded, 15);
    image_buttons_draw(x_offset_expanded, 24, buttons_top_expanded, 6);

    // black out minimap
    graphics_fill_rect(x_offset_expanded + 8, 59, 145, 111, COLOR_BLACK);

    draw_overlay_text(x_offset_expanded + 4);
    draw_build_image(x_offset_expanded + 6, 1);

    draw_sidebar_filler(x_offset_collapsed, FILLER_Y_OFFSET, 1);
    draw_sidebar_extra_info_panel(x_offset_expanded, 0);
    draw_sidebar_extra_info_buttons(x_offset_expanded, 0);

    graphics_reset_clip_rectangle();
}

static void slide_sidebar(void)
{
    data.progress = 0;
    data.slide_start = time_get_millis();
    city_view_start_sidebar_toggle();
    sound_effect_play(SOUND_EFFECT_SIDEBAR);
    
    window_type window = {
        WINDOW_SLIDING_SIDEBAR,
        window_city_draw,
        draw_sliding_foreground,
        0,
        0
    };
    window_show(&window);
}
