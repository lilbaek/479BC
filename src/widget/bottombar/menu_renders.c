#include "graphics/image.h"
#include "platform/renderer.h"
#include "graphics/renderer.h"
#include "assets/gui_assets.h"

#define PRIMARY_MENU_DEFAULT 0
#define PRIMARY_MENU_RELIGION 1
#define PRIMARY_MENU_SECURITY 2
#define PRIMARY_MENU_BEUTIFICATION 3

#define SUBMENU_NONE -1
#define SUBMENU_BUILDING -2
static struct {
    build_menu_group selected_submenu;
    int num_items;
    float x;
    float y;
    int initial_click;
    struct nk_rect bounds;
} submenu_data = {SUBMENU_NONE};

static struct {
    int selected;
} primary_menu_data = {PRIMARY_MENU_DEFAULT};

nk_bool render_nk_button_image(struct nk_context *ctx, const char *name) {
    struct nk_style_button button;
    button = ctx->style.button;
    const image *img = gui_assets_get_image_id(name);
    struct nk_rect rect = nk_widget_bounds(ctx);
    ctx->style.button.border = 1;
    ctx->style.button.rounding = 1;
    ctx->style.button.padding = nk_vec2(0, 0);
    ctx->style.button.image_padding = nk_vec2((rect.w - img->width) / 2, (rect.h - img->height) / 2);

    const int width = gui_assets_get_atlas_width();
    const int height = gui_assets_get_atlas_height();
    nk_bool res = nk_button_image(ctx, nk_subimage_ptr(platform_renderer_get_texture(img->atlas.id), width, height,
                                                       nk_recti(img->atlas.x_offset, img->atlas.y_offset,
                                                                  img->width, img->height)));
    ctx->style.button = button;
    return res;
}

static void show_sub_menu(build_menu_group submenu, float x, float y) {
    if (submenu_data.selected_submenu == submenu) {
        submenu_data.selected_submenu = SUBMENU_NONE;
        return;
    }

    submenu_data.selected_submenu = submenu;
    submenu_data.num_items = building_menu_count_items(submenu);
    submenu_data.x = x;
    submenu_data.y = y;
    submenu_data.initial_click = 1;
}
static void build(int building_type) {
    building_construction_set_type(building_type);
    submenu_data.selected_submenu = SUBMENU_BUILDING;
}

void create_tooltip(struct nk_context *ctx, int building_type) {
    if (nk_widget_is_hovered(ctx)) {
        nk_tooltip_begin_bottom(ctx, 200, 80);
        nk_layout_row_dynamic(ctx, 20, 1);
        ui_font_change(FONT_TYPE_LARGE_BOLD);;
        nk_label(ctx, get_building_translation(building_type), NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 20, 1);
        ui_font_change(FONT_TYPE_STANDARD);
        int type = building_type;
        if (type == BUILDING_DRAGGABLE_RESERVOIR) {
            type = BUILDING_RESERVOIR;
        }
        int cost = model_get_building(type)->cost;
        if (type == BUILDING_FORT) {
            cost = 0;
        }
        if (type == BUILDING_MENU_GRAND_TEMPLES) {
            cost = 0;
        }
        if (type == BUILDING_MENU_SMALL_TEMPLES && submenu_data.selected_submenu == BUILD_MENU_SMALL_TEMPLES) {
            cost = model_get_building(BUILDING_SMALL_TEMPLE_CERES)->cost;
        }
        if (type == BUILDING_MENU_LARGE_TEMPLES && submenu_data.selected_submenu == BUILD_MENU_LARGE_TEMPLES) {
            cost = model_get_building(BUILDING_LARGE_TEMPLE_CERES)->cost;
        }
        if (cost) {
            char text[100];
            sprintf(text, "%d Db", cost);
            nk_label(ctx, text, NK_TEXT_LEFT);
        }
        nk_tooltip_end(ctx);
    }
}

void create_button_expanded(struct nk_context *ctx, int building_type, const char *image_name) {
    if (building_menu_is_enabled(building_type) == 0) {
        return;
    }
    create_tooltip(ctx, building_type);
    if (render_nk_button_image(ctx, image_name)) {
        build(building_type);
    }
}

