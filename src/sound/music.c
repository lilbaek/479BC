#include "music.h"

#include "city/population.h"
#include "sound/device.h"
#include "platform/file_manager.h"
#include "core/config.h"

enum {
    TRACK_NONE = 0,
    TRACK_INTRO = 1,
    TRACK_CITY_1 = 2,
    TRACK_CITY_2 = 3,
    TRACK_MAX = 4
};

static struct {
    int current_track;
    int next_check;
} data = {TRACK_NONE, 0};

static const char mp3_tracks[][32] = {
    "",
    "music/track1.mp3",
    "music/track2.mp3",
    "music/track3.mp3",
};

void sound_music_set_volume(int percentage)
{
    sound_device_set_music_volume(percentage);
}

static void play_track(int track)
{
    sound_device_stop_music();
    if (track <= TRACK_NONE || track >= TRACK_MAX) {
        return;
    }
    const char *mp3_track = platform_file_manager_asset_path(mp3_tracks[track]);
    int volume = config_get(CONFIG_GENERAL_MUSIC_VOLUME);
    sound_device_play_music(mp3_track, volume);
    data.current_track = track;
}

void sound_music_play_intro(void)
{
    if (config_get(CONFIG_GENERAL_ENABLE_MUSIC)) {
        play_track(TRACK_INTRO);
    }
}

void sound_music_update(int force)
{
    if (data.next_check && !force) {
        --data.next_check;
        return;
    }
    if (!config_get(CONFIG_GENERAL_ENABLE_MUSIC)) {
        return;
    }
    int track;
    int population = city_population();
    if (population < 1000) {
        track = TRACK_CITY_2;
    } else if (population < 2000) {
        track = TRACK_CITY_1;
    } else if (population < 3000) {
        track = TRACK_CITY_2;
    } else if (population < 4000) {
        track = TRACK_CITY_1;
    } else {
        track = TRACK_CITY_1;
    }

    if (track == data.current_track) {
        return;
    }
    play_track(track);
    data.next_check = 10;
}

void sound_music_stop(void)
{
    sound_device_stop_music();
    data.current_track = TRACK_NONE;
    data.next_check = 0;
}
