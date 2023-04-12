#ifndef PLATFORM_PREFS_H
#define PLATFORM_PREFS_H

#include "core/file.h"

int pref_saves_path(char *dest);

FILE *pref_file_open(const char *filename, const char *folder, const char *mode);

const char *pref_data_dir(void);

void pref_save_data_dir(const char *data_dir);

void pref_remove_file(const char *filename, const char *folder);
#endif // PLATFORM_PREFS_H
