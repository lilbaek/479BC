#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#include "information.h"
#include "window/ui_window.h"
#include "window/nuklear.h"
#include "graphics/screen.h"
#include "translation/translation.h"
#include "city/labor.h"
#include "city/gods.h"
#include "scenario/criteria.h"
#include "city/ratings.h"
#include "city/message.h"
#include "core/lang.h"
#include "window/message_dialog.h"
#include "window/city.h"

static struct {
    struct nk_rect city_detail_bounds;
    struct nk_rect city_event_bounds;
} information_data;


struct nk_rect city_detail_bounds() {
    return nk_recti(screen_width() - 280, 0, 280, 180);
}

struct nk_rect city_event_bounds() {
    return nk_recti(screen_width() - 280, information_data.city_detail_bounds.h + 10, 280, 180);
}


void render_city_information(struct nk_context * ctx);
void render_events_window(struct nk_context * ctx);

void render_city_information_window(struct nk_context *ctx);

void widget_sidebar_information_draw_background(void) {

}

static int count_happy_gods(void) {
    int happy_gods = 0;
    for (int god = 0; god < MAX_GODS; god++) {
        if (city_god_happy_bolts(god) > 0) {
            happy_gods++;
        }
    }
    return happy_gods;
}

static int count_angry_gods(void) {
    int angry_gods = 0;
    for (int god = 0; god < 5; god++) {
        if (city_god_wrath_bolts(god) > 0) {
            angry_gods++;
        }
    }
    return angry_gods;
}

void create_row_spacer(struct nk_context *ctx) {
    nk_layout_row_push(ctx, 0.02f);
    nk_spacer(ctx);
    nk_layout_row_push(ctx, 0.45f);
}

void widget_sidebar_information_draw_foreground(void) {
    struct nk_context *ctx = ui_context();
    render_city_information_window(ctx);
    render_events_window(ctx);
}

void render_city_information_window(struct nk_context *ctx) {
    if (nk_begin_titled(ctx, "city_information", gettext("City information"), city_detail_bounds(),
                 NK_WINDOW_MINIMIZABLE | NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        render_city_information(ctx);
        information_data.city_detail_bounds = nk_window_get_bounds(ctx);
    } else {
        information_data.city_detail_bounds = nk_window_get_bounds(ctx);
        information_data.city_detail_bounds.h = 32;
    }
    nk_end(ctx);
}

void render_events_window(struct nk_context *ctx) {
    if (nk_begin_titled(ctx, "city_events", gettext("Events"), city_event_bounds(),
                        NK_WINDOW_MINIMIZABLE | NK_WINDOW_BORDER)) {
        int max = city_message_count();
        for (int i = 0; i < max; i++) {
            nk_layout_row_dynamic(ctx, 25, 1);
            const city_message *msg = city_message_get(i);
            const lang_message *lang_msg = lang_get_message(city_message_get_text_id(msg->message_type));
            if(nk_button_label(ctx, lang_msg->title.text)) {
                window_message_dialog_show(city_message_get_text_id(msg->message_type), window_city_draw_all);
            }
        }
        information_data.city_event_bounds = nk_window_get_bounds(ctx);
    } else {
        information_data.city_event_bounds = nk_window_get_bounds(ctx);
        information_data.city_event_bounds.h = 32;
    }
    nk_end(ctx);
}

void render_city_information(struct nk_context *ctx) {
    nk_layout_row_begin(ctx, NK_DYNAMIC, 25, 2);
    {
        create_row_spacer(ctx);
        nk_label(ctx, gettext("Unemployment:"), NK_TEXT_LEFT);
        nk_labelf(ctx, NK_TEXT_RIGHT, gettext("%d%% (%d)"), city_labor_unemployment_percentage(),
                  city_labor_workers_unemployed() - city_labor_workers_needed());
        nk_layout_row_end(ctx);
    }
    /*
nk_layout_row_begin(ctx, NK_DYNAMIC, 25, 2);
{
    create_row_spacer(ctx);
    nk_label(ctx, gettext("Invasions:"), NK_TEXT_LEFT);
    nk_labelf(ctx, NK_TEXT_RIGHT, gettext("Todo"));
    nk_layout_row_end(ctx);
}*/
    nk_layout_row_begin(ctx, NK_DYNAMIC, 25, 2);
    {
        create_row_spacer(ctx);
        nk_label(ctx, gettext("Gods:"), NK_TEXT_LEFT);
        nk_labelf(ctx, NK_TEXT_RIGHT, gettext("%d \xef\x81\xaa / %d \xef\x86\x85"), count_angry_gods(),
                  count_happy_gods());
        nk_layout_row_end(ctx);
    }
    nk_layout_row_begin(ctx, NK_DYNAMIC, 25, 2);
    {
        create_row_spacer(ctx);
        nk_label(ctx, gettext("Prosperity:"), NK_TEXT_LEFT);
        nk_labelf(ctx, NK_TEXT_RIGHT, gettext("%d%/%d"), city_rating_prosperity(), scenario_criteria_prosperity());
        nk_layout_row_end(ctx);
    }
    nk_layout_row_begin(ctx, NK_DYNAMIC, 25, 2);
    {
        create_row_spacer(ctx);
        nk_label(ctx, gettext("Culture:"), NK_TEXT_LEFT);
        nk_labelf(ctx, NK_TEXT_RIGHT, gettext("%d%/%d"), city_rating_culture(), scenario_criteria_culture());
        nk_layout_row_end(ctx);
    }
    nk_layout_row_begin(ctx, NK_DYNAMIC, 25, 2);
    {
        create_row_spacer(ctx);
        nk_label(ctx, gettext("Favor:"), NK_TEXT_LEFT);
        nk_labelf(ctx, NK_TEXT_RIGHT, gettext("%d%/%d"), city_rating_favor(), scenario_criteria_favor());
        nk_layout_row_end(ctx);
    }
    /*
nk_layout_row_begin(ctx, NK_DYNAMIC, 25, 2);
{
    create_row_spacer(ctx);
    nk_label(ctx, gettext("Peace:"), NK_TEXT_LEFT);
    nk_labelf(ctx, NK_TEXT_RIGHT, gettext("%d%/%d"), city_rating_peace(), scenario_criteria_peace());
    nk_layout_row_end(ctx);
}*/
}

int widget_sidebar_information_handle_mouse(const mouse *m) {
    if (NK_INBOX(m->x, m->y, information_data.city_detail_bounds.x, information_data.city_detail_bounds.y, information_data.city_detail_bounds.w,
                 information_data.city_detail_bounds.h)) {
        return 1;
    }
    if (NK_INBOX(m->x, m->y, information_data.city_event_bounds.x, information_data.city_event_bounds.y, information_data.city_event_bounds.w,
                 information_data.city_event_bounds.h)) {
        return 1;
    }
    return 0;
}
