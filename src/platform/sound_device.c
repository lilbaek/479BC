#include "core/calc.h"
#include "core/config.h"
#include "core/file.h"
#include "core/log.h"
#include "sound/device.h"
#include "platform/platform.h"
#include "platform/vita/vita.h"

#include "SDL.h"
#include "SDL_mixer.h"

#include <stdlib.h>
#include <string.h>

#ifdef __vita__
#include <psp2/io/fcntl.h>
#endif

#define AUDIO_RATE 22050
#define AUDIO_FORMAT AUDIO_S16
#define AUDIO_CHANNELS 2
#define AUDIO_BUFFERS 1024

#define MAX_CHANNELS 160

#if SDL_VERSION_ATLEAST(2, 0, 7)
#define USE_SDL_AUDIOSTREAM
#endif
#define HAS_AUDIOSTREAM() (platform_sdl_version_at_least(2, 0, 7))

#ifdef __vita__
static struct {
    char filename[FILE_NAME_MAX];
    char *buffer;
    int size;
} vita_music_data;
#endif

typedef struct {
    const char *filename;
    Mix_Chunk *chunk;
} sound_channel;

static struct {
    int initialized;
    Mix_Music *music;
    sound_channel channels[MAX_CHANNELS];
} data;

static struct {
    SDL_AudioFormat format;
    SDL_AudioFormat dst_format;
#ifdef USE_SDL_AUDIOSTREAM
    SDL_AudioStream *stream;
    int use_audiostream;
#endif
    SDL_AudioCVT cvt;
    unsigned char *buffer;
    int buffer_size;
    int cur_read;
    int cur_write;
} custom_music;

static int percentage_to_volume(int percentage)
{
    int master_percentage = config_get(CONFIG_GENERAL_MASTER_VOLUME);
    return calc_adjust_with_percentage(percentage, master_percentage) * SDL_MIX_MAXVOLUME / 100;
}

static void init_channels(void)
{
    data.initialized = 1;
    for (int i = 0; i < MAX_CHANNELS; i++) {
        data.channels[i].chunk = 0;
    }
}

void sound_device_open(void)
{
#ifdef USE_SDL_AUDIOSTREAM
    custom_music.use_audiostream = HAS_AUDIOSTREAM();
#endif

    // Windows: use directsound by default, as wasapi has issues
#ifdef __WINDOWS__
    SDL_AudioInit("directsound");
#endif
    if (0 == Mix_OpenAudio(AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNELS, AUDIO_BUFFERS)) {
        SDL_Log("Using default audio driver: %s", SDL_GetCurrentAudioDriver());
        init_channels();
        return;
    }
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Sound failed to initialize using default driver: %s", Mix_GetError());
    // Try to work around SDL choosing the wrong driver on Windows sometimes
    for (int i = 0; i < SDL_GetNumAudioDrivers(); i++) {
        const char *driver_name = SDL_GetAudioDriver(i);
        if (SDL_strcmp(driver_name, "disk") == 0 || SDL_strcmp(driver_name, "dummy") == 0) {
            // Skip "write-to-disk" and dummy drivers
            continue;
        }
        if (0 == SDL_AudioInit(driver_name) &&
            0 == Mix_OpenAudio(AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNELS, AUDIO_BUFFERS)) {
            SDL_Log("Using audio driver: %s", driver_name);
            init_channels();
            return;
        } else {
            SDL_Log("Not using audio driver %s, reason: %s", driver_name, SDL_GetError());
        }
    }
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Sound failed to initialize: %s", Mix_GetError());
    int max = SDL_GetNumAudioDevices(0);
    SDL_Log("Number of audio devices: %d", max);
    for (int i = 0; i < max; i++) {
        SDL_Log("Audio device: %s", SDL_GetAudioDeviceName(i, 0));
    }
}

void sound_device_close(void)
{
    if (data.initialized) {
        for (int i = 0; i < MAX_CHANNELS; i++) {
            sound_device_stop_channel(i);
        }
        Mix_CloseAudio();
        data.initialized = 0;
    }
}

static Mix_Chunk *load_chunk(const char *filename)
{
    if (filename[0]) {
#if defined(__vita__) || defined(__ANDROID__)
        FILE *fp = file_open(filename, "rb");
        if (!fp) {
            return NULL;
        }
        SDL_RWops *sdl_fp = SDL_RWFromFP(fp, SDL_TRUE);
        return Mix_LoadWAV_RW(sdl_fp, 1);
#else
        return Mix_LoadWAV(filename);
#endif
    } else {
        return NULL;
    }
}

static int load_channel(sound_channel *channel)
{
    return 0; // TODO: add sound again
    /*
    if (!channel->chunk && channel->filename) {
        channel->chunk = load_chunk(channel->filename);
    }
    return channel->chunk ? 1 : 0;*/
}

void sound_device_init_channels(int num_channels, char filenames[][CHANNEL_FILENAME_MAX])
{
    if (data.initialized) {
        if (num_channels > MAX_CHANNELS) {
            num_channels = MAX_CHANNELS;
        }
        Mix_AllocateChannels(num_channels);
        log_info("Loading audio files", 0, 0);
        for (int i = 0; i < num_channels; i++) {
            data.channels[i].chunk = 0;
            data.channels[i].filename = filenames[i][0] ? filenames[i] : 0;
        }
        log_info("Done loading audio files", 0, 0);
    }
}

