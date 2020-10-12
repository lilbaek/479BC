#ifndef WINDOW_OPTION_DIALOG_H
#define WINDOW_OPTION_DIALOG_H

#include <stdint.h>

typedef struct {
    int header;
    int desc;
    int image_id;
    uint8_t mod_author[24];
    uint8_t mod_name[24];
    uint8_t mod_image_id[24];
} option_menu_item;

void window_show_option_popup(int title, int subtitle, option_menu_item *options,
    void (*close_func)(int selection));



#endif // WINDOW_OPTION_DIALOG_H
