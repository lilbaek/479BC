#ifndef WIDGET_BOTTOMBAR_CITY_H
#define WIDGET_BOTTOMBAR_CITY_H

#include "graphics/tooltip.h"
#include "input/mouse.h"


static const int SUBMENU_DEFAULT_HEIGHT = 100;

void widget_bottom_bar_city_draw_background(void);
void widget_bottom_bar_city_draw_foreground(void);

int widget_bottom_bar_city_handle_mouse(const mouse *m);

#endif // WIDGET_BOTTOMBAR_CITY_H
