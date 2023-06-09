#ifndef GUI_ASSETS_H
#define GUI_ASSETS_H

#include "image.h"

void gui_assets_init();
const image *gui_assets_get_image_id(const char *image_name);

int gui_assets_get_atlas_width();
int gui_assets_get_atlas_height();
#endif //GUI_ASSETS_H
