#include "ratings.h"

#include "city/ratings.h"
#include "graphics/generic_button.h"
#include "graphics/image.h"
#include "graphics/lang_text.h"
#include "graphics/panel.h"
#include "graphics/text.h"
#include "graphics/window.h"
#include "scenario/criteria.h"
#include "scenario/property.h"

#include "Data/CityInfo.h"

#define ADVISOR_HEIGHT 27

static void button_rating(int rating, int param2);

static generic_button rating_buttons[] = {
    { 80, 286, 190, 352, GB_IMMEDIATE, button_rating, button_none, 1, 0},
    {200, 286, 310, 352, GB_IMMEDIATE, button_rating, button_none, 2, 0},
    {320, 286, 430, 352, GB_IMMEDIATE, button_rating, button_none, 3, 0},
    {440, 286, 550, 352, GB_IMMEDIATE, button_rating, button_none, 4, 0},
};

static int focus_button_id;

static void draw_rating_column(int x_offset, int y_offset, int value, int has_reached)
{
    int image_base = image_group(GROUP_RATINGS_COLUMN);
    int y = y_offset - image_get(image_base)->height;
    image_draw(image_base, x_offset, y);
    for (int i = 0; i < 2 * value; i++) {
        image_draw(image_base + 1, x_offset + 11, --y);
    }
    if (value > 30 && has_reached) {
        image_draw(image_base + 2, x_offset - 6, y);
    }
}

