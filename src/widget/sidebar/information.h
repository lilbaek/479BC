#ifndef WIDGET_SIDEBAR_INFORMATION_H
#define WIDGET_SIDEBAR_INFORMATION_H

#include "input/mouse.h"

void widget_sidebar_information_draw_background(void);
void widget_sidebar_information_draw_foreground(void);

int widget_sidebar_information_handle_mouse(const mouse *m);

#endif //WIDGET_SIDEBAR_INFORMATION_H