int sound_device_is_channel_playing(int channel)
{
    return data.channels[channel].chunk && Mix_Playing(channel);
}

void sound_device_set_music_volume(int volume_pct)
{
    Mix_VolumeMusic(percentage_to_volume(volume_pct));
}

void sound_device_set_channel_volume(int channel, int volume_pct)
{
    if (data.channels[channel].chunk) {
        Mix_VolumeChunk(data.channels[channel].chunk, percentage_to_volume(volume_pct));
    }
}

#ifdef __vita__
static void load_music_for_vita(const char *filename)
{
    if (vita_music_data.buffer) {
        if (strcmp(filename, vita_music_data.filename) == 0) {
            return;
        }
        free(vita_music_data.buffer);
        vita_music_data.buffer = 0;
    }
    strncpy(vita_music_data.filename, filename, FILE_NAME_MAX - 1);
    SceUID fd = sceIoOpen(vita_prepend_path(filename), SCE_O_RDONLY, 0777);
    if (fd < 0) {
        return;
    }
    vita_music_data.size = sceIoLseek(fd, 0, SCE_SEEK_END);
    sceIoLseek(fd, 0, SCE_SEEK_SET);
    vita_music_data.buffer = malloc(sizeof(char) * vita_music_data.size);
    sceIoRead(fd, vita_music_data.buffer, vita_music_data.size);
    sceIoClose(fd);
}
#endif

int sound_device_play_music(const char *filename, int volume_pct)
{
    if (data.initialized && config_get(CONFIG_GENERAL_ENABLE_AUDIO)) {
        sound_device_stop_music();
        if (!filename) {
            return 0;
        }
#ifdef __vita__
        load_music_for_vita(filename);
        if (!vita_music_data.buffer) {
            return 0;
        }
        SDL_RWops *sdl_music = SDL_RWFromMem(vita_music_data.buffer, vita_music_data.size);
        data.music = Mix_LoadMUSType_RW(sdl_music, file_has_extension(filename, "mp3") ? MUS_MP3 : MUS_WAV, SDL_TRUE);
#elif defined(__ANDROID__)
        FILE *fp = file_open(filename, "rb");
        if (!fp) {
            return 0;
        }
        SDL_RWops *sdl_fp = SDL_RWFromFP(fp, SDL_TRUE);
        data.music = Mix_LoadMUSType_RW(sdl_fp, file_has_extension(filename, "mp3") ? MUS_MP3 : MUS_WAV, SDL_TRUE);
#else
        data.music = Mix_LoadMUS(filename);
#endif
        if (!data.music) {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                "Error opening music file '%s'. Reason: %s", filename, Mix_GetError());
        } else {
            if (Mix_PlayMusic(data.music, -1) == -1) {
                data.music = 0;
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Error playing music file '%s'. Reason: %s", filename, Mix_GetError());
            } else {
                sound_device_set_music_volume(volume_pct);
            }
        }
        return data.music ? 1 : 0;
    }
    return 0;
}

void sound_device_play_file_on_channel(const char *filename, int channel, int volume_pct)
{
    if (data.initialized && config_get(CONFIG_GENERAL_ENABLE_AUDIO)) {
        sound_device_stop_channel(channel);
        data.channels[channel].chunk = load_chunk(filename);
        if (data.channels[channel].chunk) {
            sound_device_set_channel_volume(channel, volume_pct);
            Mix_PlayChannel(channel, data.channels[channel].chunk, 0);
        }
    }
}

void sound_device_play_channel(int channel, int volume_pct)
{
    if (data.initialized && config_get(CONFIG_GENERAL_ENABLE_AUDIO)) {
        sound_channel *ch = &data.channels[channel];
        if (load_channel(ch)) {
            sound_device_set_channel_volume(channel, volume_pct);
            Mix_PlayChannel(channel, ch->chunk, 0);
        }
    }
}

void sound_device_play_channel_panned(int channel, int volume_pct, int left_pct, int right_pct)
{
    if (data.initialized && config_get(CONFIG_GENERAL_ENABLE_AUDIO)) {
        sound_channel *ch = &data.channels[channel];
        if (load_channel(ch)) {
            Mix_SetPanning(channel, left_pct * 255 / 100, right_pct * 255 / 100);
            sound_device_set_channel_volume(channel, volume_pct);
            Mix_PlayChannel(channel, ch->chunk, 0);
        }
    }
}

void sound_device_stop_music(void)
{
    if (data.initialized) {
        if (data.music) {
            Mix_HaltMusic();
            Mix_FreeMusic(data.music);
            data.music = 0;
        }
    }
}

void sound_device_stop_channel(int channel)
{
    if (data.initialized) {
        sound_channel *ch = &data.channels[channel];
        if (ch->chunk) {
            Mix_HaltChannel(channel);
            Mix_FreeChunk(ch->chunk);
            ch->chunk = 0;
        }
    }
}