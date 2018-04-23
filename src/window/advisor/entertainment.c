#include "entertainment.h"

#include "building/count.h"
#include "city/culture.h"
#include "city/festival.h"
#include "city/gods.h"
#include "graphics/generic_button.h"
#include "graphics/image.h"
#include "graphics/lang_text.h"
#include "graphics/panel.h"
#include "graphics/text.h"
#include "graphics/window.h"
#include "window/hold_festival.h"

#include "Data/CityInfo.h"

#define ADVISOR_HEIGHT 23

static void button_hold_festival(int param1, int param2);

static generic_button hold_festival_button[] = {
    {102, 280, 402, 300, GB_IMMEDIATE, button_hold_festival, button_none, 0, 0},
};

static int focus_button_id;

static int get_entertainment_advice()
{
    if (Data_CityInfo.housesRequiringEntertainmentToEvolve > Data_CityInfo.housesRequiringMoreEntertainmentToEvolve) {
        return 3;
    } else if (!Data_CityInfo.housesRequiringMoreEntertainmentToEvolve) {
        return Data_CityInfo.citywideAverageEntertainment ? 1 : 0;
    } else if (Data_CityInfo.entertainmentNeedingShowsMost) {
        return 3 + Data_CityInfo.entertainmentNeedingShowsMost;
    } else {
        return 2;
    }
}

static int get_festival_advice()
{
    int months_since_festival = Data_CityInfo.monthsSinceFestival;
    if (months_since_festival <= 1) {
        return 0;
    } else if (months_since_festival <= 6) {
        return 1;
    } else if (months_since_festival <= 12) {
        return 2;
    } else if (months_since_festival <= 18) {
        return 3;
    } else if (months_since_festival <= 24) {
        return 4;
    } else if (months_since_festival <= 30) {
        return 5;
    } else {
        return 6;
    }
}

static void draw_festival_info()
{
    inner_panel_draw(48, 252, 34, 6);
    image_draw(image_group(GROUP_PANEL_WINDOWS) + 15, 460, 255);
    lang_text_draw(58, 17, 52, 224, FONT_LARGE_BLACK);

    int width = lang_text_draw_amount(8, 4, Data_CityInfo.monthsSinceFestival, 112, 260, FONT_NORMAL_WHITE);
    lang_text_draw(58, 15, 112 + width, 260, FONT_NORMAL_WHITE);
    if (city_festival_is_planned()) {
        lang_text_draw_centered(58, 34, 102, 284, 300, FONT_NORMAL_WHITE);
    } else {
        lang_text_draw_centered(58, 16, 102, 284, 300, FONT_NORMAL_WHITE);
    }
    lang_text_draw_multiline(58, 18 + get_festival_advice(), 56, 305, 380, FONT_NORMAL_WHITE);
}

