#include "video.h"

#include "core/dir.h"
#include "core/time.h"
#include "game/settings.h"
#include "graphics/graphics.h"
#include "sound/device.h"
#include "sound/music.h"
#include "sound/speech.h"

#include "libsmacker/smacker.h"

static struct {
    int is_playing;
    int is_ended;

    struct {
        smk s;
        int width;
        int height;
        int micros_per_frame;
        time_millis start_render_millis;
        int current_frame;
        int total_frames;
    } video;
    struct {
        smk s;
        int bitdepth;
        int channels;
        int rate;
        int first_frame;
    } audio;
} data;

static void closeSmk(smk *s)
{
    smk_close(*s);
    *s = 0;
}

static void closeAll()
{
    if (data.video.s) {
        closeSmk(&data.video.s);
    }
    if (data.audio.s) {
        closeSmk(&data.audio.s);
    }
}

static const unsigned char *nextAudioFrame(int *outLen)
{
    if (!data.audio.s) {
        return 0;
    }
    if (data.audio.first_frame) {
        data.audio.first_frame = 0;
        if (smk_first(data.audio.s) < 0) {
            closeSmk(&data.audio.s);
            return 0;
        }
    } else if (!smk_next(data.audio.s)) {
        closeSmk(&data.audio.s);
        return 0;
    }
    int audioLen = smk_get_audio_size(data.audio.s, 0);
    if (audioLen > 0) {
        *outLen = audioLen;
        return smk_get_audio(data.audio.s, 0);
    }
    return 0;
}

static int loadSmkVideo(const char *filename)
{
    data.video.s = smk_open_file(filename, SMK_MODE_DISK);
    if (!data.video.s) {
        return 0;
    }
    
    unsigned long width, height, frames;
    double microsPerFrame;
    smk_info_all(data.video.s, 0, &frames, &microsPerFrame);
    smk_info_video(data.video.s, &width, &height, 0);
    
    data.video.width = width;
    data.video.height = height;
    data.video.current_frame = 0;
    data.video.total_frames = frames;
    data.video.micros_per_frame = (int) (microsPerFrame);

    smk_enable_all(data.video.s,SMK_VIDEO_TRACK);
    if (smk_first(data.video.s) < 0) {
        closeSmk(&data.video.s);
        return 0;
    }
    return 1;
}

static int loadSmkAudio(const char *filename)
{
    if (!setting_sound(SOUND_EFFECTS)->enabled) {
        // no sound when sound effects are disabled
        return 1;
    }
    data.audio.s = smk_open_file(filename, SMK_MODE_DISK);
    if (!data.audio.s) {
        return 0;
    }
    
    unsigned char tracks, channels[7], bitdepths[7];
    unsigned long bitrates[7];
    smk_info_audio(data.audio.s, &tracks, channels, bitdepths, bitrates);
    
    if (tracks != 1) {
        // Video has alternate audio tracks, not supported
        closeSmk(&data.audio.s);
        return 0;
    }
    smk_enable_all(data.audio.s, tracks);
    if (smk_first(data.audio.s) < 0) {
        closeSmk(&data.audio.s);
        return 0;
    }
    data.audio.bitdepth = bitdepths[0];
    data.audio.channels = channels[0];
    data.audio.rate = bitrates[0];
    data.audio.first_frame = 1;
    return 1;
}

static int loadSmk(const char *filename)
{
    const char *path = dir_get_case_corrected_file(filename);
    if (!path) {
        return 0;
    }
    if (loadSmkVideo(path) && loadSmkAudio(path)) {
        return 1;
    } else {
        closeAll();
        return 0;
    }
}

static void endVideo()
{
    sound_device_use_default_music_player();
    sound_music_reset();
    sound_music_update();
}

int video_start(const char *filename)
{
    data.is_playing = 0;
    data.is_ended = 0;
    
    if (loadSmk(filename)) {
        sound_music_stop();
        sound_speech_stop();
        data.is_playing = 1;
        return 1;
    } else {
        return 0;
    }
}

void video_init()
{
    data.video.start_render_millis = time_get_millis();
    sound_device_use_custom_music_player(data.audio.bitdepth, data.audio.channels, data.audio.rate, nextAudioFrame);
}

int video_is_finished()
{
    return data.is_ended;
}

void video_stop()
{
    if (data.is_playing) {
        if (data.video.s) {
            closeAll();
        }
        data.is_playing = 0;
        if (!data.is_ended) {
            endVideo();
        }
    }
}

void video_shutdown()
{
    if (data.is_playing) {
        if (data.video.s) {
            closeAll();
        }
        data.is_playing = 0;
    }
}

void video_draw(int x_offset, int y_offset)
{
    if (!data.video.s) {
        return;
    }
    time_millis nowMillis = time_get_millis();
    
    int frameNo = (nowMillis - data.video.start_render_millis) * 1000 / data.video.micros_per_frame;
    if (frameNo > data.video.current_frame) {
        if (smk_next(data.video.s) == SMK_DONE) {
            closeSmk(&data.video.s);
            data.is_ended = 1;
            data.is_playing = 0;
            endVideo();
            return;
        }
        data.video.current_frame++;
    }
    
    const unsigned char *frame = smk_get_video(data.video.s);
    const unsigned char *pal = smk_get_palette(data.video.s);
    if (frame && pal) {
        for (int y = 0; y < data.video.height; y++) {
            for (int x = 0; x < data.video.width; x++) {
                color_t color = 0xFF000000 |
                    (pal[frame[(y * data.video.width) + x] * 3] << 16) |
                    (pal[frame[(y * data.video.width) + x] * 3 + 1] << 8) |
                    (pal[frame[(y * data.video.width) + x] * 3 + 2]);
                graphics_draw_pixel(x_offset + x, y_offset + y, color);
            }
        }
    }
}
