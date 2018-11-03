#include "sound/device.h"
#include "SDL.h"
#include "SDL_mixer.h"

#include <stdlib.h>
#include <string.h>

#define AUDIO_RATE 22050
#define AUDIO_FORMAT AUDIO_S16MSB
#define AUDIO_CHANNELS 2
#define AUDIO_BUFFERS 4096

#define MAX_CHANNELS 150

static int initialized = 0;
static Mix_Music *music = 0;
static Mix_Chunk *channels[MAX_CHANNELS];
static struct {
    SDL_AudioCVT cvt;
    const unsigned char *(*callback)(int *);
    unsigned char *data;
    int cur;
    int len;
} custom_music;


static int percentage_to_volume(int percentage)
{
    return percentage * 128 / 100;
}

void sound_device_open()
{
    if (0 == Mix_OpenAudio(AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNELS, AUDIO_BUFFERS)) {
        initialized = 1;
        for (int i = 0; i < MAX_CHANNELS; i++) {
            channels[i] = 0;
        }
    } else {
        SDL_Log("SOUND: not initialized");
    }
}

void sound_device_close()
{
    if (initialized) {
        for (int i = 0; i < MAX_CHANNELS; i++) {
            sound_device_stop_channel(i);
        }
        Mix_CloseAudio();
        initialized = 0;
    }
}

void sound_device_init_channels(int num_channels, const char filenames[][CHANNEL_FILENAME_MAX])
{
    if (initialized) {
        if (num_channels > MAX_CHANNELS) {
            num_channels = MAX_CHANNELS;
        }
        Mix_AllocateChannels(num_channels);
        for (int i = 0; i < num_channels; i++) {
            if (filenames[i][0]) {
                channels[i] = Mix_LoadWAV(filenames[i]);
            }
        }
    }
}

int sound_device_has_channel(int channel)
{
    return channels[channel] ? 1 : 0;
}

int sound_device_is_channel_playing(int channel)
{
    return Mix_Playing(channel);
}

void sound_device_set_music_volume(int volume_percentage)
{
    Mix_VolumeMusic(percentage_to_volume(volume_percentage));
}

void sound_device_set_channel_volume(int channel, int volume_percentage)
{
    if (channels[channel]) {
        Mix_VolumeChunk(channels[channel], percentage_to_volume(volume_percentage));
    }
}

void sound_device_set_channel_panning(int channel, int left_pct, int right_pct)
{
    if (channels[channel]) {
        Mix_SetPanning(channel, left_pct * 255 / 100, right_pct * 255 / 100);
    }
}

void sound_device_play_music(const char *filename)
{
    if (initialized) {
        sound_device_stop_music();
        music = Mix_LoadMUS(filename);
        if (music) {
            Mix_PlayMusic(music, -1);
        }
    }
}

void sound_device_play_file_on_channel(const char *filename, int channel)
{
    if (initialized) {
        if (channels[channel]) {
            sound_device_stop_channel(channel);
        }
        channels[channel] = Mix_LoadWAV(filename);
        if (channels[channel]) {
            Mix_PlayChannel(channel, channels[channel], 0);
        }
    }
}

void sound_device_play_channel(int channel)
{
    if (initialized) {
        if (channels[channel]) {
            Mix_PlayChannel(channel, channels[channel], 0);
        }
    }
}

void sound_device_stop_music()
{
    if (initialized) {
        if (music) {
            Mix_HaltMusic();
            Mix_FreeMusic(music);
            music = 0;
        }
    }
}

void sound_device_stop_channel(int channel)
{
    if (initialized) {
        if (channels[channel]) {
            Mix_HaltChannel(channel);
            Mix_FreeChunk(channels[channel]);
            channels[channel] = 0;
        }
    }
}


static int next_audio_frame()
{
    if (custom_music.data) {
        free(custom_music.data);
        custom_music.data = 0;
    }
    
    int audio_len;
    const unsigned char *data = custom_music.callback(&audio_len);
    if (!data || audio_len <= 0) {
        return 0;
    }
    
    if (audio_len > 0) {
        // convert audio to SDL format
        custom_music.cvt.buf = (Uint8*) malloc(audio_len * custom_music.cvt.len_mult);
        custom_music.cvt.len = audio_len;
        memcpy(custom_music.cvt.buf, data, audio_len);
        SDL_ConvertAudio(&custom_music.cvt);
        custom_music.cur = 0;
        custom_music.len = custom_music.cvt.len_cvt;
        custom_music.data = custom_music.cvt.buf;
        custom_music.cvt.buf = 0;
        custom_music.cvt.len = 0;
    }
    return audio_len;
}

static int copy_audio_from_buffer(Uint8 *stream, int len)
{
    if (!custom_music.data || custom_music.cur >= custom_music.len) {
        return 0;
    }
    // push existing bytes
    int to_write = custom_music.len - custom_music.cur;
    if (to_write > len) {
        to_write = len;
    }
    memcpy(stream, &custom_music.data[custom_music.cur], to_write);
    custom_music.cur += to_write;
    return to_write;
}

static void custom_music_callback(void *dummy, Uint8 *stream, int len)
{
    int can_continue;
    do {
        int copied = copy_audio_from_buffer(stream, len);
        if (copied) {
            len -= copied;
            stream += copied;
        }
        if (len == 0) {
            can_continue = 0;
        } else {
            can_continue = next_audio_frame();
        }
    } while (can_continue);
    if (len) {
        // end of stream, write silence
        memset(stream, 0, len);
    }
}

void sound_device_use_custom_music_player(int bitdepth, int channels, int rate, const unsigned char *(*callback)(int *out_len))
{
    SDL_BuildAudioCVT(&custom_music.cvt, bitdepth, channels, rate, AUDIO_FORMAT, AUDIO_CHANNELS, AUDIO_RATE);
    custom_music.callback = callback;
    Mix_HookMusic(custom_music_callback, 0);
}

void sound_device_use_default_music_player()
{
    if (custom_music.data) {
        free(custom_music.data);
        custom_music.data = 0;
        custom_music.len = 0;
        custom_music.cur = 0;
    }
    Mix_HookMusic(0, 0);
}
