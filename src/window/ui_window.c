#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#include "SDL.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION

#include "cheat.h"
#include "platform/file_manager.h"
#include "nuklear.h"
#include "nuklear_sdl_renderer.h"
#include "ui_window.h"

/*#define INCLUDE_ALL */
/*#define INCLUDE_STYLE */
/*#define INCLUDE_CALCULATOR */
#define INCLUDE_OVERVIEW
/*#define INCLUDE_NODE_EDITOR */

#ifdef INCLUDE_ALL
#define INCLUDE_STYLE
#define INCLUDE_CALCULATOR
#define INCLUDE_CANVAS
#define INCLUDE_OVERVIEW
#define INCLUDE_NODE_EDITOR
#endif

#ifdef INCLUDE_STYLE
#include "demo/style.c"
#endif
#ifdef INCLUDE_CALCULATOR
#include "demo/calculator.c"
#endif
#ifdef INCLUDE_CANVAS
#include "demo/canvas.c"
#endif
#ifdef INCLUDE_OVERVIEW

#include "window/demo/overview.c"
#include "icons.h"
#include "core/log.h"


#endif
#ifdef INCLUDE_NODE_EDITOR
#include "demo/node_editor.c"
#endif
static struct {
    struct nk_context *ctx;
    struct nk_font *proggy_clean;
    struct nk_font *font_awesome;
    struct nk_font *roboto_regular_large;
    struct nk_font *roboto_icons;
    struct nk_font *roboto_bold_large;
    struct nk_font *roboto_regular_xlarge;
    struct nk_font *roboto_bold_xlarge;
} ui_data_struct;

nk_rune ranges_icons[] = {
        ICON_MIN_FA,
        ICON_MAX_FA,
        0
};

void ui_window_setup(SDL_Window *win, SDL_Renderer *renderer) {
    log_info("ui_window_setup", 0, 0);
    float font_scale = 1;
    ui_data_struct.ctx = nk_sdl_init(win, renderer);
    {
        {
            struct nk_font_atlas *atlas;
            struct nk_font_config config = nk_font_config(0);

            /* set up the font atlas and add desired font; note that font sizes are
             * multiplied by font_scale to produce better results at higher DPIs */
            nk_sdl_font_stash_begin(&atlas);
            ui_data_struct.proggy_clean = nk_font_atlas_add_from_file(atlas,
                                                                      platform_file_manager_asset_path(
                                                                              "ProggyClean.ttf"),
                                                                      13 * font_scale, &config);

            ui_data_struct.roboto_regular_large = nk_font_atlas_add_from_file(atlas, platform_file_manager_asset_path(
                    "Roboto-Regular.ttf"), 20 * font_scale, &config);
            ui_data_struct.roboto_bold_large = nk_font_atlas_add_from_file(atlas, platform_file_manager_asset_path(
                    "Roboto-Bold.ttf"), 20 * font_scale, &config);
            ui_data_struct.roboto_regular_xlarge = nk_font_atlas_add_from_file(atlas, platform_file_manager_asset_path(
                    "Roboto-Regular.ttf"), 28 * font_scale, &config);
            ui_data_struct.roboto_bold_xlarge = nk_font_atlas_add_from_file(atlas, platform_file_manager_asset_path(
                    "Roboto-Bold.ttf"), 28 * font_scale, &config);

            ui_data_struct.roboto_icons = nk_font_atlas_add_from_file(atlas, platform_file_manager_asset_path(
                    "Roboto-Regular.ttf"), 16, 0);

            struct nk_font_config cfg = nk_font_config(16);
            cfg.range = ranges_icons;
            cfg.merge_mode = 1;
            const char *path = platform_file_manager_asset_path(FONT_ICON_FILE_NAME_FA);
            nk_font_atlas_add_from_file(atlas, path, 13, &cfg);
            nk_sdl_font_stash_end();
        }

        /* this hack makes the font appear to be scaled down to the desired
         * size and is only necessary when font_scale > 1 */
        if(ui_data_struct.proggy_clean == NULL) {
            log_error("Could not load fonts", 0, 0);
        }
        ui_data_struct.proggy_clean->handle.height /= font_scale;
        ui_data_struct.roboto_regular_large->handle.height /= font_scale;
        ui_data_struct.roboto_bold_large->handle.height /= font_scale;
        ui_data_struct.roboto_regular_xlarge->handle.height /= font_scale;
        ui_data_struct.roboto_bold_xlarge->handle.height /= font_scale;
        nk_style_set_font(ui_data_struct.ctx, &ui_data_struct.proggy_clean->handle);
    }
}

void ui_input_begin() {
    nk_input_begin(ui_data_struct.ctx);
}

void ui_input_process(SDL_Event *evt) {
    nk_sdl_handle_event(evt);
}

void ui_input_end() {
    nk_input_end(ui_data_struct.ctx);
}

void ui_teardown() {
    nk_sdl_shutdown();
}

void ui_render() {
    nk_style_set_font(ui_data_struct.ctx, &ui_data_struct.proggy_clean->handle);
    // overview(ui_data_struct.ctx);
    window_cheat_render();
    nk_sdl_render(NK_ANTI_ALIASING_ON);
}

void ui_font_change(font_type type) {
    switch (type) {
        case FONT_TYPE_ICONS:
            nk_style_set_font(ui_data_struct.ctx, &ui_data_struct.roboto_icons->handle);
            break;
        case FONT_TYPE_STANDARD:
            nk_style_set_font(ui_data_struct.ctx, &ui_data_struct.proggy_clean->handle);
            break;
        case FONT_TYPE_LARGE:
            nk_style_set_font(ui_data_struct.ctx, &ui_data_struct.roboto_regular_large->handle);
            break;
        case FONT_TYPE_LARGE_BOLD:
            nk_style_set_font(ui_data_struct.ctx, &ui_data_struct.roboto_bold_large->handle);
            break;
        case FONT_TYPE_EXTRA_LARGE:
            nk_style_set_font(ui_data_struct.ctx, &ui_data_struct.roboto_regular_xlarge->handle);
            break;
        case FONT_TYPE_EXTRA_LARGE_BOLD:
            nk_style_set_font(ui_data_struct.ctx, &ui_data_struct.roboto_bold_xlarge->handle);
            break;
    }
}

struct nk_context *ui_context(void) {
    return ui_data_struct.ctx;
}