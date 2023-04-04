#include "system.h"

#include "core/dir.h"
#include "game/settings.h"
#include "sound/channel.h"
#include "sound/city.h"
#include "sound/device.h"
#include "sound/effect.h"
#include "sound/music.h"
#include "sound/speech.h"
#include "platform/file_manager.h"

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
        const char *corrected = platform_file_manager_asset_path(original);
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

    sound_city_set_volume(setting_sound(SOUND_CITY)->volume);
    sound_effect_set_volume(setting_sound(SOUND_EFFECTS)->volume);
    sound_music_set_volume(setting_sound(SOUND_MUSIC)->volume);
    sound_speech_set_volume(setting_sound(SOUND_SPEECH)->volume);
}

void sound_system_shutdown(void)
{
    sound_device_close();
}