static int draw_background()
{
    outer_panel_draw(0, 0, 40, ADVISOR_HEIGHT);
    image_draw(image_group(GROUP_ADVISOR_ICONS) + 3, 10, 10);
    int width = lang_text_draw(53, 0, 60, 12, FONT_LARGE_BLACK);
    if (!scenario_criteria_population_enabled() || scenario_is_open_play()) {
        lang_text_draw(53, 7, 80 + width, 17, FONT_NORMAL_BLACK);
    } else {
        width += lang_text_draw(53, 6, 80 + width, 17, FONT_NORMAL_BLACK);
        text_draw_number(scenario_criteria_population(), '@', ")", 80 + width, 17, FONT_NORMAL_BLACK);
    }

    image_draw(image_group(GROUP_RATINGS_BACKGROUND), 60, 48);

    // culture
    int culture = city_rating_culture();
    button_border_draw(80, 286, 110, 66, focus_button_id == 1);
    lang_text_draw_centered(53, 1, 80, 294, 110, FONT_NORMAL_BLACK);
    text_draw_number_centered(culture, 80, 309, 100, FONT_LARGE_BLACK);
    if (scenario_criteria_culture_enabled()) {
        width = text_draw_number(scenario_criteria_culture(), '@', " ", 85, 334, FONT_NORMAL_BLACK);
    } else {
        width = text_draw_number(0, '@', " ", 85, 334, FONT_NORMAL_BLACK);
    }
    lang_text_draw(53, 5, 85 + width, 334, FONT_NORMAL_BLACK);
    int has_reached = !scenario_criteria_culture_enabled() ||
        culture > scenario_criteria_culture() || culture == 100;
    draw_rating_column(110, 274, culture, has_reached);

    // prosperity
    int prosperity = city_rating_prosperity();
    button_border_draw(200, 286, 110, 66, focus_button_id == 2);
    lang_text_draw_centered(53, 2, 200, 294, 110, FONT_NORMAL_BLACK);
    text_draw_number_centered(prosperity, 200, 309, 100, FONT_LARGE_BLACK);
    if (scenario_criteria_prosperity_enabled()) {
        width = text_draw_number(scenario_criteria_prosperity(), '@', " ", 205, 334, FONT_NORMAL_BLACK);
    } else {
        width = text_draw_number(0, '@', " ", 205, 334, FONT_NORMAL_BLACK);
    }
    lang_text_draw(53, 5, 205 + width, 334, FONT_NORMAL_BLACK);
    has_reached = !scenario_criteria_prosperity_enabled() ||
        prosperity > scenario_criteria_prosperity() || prosperity == 100;
    draw_rating_column(230, 274, prosperity, has_reached);

    // peace
    int peace = city_rating_peace();
    button_border_draw(320, 286, 110, 66, focus_button_id == 3);
    lang_text_draw_centered(53, 3, 320, 294, 110, FONT_NORMAL_BLACK);
    text_draw_number_centered(peace, 320, 309, 100, FONT_LARGE_BLACK);
    if (scenario_criteria_peace_enabled()) {
        width = text_draw_number(scenario_criteria_peace(), '@', " ", 325, 334, FONT_NORMAL_BLACK);
    } else {
        width = text_draw_number(0, '@', " ", 325, 334, FONT_NORMAL_BLACK);
    }
    lang_text_draw(53, 5, 325 + width, 334, FONT_NORMAL_BLACK);
    has_reached = !scenario_criteria_peace_enabled() ||
        peace > scenario_criteria_peace() || peace == 100;
    draw_rating_column(350, 274, peace, has_reached);

    // favor
    int favor = city_rating_favor();
    button_border_draw(440, 286, 110, 66, focus_button_id == 4);
    lang_text_draw_centered(53, 4, 440, 294, 110, FONT_NORMAL_BLACK);
    text_draw_number_centered(favor, 440, 309, 100, FONT_LARGE_BLACK);
    if (scenario_criteria_favor_enabled()) {
        width = text_draw_number(scenario_criteria_favor(), '@', " ", 445, 334, FONT_NORMAL_BLACK);
    } else {
        width = text_draw_number(0, '@', " ", 445, 334, FONT_NORMAL_BLACK);
    }
    lang_text_draw(53, 5, 445 + width, 334, FONT_NORMAL_BLACK);
    has_reached = !scenario_criteria_favor_enabled() ||
        favor > scenario_criteria_favor() || favor == 100;
    draw_rating_column(470, 274, favor, has_reached);

    // bottom info box
    inner_panel_draw(64, 356, 32, 4);
    switch (Data_CityInfo.ratingAdvisorSelection) {
        case 1:
            lang_text_draw(53, 1, 72, 359, FONT_NORMAL_WHITE);
            if (city_rating_culture() <= 90) {
                lang_text_draw_multiline(53, 9 + Data_CityInfo.ratingAdvisorExplanationCulture,
                    72, 374, 496, FONT_NORMAL_WHITE);
            } else {
                lang_text_draw_multiline(53, 50, 72, 374, 496, FONT_NORMAL_WHITE);
            }
            break;
        case 2:
            lang_text_draw(53, 2, 72, 359, FONT_NORMAL_WHITE);
            if (Data_CityInfo.ratingProsperity <= 90) {
                lang_text_draw_multiline(53, 16 + Data_CityInfo.ratingAdvisorExplanationProsperity,
                    72, 374, 496, FONT_NORMAL_WHITE);
            } else {
                lang_text_draw_multiline(53, 51, 72, 374, 496, FONT_NORMAL_WHITE);
            }
            break;
        case 3:
            lang_text_draw(53, 3, 72, 359, FONT_NORMAL_WHITE);
            if (Data_CityInfo.ratingPeace <= 90) {
                lang_text_draw_multiline(53, 41 + Data_CityInfo.ratingAdvisorExplanationPeace,
                    72, 374, 496, FONT_NORMAL_WHITE);
            } else {
                lang_text_draw_multiline(53, 52, 72, 374, 496, FONT_NORMAL_WHITE);
            }
            break;
        case 4:
            lang_text_draw(53, 4, 72, 359, FONT_NORMAL_WHITE);
            if (Data_CityInfo.ratingFavor <= 90) {
                lang_text_draw_multiline(53, 27 + Data_CityInfo.ratingAdvisorExplanationFavor,
                    72, 374, 496, FONT_NORMAL_WHITE);
            } else {
                lang_text_draw_multiline(53, 53, 72, 374, 496, FONT_NORMAL_WHITE);
            }
            break;
        default:
            lang_text_draw_centered(53, 8, 72, 380, 496, FONT_NORMAL_WHITE);
            break;
    }

    return ADVISOR_HEIGHT;
}

static void draw_foreground()
{
    // culture
    button_border_draw(80, 286, 110, 66, focus_button_id == 1);
    // prosperity
    button_border_draw(200, 286, 110, 66, focus_button_id == 2);
    // peace
    button_border_draw(320, 286, 110, 66, focus_button_id == 3);
    // favor
    button_border_draw(440, 286, 110, 66, focus_button_id == 4);
}

static void handle_mouse(const mouse *m)
{
    generic_buttons_handle_mouse(m, 0, 0, rating_buttons, 4, &focus_button_id);
}

static void button_rating(int rating, int param2)
{
    Data_CityInfo.ratingAdvisorSelection = rating;
    window_invalidate();
}

static int get_tooltip_text()
{
    switch (focus_button_id) {
        case 1: return 102;
        case 2: return 103;
        case 3: return 104;
        case 4: return 105;
        default: return 0;
    }
}

const advisor_window_type *window_advisor_ratings()
{
    static const advisor_window_type window = {
        draw_background,
        draw_foreground,
        handle_mouse,
        get_tooltip_text
    };
    return &window;
}