void create_button_expanded_text(struct nk_context *ctx, int building_type) {
    if (building_menu_is_enabled(building_type) == 0) {
        return;
    }
    create_tooltip(ctx, building_type);
    if (nk_button_label(ctx, get_building_translation(building_type))) {
        build(building_type);
    }
}

void render_submenu_water(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 300 / 2 + 60 / 2, submenu_data.y - 110, 280,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_WATER", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 3);
        create_button_expanded(ctx, BUILDING_WELL, "button_well");
        create_button_expanded(ctx, BUILDING_FOUNTAIN, "button_fountain");
        create_button_expanded(ctx, BUILDING_DRAGGABLE_RESERVOIR, "button_reservoir");
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}

void render_submenu_farms(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 300 / 2 + 60 / 2, submenu_data.y - 110, 300,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_FARM", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 4);
        create_button_expanded_text(ctx, BUILDING_WHEAT_FARM);
        create_button_expanded_text(ctx, BUILDING_VEGETABLE_FARM);
        create_button_expanded_text(ctx, BUILDING_OLIVE_FARM);
        create_button_expanded_text(ctx, BUILDING_VINES_FARM);
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}

void render_submenu_raw_materials(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 300 / 2 + 60 / 2, submenu_data.y - 110, 300,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_RAW", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 5);
        create_button_expanded_text(ctx, BUILDING_CLAY_PIT);
        create_button_expanded_text(ctx, BUILDING_MARBLE_QUARRY);
        create_button_expanded_text(ctx, BUILDING_IRON_MINE);
        create_button_expanded_text(ctx, BUILDING_TIMBER_YARD);
        create_button_expanded_text(ctx, BUILDING_GOLD_MINE);
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}

void render_submenu_industry(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 300 / 2 + 60 / 2, submenu_data.y - 110, 300,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_INDUSTRY", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 4);
        create_button_expanded_text(ctx, BUILDING_MARKET);
        create_button_expanded_text(ctx, BUILDING_GRANARY);
        create_button_expanded_text(ctx, BUILDING_WAREHOUSE);
        create_button_expanded_text(ctx, BUILDING_CARAVANSERAI);
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}

void render_submenu_workshop(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 300 / 2 + 60 / 2, submenu_data.y - 110, 300,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_WORKSHOP", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 5);
        create_button_expanded_text(ctx, BUILDING_WINE_WORKSHOP);
        create_button_expanded_text(ctx, BUILDING_OIL_WORKSHOP);
        create_button_expanded_text(ctx, BUILDING_WEAPONS_WORKSHOP);
        create_button_expanded_text(ctx, BUILDING_FURNITURE_WORKSHOP);
        create_button_expanded_text(ctx, BUILDING_POTTERY_WORKSHOP);
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}

void render_submenu_health(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 300 / 2 + 60 / 2, submenu_data.y - 110, 300,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_HEALTH", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 4);
        create_button_expanded_text(ctx, BUILDING_DOCTOR);
        create_button_expanded_text(ctx, BUILDING_HOSPITAL);
        create_button_expanded_text(ctx, BUILDING_BARBER);
        create_button_expanded_text(ctx, BUILDING_BATHHOUSE);
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}

void render_submenu_small_temples(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 440 / 2 + 60 / 2, submenu_data.y - 110, 440,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_SMALL_TEMPLES", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 5);
        create_button_expanded_text(ctx, BUILDING_SMALL_TEMPLE_CERES);
        create_button_expanded_text(ctx, BUILDING_SMALL_TEMPLE_MARS);
        create_button_expanded_text(ctx, BUILDING_SMALL_TEMPLE_MERCURY);
        create_button_expanded_text(ctx, BUILDING_SMALL_TEMPLE_NEPTUNE);
        create_button_expanded_text(ctx, BUILDING_SMALL_TEMPLE_VENUS);
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}


