#include "input.h"

#include "input/keyboard.h"
#include "input/mouse.h"

int input_go_back_requested(const mouse *m, const hotkeys *h)
{
    return m->right.went_up || (m->is_touch && m->left.double_click) || keyboard_is_esc_pressed();
}
