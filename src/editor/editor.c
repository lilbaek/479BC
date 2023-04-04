#include "editor/editor.h"

#include "core/file.h"

static int is_active;
void editor_set_active(int active)
{
    is_active = active;
}

int editor_is_active(void)
{
    return is_active;
}
