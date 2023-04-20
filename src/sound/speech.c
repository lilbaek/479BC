#include "speech.h"

#include "core/dir.h"
#include "sound/channel.h"
#include "sound/device.h"
#include "core/config.h"

void sound_speech_set_volume(int percentage)
{
    sound_device_set_channel_volume(SOUND_CHANNEL_SPEECH, percentage);
}

void sound_speech_play_file(const char *filename)
{
    if (!config_get(CONFIG_GENERAL_ENABLE_SPEECH)) {
        return;
    }
    sound_device_stop_channel(SOUND_CHANNEL_SPEECH);

    const char *cased_filename = dir_get_file(filename, MAY_BE_LOCALIZED);
    if (cased_filename) {
        sound_device_play_file_on_channel(cased_filename, SOUND_CHANNEL_SPEECH, config_get(CONFIG_GENERAL_SPEECH_VOLUME));
    }
}

void sound_speech_stop(void)
{
    sound_device_stop_channel(SOUND_CHANNEL_SPEECH);
}