void render_submenu_large_temples(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 440 / 2 + 60 / 2, submenu_data.y - 110, 440,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_LARGE_TEMPLES", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 5);
        create_button_expanded_text(ctx, BUILDING_LARGE_TEMPLE_CERES);
        create_button_expanded_text(ctx, BUILDING_LARGE_TEMPLE_MARS);
        create_button_expanded_text(ctx, BUILDING_LARGE_TEMPLE_MERCURY);
        create_button_expanded_text(ctx, BUILDING_LARGE_TEMPLE_NEPTUNE);
        create_button_expanded_text(ctx, BUILDING_LARGE_TEMPLE_VENUS);
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}
void render_submenu_grand_temples(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 440 / 2 + 60 / 2, submenu_data.y - 110, 440,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_GRAND_TEMPLES", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 5);
        create_button_expanded_text(ctx, BUILDING_GRAND_TEMPLE_CERES);
        create_button_expanded_text(ctx, BUILDING_GRAND_TEMPLE_MARS);
        create_button_expanded_text(ctx, BUILDING_GRAND_TEMPLE_MERCURY);
        create_button_expanded_text(ctx, BUILDING_GRAND_TEMPLE_NEPTUNE);
        create_button_expanded_text(ctx, BUILDING_GRAND_TEMPLE_VENUS);
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}

void render_submenu_administration(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 440 / 2 + 60 / 2, submenu_data.y - 110, 440,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_ADMIN", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 5);
        create_button_expanded_text(ctx, BUILDING_PREFECTURE);
        create_button_expanded_text(ctx, BUILDING_ROADBLOCK);
        create_button_expanded_text(ctx, BUILDING_FORUM);
        create_button_expanded_text(ctx, BUILDING_SENATE_UPGRADED);
        create_button_expanded_text(ctx, BUILDING_CITY_MINT);
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}

void render_submenu_education(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 400 / 2 + 60 / 2, submenu_data.y - 110, 400,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_EDUCATION", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 4);
        create_button_expanded_text(ctx, BUILDING_SCHOOL);
        create_button_expanded_text(ctx, BUILDING_ACADEMY);
        create_button_expanded_text(ctx, BUILDING_LIBRARY);
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}

void render_submenu_entertainment(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 400 / 2 + 60 / 2, submenu_data.y - 110, 400,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_ENT", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 10);
        create_button_expanded_text(ctx, BUILDING_THEATER);
        create_button_expanded_text(ctx, BUILDING_TAVERN);
        create_button_expanded_text(ctx, BUILDING_AMPHITHEATER);
        create_button_expanded_text(ctx, BUILDING_ARENA);
        create_button_expanded_text(ctx, BUILDING_COLOSSEUM);
        create_button_expanded_text(ctx, BUILDING_HIPPODROME);
        create_button_expanded_text(ctx, BUILDING_GLADIATOR_SCHOOL);
        create_button_expanded_text(ctx, BUILDING_LION_HOUSE);
        create_button_expanded_text(ctx, BUILDING_ACTOR_COLONY);
        create_button_expanded_text(ctx, BUILDING_CHARIOT_MAKER);
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}

void render_submenu_engineering(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 400 / 2 + 60 / 2, submenu_data.y - 110, 400,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_ENG", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 9);
        create_button_expanded_text(ctx, BUILDING_ENGINEERS_POST);
        create_button_expanded_text(ctx, BUILDING_LOW_BRIDGE);
        create_button_expanded_text(ctx, BUILDING_SHIP_BRIDGE);
        create_button_expanded_text(ctx, BUILDING_SHIPYARD);
        create_button_expanded_text(ctx, BUILDING_DOCK);
        create_button_expanded_text(ctx, BUILDING_WHARF);
        create_button_expanded_text(ctx, BUILDING_WORKCAMP);
        create_button_expanded_text(ctx, BUILDING_ARCHITECT_GUILD);
        create_button_expanded_text(ctx, BUILDING_LIGHTHOUSE);
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}

void render_submenu_statues(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 400 / 2 + 60 / 2, submenu_data.y - 110, 400,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_STATUES", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 8);
        create_button_expanded_text(ctx, BUILDING_SMALL_STATUE);
        create_button_expanded_text(ctx, BUILDING_SMALL_STATUE_ALT);
        create_button_expanded_text(ctx, BUILDING_SMALL_STATUE_ALT_B);
        create_button_expanded_text(ctx, BUILDING_GLADIATOR_STATUE);
        create_button_expanded_text(ctx, BUILDING_MEDIUM_STATUE);
        create_button_expanded_text(ctx, BUILDING_LEGION_STATUE);
        create_button_expanded_text(ctx, BUILDING_LARGE_STATUE);
        create_button_expanded_text(ctx, BUILDING_HORSE_STATUE);
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}

