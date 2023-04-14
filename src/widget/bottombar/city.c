#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#include <stdio.h>
#include "city.h"
#include "window/ui_window.h"
#include "graphics/screen.h"
#include "window/nuklear.h"
#include "assets/assets.h"
#include "building/menu.h"
#include "building/construction.h"
#include "window/nuklear.h"
#include "core/lang.h"
#include "building/model.h"
#include "widget/minimap.h"
#include "widget/sidebar/common.h"
#include "graphics/window.h"
#include "game/orientation.h"
#include "core/config.h"
#include "game/undo.h"
#include "translation/translation.h"
#include "menu_renders.c"
#include "graphics/image.h"
#include "platform/renderer.h"

struct nk_rect bottom_bar_bounds() {
    return nk_recti(screen_dialog_offset_x() - 117, screen_height() - 150, 880, 150);
}

struct nk_rect minimap_button_bounds() { return nk_recti(0, screen_height() - 35, MINIMAP_WIDTH, 35); }



void render_bottom_bar(struct nk_context *ctx) {
    if (nk_begin(ctx, "city_bottom_bar", bottom_bar_bounds(),
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 6, 2);

        float row_layout[3];
        row_layout[0] = 810;
        row_layout[1] = 50;
        nk_layout_row(ctx, NK_STATIC, 88, 2, row_layout);
        if (nk_group_begin(ctx, "column1", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(ctx, 3, 2);
            nk_layout_row_begin(ctx, NK_DYNAMIC, 75, 15);
            {
                nk_layout_row_push(ctx, 0.02f);
                nk_spacer(ctx);
                if (primary_menu_data.selected == PRIMARY_MENU_DEFAULT) {
                    create_button(ctx, BUILDING_ROAD, "button_road");
                    create_button(ctx, BUILDING_HOUSE_VACANT_LOT, "button_tent");
                    create_expanding_button(ctx, BUILD_MENU_WATER, "button_water");
                    create_expanding_button(ctx, BUILD_MENU_FARMS, "button_farm");
                    create_expanding_button(ctx, BUILD_MENU_INDUSTRY, "button_industrial");
                    create_expanding_button(ctx, BUILD_MENU_RAW_MATERIALS, "button_materials");
                    create_expanding_button(ctx, BUILD_MENU_HEALTH, "button_health");
                    create_expanding_button(ctx, BUILD_MENU_EDUCATION, "button_education");
                    create_expanding_button(ctx, BUILD_MENU_ENTERTAINMENT, "button_entertainment");
                    create_expanding_button(ctx, BUILD_MENU_ADMINISTRATION, "button_goverment");
                    create_expanding_button(ctx, BUILD_MENU_ENGINEERING, "button_engineering");
                    create_expanding_button(ctx, BUILD_MENU_WORKSHOPS, "button_workshop");
                } else if (primary_menu_data.selected == PRIMARY_MENU_RELIGION) {
                    create_button_text(ctx, BUILDING_ORACLE);
                    create_button_text(ctx, BUILDING_SMALL_MAUSOLEUM);
                    create_button_text(ctx, BUILDING_LARGE_MAUSOLEUM);
                    create_button_text(ctx, BUILDING_NYMPHAEUM);
                    create_expanding_button(ctx, BUILD_MENU_SMALL_TEMPLES, "default");
                    create_expanding_button(ctx, BUILD_MENU_LARGE_TEMPLES, "default");
                    create_expanding_button(ctx, BUILD_MENU_GRAND_TEMPLES, "default");
                } else if (primary_menu_data.selected == PRIMARY_MENU_SECURITY) {
                    create_button_text(ctx, BUILDING_WALL);
                    create_button_text(ctx, BUILDING_TOWER);
                    create_button_text(ctx, BUILDING_GATEHOUSE);
                    create_button_text(ctx, BUILDING_MILITARY_ACADEMY);
                    create_button_text(ctx, BUILDING_BARRACKS);
                    create_button_text(ctx, BUILDING_MESS_HALL);
                    create_button_text(ctx, BUILDING_WATCHTOWER);
                    create_button_text(ctx, BUILDING_FORT_LEGIONARIES);
                    create_button_text(ctx, BUILDING_FORT_JAVELIN);
                    create_button_text(ctx, BUILDING_FORT_MOUNTED);
                } else if (primary_menu_data.selected == PRIMARY_MENU_BEUTIFICATION) {
                    create_button(ctx, BUILDING_GARDENS, "button_garden");
                    create_button(ctx, BUILDING_PLAZA, "default");
                    create_expanding_button(ctx, BUILD_MENU_STATUES, "default");
                    create_expanding_button(ctx, BUILD_MENU_TREES, "default");
                    create_expanding_button(ctx, BUILD_MENU_PARKS, "default");
                    create_expanding_button(ctx, BUILD_MENU_PATHS, "default");
                    create_expanding_button(ctx, BUILD_MENU_GOV_RES, "default");
                }
                nk_layout_row_end(ctx);
            }
            nk_group_end(ctx);
        }
        if (nk_group_begin(ctx, "column2", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(ctx, 40, 1);
            struct nk_style_button button;
            button = ctx->style.button;
            ctx->style.button.border = 1;
            ctx->style.button.rounding = 1;
            ctx->style.button.padding = nk_vec2(2, 0);
            ctx->style.button.image_padding = nk_vec2(10, 5);
            if (submenu_data.selected_submenu == SUBMENU_NONE) {
                create_tooltip(ctx, BUILDING_CLEAR_LAND);
            }
            if (render_nk_button_image(ctx, "button_shovel")) {
                build(BUILDING_CLEAR_LAND);
            }
            ctx->style.button = button;
            if (game_can_undo()) {
                nk_layout_row_dynamic(ctx, 40, 1);
                if (nk_button_label(ctx, "Undo")) {
                    game_undo_perform();
                    window_invalidate();
                }
            }
            nk_group_end(ctx);
        }
        nk_layout_row_dynamic(ctx, 50, 3);
        if (nk_group_begin(ctx, "column1", NK_WINDOW_NO_SCROLLBAR)) {
            nk_group_end(ctx);
        }
        if (nk_group_begin(ctx, "column2", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(ctx, 25, 3);

            render_primary_menu_button(ctx, PRIMARY_MENU_DEFAULT, gettext("General"));
            render_primary_menu_button(ctx, PRIMARY_MENU_RELIGION, gettext("Religion"));
            // render_primary_menu_button(ctx, PRIMARY_MENU_SECURITY, gettext("Security"));
            render_primary_menu_button(ctx, PRIMARY_MENU_BEUTIFICATION, gettext("Decorate"));

            nk_group_end(ctx);
        }
        if (nk_group_begin(ctx, "column3", NK_WINDOW_NO_SCROLLBAR)) {
            nk_group_end(ctx);
        }

    }
    nk_end(ctx);
}


void render_sub_menu(struct nk_context *ctx) {
    if (submenu_data.selected_submenu != SUBMENU_NONE) {
        const mouse *m = mouse_get();
        if (m->left.went_down &&
            !NK_INBOX(m->x, m->y, submenu_data.bounds.x, submenu_data.bounds.y, submenu_data.bounds.w,
                      submenu_data.bounds.h) && !submenu_data.initial_click) {
            submenu_data.selected_submenu = SUBMENU_NONE;
        } else {
            submenu_data.initial_click = 0;
            switch (submenu_data.selected_submenu) {
                case BUILD_MENU_WATER:
                    render_submenu_water(ctx);
                    break;
                case BUILD_MENU_HEALTH:
                    render_submenu_health(ctx);
                    break;
                case BUILD_MENU_SMALL_TEMPLES:
                    render_submenu_small_temples(ctx);
                    break;
                case BUILD_MENU_LARGE_TEMPLES:
                    render_submenu_large_temples(ctx);
                    break;
                case BUILD_MENU_GRAND_TEMPLES:
                    render_submenu_grand_temples(ctx);
                    break;
                case BUILD_MENU_EDUCATION:
                    render_submenu_education(ctx);
                    break;
                case BUILD_MENU_ENTERTAINMENT:
                    render_submenu_entertainment(ctx);
                    break;
                case BUILD_MENU_ADMINISTRATION:
                    render_submenu_administration(ctx);
                    break;
                case BUILD_MENU_ENGINEERING:
                    render_submenu_engineering(ctx);
                    break;
                case BUILD_MENU_FARMS:
                    render_submenu_farms(ctx);
                    break;
                case BUILD_MENU_RAW_MATERIALS:
                    render_submenu_raw_materials(ctx);
                    break;
                case BUILD_MENU_INDUSTRY:
                    render_submenu_industry(ctx);
                    break;
                case BUILD_MENU_WORKSHOPS:
                    render_submenu_workshop(ctx);
                    break;
                case BUILD_MENU_STATUES:
                    render_submenu_statues(ctx);
                    break;
                case BUILD_MENU_TREES:
                    render_submenu_trees(ctx);
                    break;
                case BUILD_MENU_PARKS:
                    render_submenu_parks(ctx);
                    break;
                case BUILD_MENU_PATHS:
                    render_submenu_paths(ctx);
                    break;
                case BUILD_MENU_GOV_RES:
                    render_submenu_gov(ctx);
                    break;
            }
        }
    }
}

void render_minimap_bar(struct nk_context *ctx) {
    struct nk_style_button button;
    button = ctx->style.button;
    ctx->style.button.border = 1;
    ctx->style.button.rounding = 1;
    ctx->style.button.padding = nk_vec2(2, 0);
    ctx->style.button.image_padding = nk_vec2(0, 1);
    if (nk_begin(ctx, "city_minimap_bar", minimap_button_bounds(),
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_begin(ctx, NK_DYNAMIC, 24, 4);
        {
            nk_layout_row_push(ctx, 0.03f);
            nk_spacer(ctx);
            nk_layout_row_push(ctx, 0.25f);
            if (nk_widget_is_hovered(ctx)) {
                nk_tooltip_bottom(ctx, "Show/hide grid");
            }
            if (render_nk_button_image(ctx, "button_grid")) {
                config_set(CONFIG_UI_SHOW_GRID, config_get(CONFIG_UI_SHOW_GRID) ^ 1);
            }
            ctx->style.button.image_padding = nk_vec2(4, 1);
            nk_layout_row_push(ctx, 0.22f);
            if (nk_widget_is_hovered(ctx)) {
                nk_tooltip_bottom(ctx, "Reset rotation");
            }
            if (render_nk_button_image(ctx, "button_reset_rotation")) {
                game_orientation_rotate_north();
                window_invalidate();
            }
            nk_layout_row_push(ctx, 0.22f);
            if (nk_widget_is_hovered(ctx)) {
                nk_tooltip_bottom(ctx, "Rotate left");
            }
            if (render_nk_button_image(ctx, "button_anticlockwise_rotation")) {
                window_invalidate();
                game_orientation_rotate_left();
            }
            nk_layout_row_push(ctx, 0.22f);
            if (nk_widget_is_hovered(ctx)) {
                nk_tooltip_bottom(ctx, "Rotate right");
            }
            if (render_nk_button_image(ctx, "button_clockwise_rotation")) {
                game_orientation_rotate_right();
                window_invalidate();
            }
            nk_layout_row_end(ctx);
        }
    }
    nk_end(ctx);
    ctx->style.button = button;
}


void widget_bottom_bar_city_draw_foreground(void) {
    struct nk_context *ctx = ui_context();
    render_bottom_bar(ctx);
    render_minimap_bar(ctx);
    render_sub_menu(ctx);
    ui_font_change(FONT_TYPE_STANDARD);
    widget_minimap_draw_decorated(0, (screen_height() - MINIMAP_HEIGHT) - 35, MINIMAP_WIDTH, MINIMAP_HEIGHT);
}

void widget_bottom_bar_city_draw_background(void) {
    widget_minimap_update(0);
    widget_minimap_draw_decorated(0, (screen_height() - MINIMAP_HEIGHT) - 35, MINIMAP_WIDTH, MINIMAP_HEIGHT);
}

int widget_bottom_bar_city_handle_mouse(const mouse *m) {
    if (NK_INBOX(m->x, m->y, bottom_bar_bounds().x, bottom_bar_bounds().y, bottom_bar_bounds().w,
                 bottom_bar_bounds().h)) {
        return 1;
    }
    if (submenu_data.selected_submenu == SUBMENU_BUILDING) {
        submenu_data.selected_submenu = SUBMENU_NONE;
        return 1;
    }
    if (widget_minimap_handle_mouse(m)) {
        return 1;
    }
    if (NK_INBOX(m->x, m->y, minimap_button_bounds().x, minimap_button_bounds().y, minimap_button_bounds().w,
                 minimap_button_bounds().h)) {
        return 1;
    }
    return 0;
}