#include "game.h"

#include "building/properties.h"
#include "city/view.h"
#include "core/config.h"
#include "core/image.h"
#include "core/lang.h"
#include "core/locale.h"
#include "core/log.h"
#include "core/random.h"
#include "editor/editor.h"
#include "game/animation.h"
#include "game/file_editor.h"
#include "game/speed.h"
#include "game/state.h"
#include "game/tick.h"
#include "graphics/font.h"
#include "graphics/window.h"
#include "scenario/property.h"
#include "scenario/scenario.h"
#include "sound/city.h"
#include "sound/system.h"
#include "translation/translation.h"
#include "window/editor/map.h"
#include "window/main_menu.h"
#include "window/ui_window.h"
#include "scenario/map.h"
#include "map/tiles.h"
#include "map/natives.h"
#include "figuretype/animal.h"
#include "figuretype/water.h"
#include "figure/visited_buildings.h"
#include "map/routing_terrain.h"
#include "city/map.h"
#include "time.h"
#include "scenario/gladiator_revolt.h"
#include "scenario/emperor_change.h"
#include "scenario/criteria.h"
#include "empire/empire.h"
#include "figure/trader.h"
#include "scenario/invasion.h"
#include "city/military.h"
#include "scenario/request.h"
#include "scenario/demand_change.h"
#include "scenario/price_change.h"
#include "city/data.h"
#include "scenario/empire.h"
#include "empire/object.h"
#include "noise/mapgenerator.h"
#include "empire/city.h"
#include "map/image.h"
#include "city/data_private.h"

static void errlog(const char *msg)
{
    log_error(msg, 0, 0);
}

static encoding_type update_encoding(void)
{
    language_type language = locale_determine_language();
    encoding_type encoding = encoding_determine(language);
    log_info("Detected encoding:", 0, encoding);
    font_set_encoding(encoding);
    translation_load(language);
    return encoding;
}

int game_pre_init(void)
{
    SDL_Log("Running game_pre_init");
    SDL_Log("Running settings_load");
    scenario_settings_init();
    SDL_Log("Running scenario_settings_init");
    game_state_unpause();
    SDL_Log("Running game_state_unpause");
    if (!lang_load(0)) {
        SDL_Log("c3.eng' or 'c3_mm.eng' files not found or too large.");
        errlog("'c3.eng' or 'c3_mm.eng' files not found or too large.");
        return 0;
    }
    update_encoding();
    random_init();
    return 1;
}

int game_init(void)
{
    if (!image_load_climate(CLIMATE_CENTRAL, 0, 1, 0)) {
        errlog("unable to load main graphics");
        return 0;
    }
    if (!image_load_enemy(ENEMY_0_BARBARIAN)) {
        errlog("unable to load enemy graphics");
        return 0;
    }
    image_load_fonts(encoding_get());
    if (encoding_get() != ENCODING_WESTERN_EUROPE && encoding_get() != ENCODING_EASTERN_EUROPE) {
        errlog("unsupported encoding");
        return 0;
    }
    init_game_building__properties();
    load_custom_messages();
    sound_system_init();
    game_state_init();
    resource_init();
    log_info("window_main_menu_show", 0, 0);
    window_main_menu_show(1);
    return 1;
}

static int reload_language(int is_editor, int reload_images)
{
    if (!lang_load(is_editor)) {
        if (is_editor) {
            errlog("'c3_map.eng' or 'c3_map_mm.eng' files not found or too large.");
        } else {
            errlog("'c3.eng' or 'c3_mm.eng' files not found or too large.");
        }
        return 0;
    }
    encoding_type encoding = update_encoding();
    if (!is_editor) {
        load_custom_messages();
    }

    if (!image_load_fonts(encoding)) {
        errlog("unable to load font graphics");
        return 0;
    }
    if (!image_load_climate(scenario_property_climate(), is_editor, reload_images, 0)) {
        errlog("unable to load main graphics");
        return 0;
    }

    resource_init();

    return 1;
}

int game_init_editor(void)
{
    if (!reload_language(1, 0)) {
        return 0;
    }

    game_file_editor_clear_data();
    game_file_editor_create_scenario(2);

    if (city_view_is_sidebar_collapsed()) {
        city_view_toggle_sidebar();
    }

    editor_set_active(1);
    window_editor_map_show();
    return 1;
}

int game_create_random_scenario(int size, int seed)
{
    game_file_editor_clear_data();
    game_file_editor_create_random_scenario(size);

    scenario_set_custom(2);
    scenario_set_name("Custom");
    scenario_map_init();

    int empire_id = scenario_empire_id();
    empire_load(1, empire_id);
    empire_object_init_cities(empire_id);

    figure_init_scenario();
    figure_create_flotsam();

    map_routing_update_all(); // Need routing to create roads in generator

    noise_fill_map(seed);

    // initialize grids
    map_tiles_update_all_water();
    map_tiles_update_all_rocks();
    map_tiles_update_all_trees();
    map_tiles_update_all_shrubs();
    map_tiles_update_all_empty_land();
    map_tiles_add_entry_exit_sign();
    map_tiles_update_all_meadow();
    map_tiles_update_all_roads();
    map_tiles_update_all_highways();
    map_tiles_update_all_plazas();
    map_tiles_update_all_walls();

    map_natives_init();

    city_view_init();

    figure_create_fishing_points();
    figure_create_herds();
    figure_create_flotsam();
    figure_visited_buildings_init();

    map_routing_update_all();

    scenario_map_init_entry_exit();

    map_point entry = scenario_map_entry();
    map_point exit = scenario_map_exit();

    city_map_set_entry_point(entry.x, entry.y);
    city_map_set_exit_point(exit.x, exit.y);

    game_time_init(scenario_property_start_year());

    // set up events
    scenario_gladiator_revolt_init();
    scenario_emperor_change_init();
    scenario_criteria_init_max_year();

    empire_init_scenario();
    traders_clear();
    scenario_invasion_init();
    city_military_determine_distant_battle_city();
    scenario_request_init();
    scenario_demand_change_init();
    scenario_price_change_init();
    empire_unlock_all_resources();
    scenario_unlock_all_buildings();

    image_load_climate(scenario_property_climate(), 0, 0, 0);
    image_load_enemy(scenario_property_enemy());

    city_data_init_scenario();
    city_data.ratings.favor = 50; // TODO: Configurable
    map_image_update_all();

    city_view_set_camera(76, 152);
    setting_set_default_game_speed();
    game_state_unpause();

    return 1;
}

void game_exit_editor(void)
{
    if (!reload_language(0, 0)) {
        return;
    }
    editor_set_active(0);
    window_main_menu_show(1);
}

void game_run(void)
{
    game_animation_update();
    int num_ticks = game_speed_get_elapsed_ticks();
    for (int i = 0; i < num_ticks; i++) {
        game_tick_run();
        if (window_is_invalid()) {
            break;
        }
    }
}

void game_draw(void)
{
    window_draw(0);
    sound_city_play();
}

void game_exit(void)
{
    config_save();
    sound_system_shutdown();
}