void render_submenu_trees(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 400 / 2 + 60 / 2, submenu_data.y - 110, 400,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_TREES", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 8);
        create_button_expanded_text(ctx, BUILDING_PINE_TREE);
        create_button_expanded_text(ctx, BUILDING_FIR_TREE);
        create_button_expanded_text(ctx, BUILDING_OAK_TREE);
        create_button_expanded_text(ctx, BUILDING_ELM_TREE);
        create_button_expanded_text(ctx, BUILDING_FIG_TREE);
        create_button_expanded_text(ctx, BUILDING_PLUM_TREE);
        create_button_expanded_text(ctx, BUILDING_PALM_TREE);
        create_button_expanded_text(ctx, BUILDING_DATE_TREE);
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}

void render_submenu_parks(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 400 / 2 + 60 / 2, submenu_data.y - 110, 400,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_PARKS", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 10);
        create_button_expanded_text(ctx, BUILDING_OBELISK);
        create_button_expanded_text(ctx, BUILDING_DECORATIVE_COLUMN);
        create_button_expanded_text(ctx, BUILDING_COLONNADE);
        create_button_expanded_text(ctx, BUILDING_HEDGE_LIGHT);
        create_button_expanded_text(ctx, BUILDING_HEDGE_DARK);
        create_button_expanded_text(ctx, BUILDING_GARDEN_WALL);
        create_button_expanded_text(ctx, BUILDING_ROOFED_GARDEN_WALL);
        create_button_expanded_text(ctx, BUILDING_PAVILION_BLUE);
        create_button_expanded_text(ctx, BUILDING_SMALL_POND);
        create_button_expanded_text(ctx, BUILDING_LARGE_POND);
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}

void render_submenu_paths(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 400 / 2 + 60 / 2, submenu_data.y - 110, 400,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_PATHS", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 9);
        create_button_expanded_text(ctx, BUILDING_GARDEN_PATH);
        create_button_expanded_text(ctx, BUILDING_PINE_PATH);
        create_button_expanded_text(ctx, BUILDING_FIR_PATH);
        create_button_expanded_text(ctx, BUILDING_OAK_PATH);
        create_button_expanded_text(ctx, BUILDING_ELM_PATH);
        create_button_expanded_text(ctx, BUILDING_FIG_PATH);
        create_button_expanded_text(ctx, BUILDING_PLUM_PATH);
        create_button_expanded_text(ctx, BUILDING_PALM_PATH);
        create_button_expanded_text(ctx, BUILDING_DATE_PATH);
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}

void render_submenu_gov(struct nk_context *ctx) {
    submenu_data.bounds = nk_recti(submenu_data.x - 400 / 2 + 60 / 2, submenu_data.y - 110, 400,
                                   SUBMENU_DEFAULT_HEIGHT);
    if (nk_begin(ctx, "WINDOW_BUILD_MENU_BOTTOM_BAR_GOV", submenu_data.bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 3, 1);
        nk_layout_row_dynamic(ctx, 75, 3);
        create_button_expanded_text(ctx, BUILDING_GOVERNORS_HOUSE);
        create_button_expanded_text(ctx, BUILDING_GOVERNORS_VILLA);
        create_button_expanded_text(ctx, BUILDING_GOVERNORS_PALACE);
        nk_layout_row_dynamic(ctx, 3, 1);
    }
    nk_end(ctx);
}

