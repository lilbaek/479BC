#include "platform/prefs.h"

#include "core/log.h"
#include "platform/platform.h"

#include "SDL.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static FILE *open_pref_file(const char *filename, const char *folder, const char *mode)
{
    char *pref_dir = SDL_GetPrefPath("479BC", folder);
    log_info("open_pref_file - Location:",pref_dir,0);
    if (!pref_dir) {
        return NULL;
    }
    size_t dir_len = strlen(pref_dir);
    char *pref_file = malloc((strlen(filename) + dir_len + 2) * sizeof(char));
    if (!pref_file) {
        SDL_free(pref_dir);
        return NULL;
    }
    strcpy(pref_file, pref_dir);
    strcpy(&pref_file[dir_len], filename);
    SDL_free(pref_dir);

    FILE *fp = fopen(pref_file, mode);
    free(pref_file);
    return fp;
}

void pref_remove_file(const char *filename, const char *folder) {
    char *pref_dir = SDL_GetPrefPath("479BC", folder);
    log_info("pref_remove_file - Location:",pref_dir,0);
    if (!pref_dir) {
        return;
    }
    size_t dir_len = strlen(pref_dir);
    char *pref_file = malloc((strlen(filename) + dir_len + 2) * sizeof(char));
    if (!pref_file) {
        SDL_free(pref_dir);
        return;
    }
    strcpy(pref_file, pref_dir);
    strcpy(&pref_file[dir_len], filename);
    SDL_free(pref_dir);
    file_remove(pref_file);
}

int pref_saves_path(char *dest) {
    char *pref_dir = SDL_GetPrefPath("479BC", "saves");
    log_info("pref_saves_path - Location:",pref_dir,0);
    if (!pref_dir) {
        return 0;
    }
    strncpy(dest, pref_dir, FILE_NAME_MAX - 1);
    SDL_free(pref_dir);
    return 1;
}

FILE *pref_file_open(const char *filename, const char *folder, const char *mode) {
    return open_pref_file(filename, folder, mode);
}

const char *pref_data_dir(void)
{
    static char data_dir[1000];
    FILE *fp = open_pref_file("data_dir.txt", "settings", "r");
    if (fp) {
        size_t length = fread(data_dir, 1, 1000, fp);
        fclose(fp);
        if (length > 0) {
            data_dir[length] = 0;
            return data_dir;
        }
    }
    return NULL;
}

void pref_save_data_dir(const char *data_dir)
{
    FILE *fp = open_pref_file("data_dir.txt", "settings", "w");
    if (fp) {
        fwrite(data_dir, 1, strlen(data_dir), fp);
        fclose(fp);
    }
}
