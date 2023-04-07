#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include "SDL.h"
typedef enum {
    FONT_TYPE_STANDARD = 0,
    FONT_TYPE_ICONS = 1,
    FONT_TYPE_LARGE = 2,
    FONT_TYPE_LARGE_BOLD = 3,
    FONT_TYPE_EXTRA_LARGE = 4,
    FONT_TYPE_EXTRA_LARGE_BOLD = 5
} font_type;

void ui_window_setup(SDL_Window *win, SDL_Renderer *renderer);
void ui_input_begin();
void ui_input_process(SDL_Event *evt);
void ui_input_end();
void ui_teardown();
void ui_render();

void ui_font_change(font_type type);

struct nk_context* ui_context(void);

#endif