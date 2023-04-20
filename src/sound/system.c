#include "system.h"

#include "core/dir.h"
#include "sound/channel.h"
#include "sound/city.h"
#include "sound/device.h"
#include "sound/effect.h"
#include "sound/music.h"
#include "sound/speech.h"
#include "platform/file_manager.h"
#include "core/config.h"

#include <string.h>

static char channel_filenames[SOUND_CHANNEL_MAX][CHANNEL_FILENAME_MAX] = {
    "sound/default.wav"
};

static void correct_channel_filenames(void)
{
    for (int i = 1; i < SOUND_CHANNEL_MAX; i++) {
        if (!channel_filenames[i][0]) {
            continue;
        }
        char *original = channel_filenames[i];
        const char *corrected = dir_get_file(original, MAY_BE_LOCALIZED); // TODO
        if (!corrected) {
            channel_filenames[i][0] = 0;
        } else if (corrected != original) {
            strncpy(original, corrected, CHANNEL_FILENAME_MAX);
        }
    }
}

void sound_system_init(void)
{
    correct_channel_filenames();

    sound_device_open();
    sound_device_init_channels(SOUND_CHANNEL_MAX, channel_filenames);

    sound_city_set_volume(config_get(CONFIG_GENERAL_CITY_VOLUME));
    sound_effect_set_volume(config_get(CONFIG_GENERAL_EFFECTS_VOLUME));
    sound_music_set_volume(config_get(CONFIG_GENERAL_MUSIC_VOLUME));
    sound_speech_set_volume(config_get(CONFIG_GENERAL_SPEECH_VOLUME));
}

void sound_system_shutdown(void)
{
    sound_device_close();
}
