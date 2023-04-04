#ifndef WINDOW_POPUP_DIALOG_H
#define WINDOW_POPUP_DIALOG_H

#include <stdint.h>

void window_popup_dialog_show_confirmation(const char *custom_title, const char *custom_text,
    void (*close_func)(int accepted));

void window_popup_dialog_show_ex(const char *custom_title, const char *custom_text,
                                 void (*close_func)(int accepted), int has_ok_cancel_buttons);
#endif // WINDOW_POPUP_DIALOG_H