static int draw_background()
{
    city_gods_calculate_moods(0);
    city_culture_calculate_entertainment();

    outer_panel_draw(0, 0, 40, ADVISOR_HEIGHT);
    image_draw(image_group(GROUP_ADVISOR_ICONS) + 8, 10, 10);

    lang_text_draw(58, 0, 60, 12, FONT_LARGE_BLACK);

    lang_text_draw(58, 1, 180, 46, FONT_SMALL_PLAIN);
    lang_text_draw(58, 2, 260, 46, FONT_SMALL_PLAIN);
    lang_text_draw(58, 3, 340, 46, FONT_SMALL_PLAIN);
    lang_text_draw(58, 4, 480, 46, FONT_SMALL_PLAIN);

    inner_panel_draw(32, 60, 36, 5);

    // theaters
    lang_text_draw_amount(8, 34, building_count_total(BUILDING_THEATER), 40, 64, FONT_NORMAL_WHITE);
    text_draw_number_centered(building_count_active(BUILDING_THEATER), 150, 64, 100, FONT_NORMAL_WHITE);
    text_draw_number_centered(Data_CityInfo.entertainmentTheaterShows, 230, 64, 100, FONT_NORMAL_WHITE);
    int width = text_draw_number(500 * building_count_active(BUILDING_THEATER), '@', " ", 340, 64, FONT_NORMAL_WHITE);
    lang_text_draw(58, 5, 340 + width, 64, FONT_NORMAL_WHITE);
    int pct_theater = city_culture_coverage_theater();
    if (pct_theater == 0) {
        lang_text_draw_centered(57, 10, 470, 64, 100, FONT_NORMAL_WHITE);
    } else if (pct_theater < 100) {
        lang_text_draw_centered(57, 11 + pct_theater / 10, 470, 64, 100, FONT_NORMAL_WHITE);
    } else {
        lang_text_draw_centered(57, 21, 470, 64, 100, FONT_NORMAL_WHITE);
    }

    // amphitheaters
    lang_text_draw_amount(8, 36, building_count_total(BUILDING_AMPHITHEATER), 40, 84, FONT_NORMAL_WHITE);
    text_draw_number_centered(building_count_active(BUILDING_AMPHITHEATER), 150, 84, 100, FONT_NORMAL_WHITE);
    text_draw_number_centered(Data_CityInfo.entertainmentAmphitheaterShows, 230, 84, 100, FONT_NORMAL_WHITE);
    width = text_draw_number(800 * building_count_active(BUILDING_AMPHITHEATER), '@', " ", 340, 84, FONT_NORMAL_WHITE);
    lang_text_draw(58, 5, 340 + width, 84, FONT_NORMAL_WHITE);
    int pct_amphitheater = city_culture_coverage_amphitheater();
    if (pct_amphitheater == 0) {
        lang_text_draw_centered(57, 10, 470, 84, 100, FONT_NORMAL_WHITE);
    } else if (pct_amphitheater < 100) {
        lang_text_draw_centered(57, 11 + pct_amphitheater / 10, 470, 84, 100, FONT_NORMAL_WHITE);
    } else {
        lang_text_draw_centered(57, 21, 470, 84, 100, FONT_NORMAL_WHITE);
    }

    // colosseums
    lang_text_draw_amount(8, 38, building_count_total(BUILDING_COLOSSEUM), 40, 104, FONT_NORMAL_WHITE);
    text_draw_number_centered(building_count_active(BUILDING_COLOSSEUM), 150, 104, 100, FONT_NORMAL_WHITE);
    text_draw_number_centered(Data_CityInfo.entertainmentColosseumShows, 230, 104, 100, FONT_NORMAL_WHITE);
    width = text_draw_number(1500 * building_count_active(BUILDING_COLOSSEUM), '@', " ", 340, 104, FONT_NORMAL_WHITE);
    lang_text_draw(58, 5, 340 + width, 104, FONT_NORMAL_WHITE);
    int pct_colosseum = city_culture_coverage_colosseum();
    if (pct_colosseum == 0) {
        lang_text_draw_centered(57, 10, 470, 104, 100, FONT_NORMAL_WHITE);
    } else if (pct_colosseum < 100) {
        lang_text_draw_centered(57, 11 + pct_colosseum / 10, 470, 104, 100, FONT_NORMAL_WHITE);
    } else {
        lang_text_draw_centered(57, 21, 470, 104, 100, FONT_NORMAL_WHITE);
    }

    // hippodromes
    lang_text_draw_amount(8, 40, building_count_total(BUILDING_HIPPODROME), 40, 123, FONT_NORMAL_WHITE);
    text_draw_number_centered(building_count_active(BUILDING_HIPPODROME), 150, 123, 100, FONT_NORMAL_WHITE);
    text_draw_number_centered(Data_CityInfo.entertainmentHippodromeShows, 230, 123, 100, FONT_NORMAL_WHITE);
    lang_text_draw(58, 6, 360, 123, FONT_NORMAL_WHITE);
    if (city_culture_coverage_hippodrome() == 0) {
        lang_text_draw_centered(57, 10, 470, 123, 100, FONT_NORMAL_WHITE);
    } else {
        lang_text_draw_centered(57, 21, 470, 123, 100, FONT_NORMAL_WHITE);
    }

    lang_text_draw_multiline(58, 7 + get_entertainment_advice(), 60, 148, 512, FONT_NORMAL_BLACK);

    draw_festival_info();

    return ADVISOR_HEIGHT;
}

static void draw_foreground()
{
    if (!city_festival_is_planned()) {
        button_border_draw(102, 280, 300, 20, focus_button_id == 1);
    }
}

static void handle_mouse(const mouse *m)
{
    generic_buttons_handle_mouse(m, 0, 0, hold_festival_button, 1, &focus_button_id);
}

static void button_hold_festival(int param1, int param2)
{
    if (!city_festival_is_planned()) {
        window_hold_festival_show();
    }
}

static int get_tooltip_text()
{
    if (focus_button_id) {
        return 112;
    } else {
        return 0;
    }
}

const advisor_window_type *window_advisor_entertainment()
{
    static const advisor_window_type window = {
        draw_background,
        draw_foreground,
        handle_mouse,
        get_tooltip_text
    };
    focus_button_id = 0;
    return &window;
}
