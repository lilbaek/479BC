#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include "SDL.h"

void ui_window_setup(SDL_Window *win, SDL_Renderer *renderer);
void ui_input_begin();
void ui_input_process(SDL_Event *evt);
void ui_input_end();
void ui_teardown();
void ui_render();
void ui_font_standard();
void ui_font_icons();
void ui_font_large();
void ui_font_large_bold();
void ui_font_extra_large();
void ui_font_extra_large_bold();
struct nk_context* ui_context(void);

#endif