void create_tooltip_submenu(struct nk_context *ctx, int submenu) {
    if (nk_widget_is_hovered(ctx)) {
        nk_tooltip_begin_bottom(ctx, 200, 60);
        nk_layout_row_dynamic(ctx, 20, 1);
        ui_font_change(FONT_TYPE_LARGE_BOLD);;
        char *text;
        switch (submenu) {
            case BUILD_MENU_WATER:
                text = gettext("Water");
                break;
            case BUILD_MENU_HEALTH:
                text = gettext("Health");
                break;
            case BUILD_MENU_TEMPLES:
                text = gettext("Religion");
                break;
            case BUILD_MENU_EDUCATION:
                text = gettext("Education");
                break;
            case BUILD_MENU_ENTERTAINMENT:
                text = gettext("Entertainment");
                break;
            case BUILD_MENU_ADMINISTRATION:
                text = gettext("Administration");
                break;
            case BUILD_MENU_ENGINEERING:
                text = gettext("Engineering");
                break;
            case BUILD_MENU_SECURITY:
                text = gettext("Security");
                break;
            case BUILD_MENU_INDUSTRY:
                text = gettext("Distribution");
                break;
            case BUILD_MENU_SMALL_TEMPLES:
                text = gettext("Small temples");
                break;
            case BUILD_MENU_LARGE_TEMPLES:
                text = gettext("Large temples");
                break;
            case BUILD_MENU_GRAND_TEMPLES:
                text = gettext("Grand temples");
                break;
            case BUILD_MENU_FARMS:
                text = gettext("Farms");
                break;
            case BUILD_MENU_RAW_MATERIALS:
                text = gettext("Raw materials");
                break;
            case BUILD_MENU_WORKSHOPS:
                text = gettext("Workshops");
                break;
            case BUILD_MENU_STATUES:
                text = gettext("Statues");
                break;
            case BUILD_MENU_TREES:
                text = gettext("Trees");
                break;
            case BUILD_MENU_PARKS:
                text = gettext("Parks");
                break;
            case BUILD_MENU_PATHS:
                text = gettext("Paths");
                break;
            case BUILD_MENU_GOV_RES:
                text = gettext("Governor's mansions");
                break;
            default:
                text = "Unknown";
                break;
        }
        nk_label(ctx, text, NK_TEXT_LEFT);
        nk_tooltip_end(ctx);
        ui_font_change(FONT_TYPE_STANDARD);
    }
}

void create_expanding_button(struct nk_context *ctx, int submenu, const char *image_name) {
    if (building_menu_count_items(submenu) <= 0) {
        return;
    }
    nk_layout_row_push(ctx, 0.08f);
    if (submenu_data.selected_submenu == SUBMENU_NONE) {
        create_tooltip_submenu(ctx, submenu);
    }
    struct nk_rect bounds = nk_widget_bounds(ctx);
    if (render_nk_button_image(ctx, image_name)) {
        show_sub_menu(submenu, bounds.x, bounds.y);
    }
}

void create_button(struct nk_context *ctx, int building_type, const char *image_name) {
    if (building_menu_is_enabled(building_type) == 0) {
        return;
    }
    nk_layout_row_push(ctx, 0.08f);
    if (submenu_data.selected_submenu == SUBMENU_NONE) {
        create_tooltip(ctx, building_type);
    }
    if (render_nk_button_image(ctx, image_name)) {
        build(building_type);
    }
}

void create_button_text(struct nk_context *ctx, int building_type) {
    if (building_menu_is_enabled(building_type) == 0) {
        return;
    }
    nk_layout_row_push(ctx, 0.08f);
    if (submenu_data.selected_submenu == SUBMENU_NONE) {
        create_tooltip(ctx, building_type);
    }
    if (nk_button_label(ctx, get_building_translation(building_type))) {
        build(building_type);
    }
}

void render_primary_menu_button(struct nk_context *ctx, int button, char *text) {
    struct nk_command_buffer *pBuffer = nk_window_get_canvas(ctx);
    struct nk_rect rect = nk_widget_bounds(ctx);
    if (nk_widget_is_mouse_clicked(ctx, NK_BUTTON_LEFT)) {
        primary_menu_data.selected = button;
    }
    if (primary_menu_data.selected == button) {
        nk_label_colored(ctx, text, NK_TEXT_CENTERED, nk_rgb(228, 204, 167));
        nk_fill_rect(nk_window_get_canvas(ctx), nk_rect(rect.x, rect.y + rect.h, rect.w, 1), 0, nk_rgb(228, 204, 167));
    } else {
        if (nk_widget_is_hovered(ctx)) {
            nk_label_colored(ctx, text, NK_TEXT_CENTERED, nk_rgb(228, 204, 167));
        } else {
            nk_label_colored(ctx, text, NK_TEXT_CENTERED, nk_rgb(255, 255, 255));
        }
    }
}