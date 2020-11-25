#include "SDL.h"

#include "core/backtrace.h"
#include "core/config.h"
#include "core/encoding.h"
#include "core/file.h"
#include "core/lang.h"
#include "core/time.h"
#include "game/game.h"
#include "game/settings.h"
#include "game/system.h"
#include "graphics/screen.h"
#include "input/mouse.h"
#include "input/touch.h"
#include "platform/arguments.h"
#include "platform/file_manager.h"
#include "platform/keyboard_input.h"
#include "platform/platform.h"
#include "platform/prefs.h"
#include "platform/screen.h"
#include "platform/touch.h"

#include "tinyfiledialogs/tinyfiledialogs.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __SWITCH__
#include "platform/switch/switch.h"
#include "platform/switch/switch_input.h"
#endif

#ifdef __vita__
#include "platform/vita/vita.h"
#include "platform/vita/vita_input.h"
#endif

#include "platform/android/android.h"

#if defined(_WIN32)
#include <string.h>
#endif

#if defined(USE_TINYFILEDIALOGS) || defined(__ANDROID__)
#define SHOW_FOLDER_SELECT_DIALOG
#endif

#ifdef DRAW_FPS
#include "graphics/window.h"
#include "graphics/graphics.h"
#include "graphics/text.h"
#endif

#define INTPTR(d) (*(int*)(d))

enum {
    USER_EVENT_QUIT,
    USER_EVENT_RESIZE,
    USER_EVENT_FULLSCREEN,
    USER_EVENT_WINDOWED,
    USER_EVENT_CENTER_WINDOW,
};

static void handler(int sig)
{
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Oops, crashed with signal %d :(", sig);
    backtrace_print();
    exit(1);
}

#if defined(_WIN32) || defined(__vita__) || defined(__SWITCH__)
/* Log to separate file on windows, since we don't have a console there */
static FILE *log_file = 0;

static void write_log(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
    if (log_file) {
        if (priority == SDL_LOG_PRIORITY_ERROR) {
            fwrite("ERROR: ", sizeof(char), 7, log_file);
        } else {
            fwrite("INFO: ", sizeof(char), 6, log_file);
        }
        fwrite(message, sizeof(char), strlen(message), log_file);
        fwrite("\n", sizeof(char), 1, log_file);
        fflush(log_file);
    }
}

static void setup_logging(void)
{
    log_file = file_open("julius-log.txt", "wt");
    SDL_LogSetOutputFunction(write_log, NULL);
}

static void teardown_logging(void)
{
    if (log_file) {
        file_close(log_file);
    }
}

#else
static void setup_logging(void) {}
static void teardown_logging(void) {}
#endif

static void post_event(int code)
{
    SDL_Event event;
    event.user.type = SDL_USEREVENT;
    event.user.code = code;
    SDL_PushEvent(&event);
}

void system_exit(void)
{
    post_event(USER_EVENT_QUIT);
}

void system_resize(int width, int height)
{
    static int s_width;
    static int s_height;

    s_width = width;
    s_height = height;
    SDL_Event event;
    event.user.type = SDL_USEREVENT;
    event.user.code = USER_EVENT_RESIZE;
    event.user.data1 = &s_width;
    event.user.data2 = &s_height;
    SDL_PushEvent(&event);
}

void system_center(void)
{
    post_event(USER_EVENT_CENTER_WINDOW);
}

void system_set_fullscreen(int fullscreen)
{
    post_event(fullscreen ? USER_EVENT_FULLSCREEN : USER_EVENT_WINDOWED);
}

#ifdef _WIN32
#define PLATFORM_ENABLE_PER_FRAME_CALLBACK
static void platform_per_frame_callback(void)
{
    platform_screen_recreate_texture();
}
#endif

#ifdef DRAW_FPS
static struct {
    int frame_count;
    int last_fps;
    Uint32 last_update_time;
} fps = {0, 0, 0};

static void run_and_draw(void)
{
    time_millis time_before_run = SDL_GetTicks();
    time_set_millis(time_before_run);

    game_run();
    Uint32 time_between_run_and_draw = SDL_GetTicks();
    game_draw();
    Uint32 time_after_draw = SDL_GetTicks();

    fps.frame_count++;
    if (time_after_draw - fps.last_update_time > 1000) {
        fps.last_fps = fps.frame_count;
        fps.last_update_time = time_after_draw;
        fps.frame_count = 0;
    }
    if (window_is(WINDOW_CITY) || window_is(WINDOW_CITY_MILITARY) || window_is(WINDOW_SLIDING_SIDEBAR)) {
        int y_offset = 24;
        int y_offset_text = y_offset + 5;
        graphics_fill_rect(0, y_offset, 100, 20, COLOR_WHITE);
        text_draw_number_colored(fps.last_fps,
            'f', "", 5, y_offset_text, FONT_NORMAL_PLAIN, COLOR_FONT_RED);
        text_draw_number_colored(time_between_run_and_draw - time_before_run,
            'g', "", 40, y_offset_text, FONT_NORMAL_PLAIN, COLOR_FONT_RED);
        text_draw_number_colored(time_after_draw - time_between_run_and_draw,
            'd', "", 70, y_offset_text, FONT_NORMAL_PLAIN, COLOR_FONT_RED);
    }

    platform_screen_render();
}
#else
static void run_and_draw(void)
{
    time_set_millis(SDL_GetTicks());

    game_run();
    game_draw();

    platform_screen_render();
}
#endif

static void handle_mouse_button(SDL_MouseButtonEvent *event, int is_down)
{
    if (!SDL_GetRelativeMouseMode()) {
        mouse_set_position(event->x, event->y);
    }
    if (event->button == SDL_BUTTON_LEFT) {
        mouse_set_left_down(is_down);
    } else if (event->button == SDL_BUTTON_RIGHT) {
        mouse_set_right_down(is_down);
    }
}

static void handle_window_event(SDL_WindowEvent *event, int *window_active)
{
    switch (event->event) {
        case SDL_WINDOWEVENT_ENTER:
            mouse_set_inside_window(1);
            break;
        case SDL_WINDOWEVENT_LEAVE:
            mouse_set_inside_window(0);
            break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            SDL_Log("Window resized to %d x %d", (int) event->data1, (int) event->data2);
            platform_screen_resize(event->data1, event->data2);
            break;
        case SDL_WINDOWEVENT_RESIZED:
            SDL_Log("System resize to %d x %d", (int) event->data1, (int) event->data2);
            break;
        case SDL_WINDOWEVENT_MOVED:
            SDL_Log("Window move to coordinates x: %d y: %d\n", (int) event->data1, (int) event->data2);
            platform_screen_move(event->data1, event->data2);
            break;

        case SDL_WINDOWEVENT_SHOWN:
            SDL_Log("Window %d shown", (unsigned int) event->windowID);
            *window_active = 1;
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            SDL_Log("Window %d hidden", (unsigned int) event->windowID);
            *window_active = 0;
            break;
    }
}

static void handle_event(SDL_Event *event, int *active, int *quit)
{
    switch (event->type) {
        case SDL_WINDOWEVENT:
            handle_window_event(&event->window, active);
            break;
        case SDL_KEYDOWN:
            platform_handle_key_down(&event->key);
            break;
        case SDL_KEYUP:
            platform_handle_key_up(&event->key);
            break;
        case SDL_TEXTINPUT:
            platform_handle_text(&event->text);
            break;
        case SDL_MOUSEMOTION:
            if (event->motion.which != SDL_TOUCH_MOUSEID && !SDL_GetRelativeMouseMode()) {
                mouse_set_position(event->motion.x, event->motion.y);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event->button.which != SDL_TOUCH_MOUSEID) {
                handle_mouse_button(&event->button, 1);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event->button.which != SDL_TOUCH_MOUSEID) {
                handle_mouse_button(&event->button, 0);
            }
            break;
        case SDL_MOUSEWHEEL:
            if (event->wheel.which != SDL_TOUCH_MOUSEID) {
                mouse_set_scroll(event->wheel.y > 0 ? SCROLL_UP : event->wheel.y < 0 ? SCROLL_DOWN : SCROLL_NONE);
            }
            break;

        case SDL_FINGERDOWN:
            platform_touch_start(&event->tfinger);
            break;
        case SDL_FINGERMOTION:
            platform_touch_move(&event->tfinger);
            break;
        case SDL_FINGERUP:
            platform_touch_end(&event->tfinger);
            break;

        case SDL_QUIT:
            *quit = 1;
            break;

        case SDL_USEREVENT:
            if (event->user.code == USER_EVENT_QUIT) {
                *quit = 1;
            } else if (event->user.code == USER_EVENT_RESIZE) {
                platform_screen_set_window_size(INTPTR(event->user.data1), INTPTR(event->user.data2));
            } else if (event->user.code == USER_EVENT_FULLSCREEN) {
                platform_screen_set_fullscreen();
            } else if (event->user.code == USER_EVENT_WINDOWED) {
                platform_screen_set_windowed();
            } else if (event->user.code == USER_EVENT_CENTER_WINDOW) {
                platform_screen_center_window();
            }
            break;

        default:
            break;
    }
}

static void main_loop(void)
{
    mouse_set_inside_window(1);

    run_and_draw();
    int active = 1;
    int quit = 0;
    while (!quit) {
        SDL_Event event;
#ifdef PLATFORM_ENABLE_PER_FRAME_CALLBACK
        platform_per_frame_callback();
#endif
        /* Process event queue */
#ifdef __vita__
        while (vita_poll_event(&event)) {
#elif defined(__SWITCH__)
        while (switch_poll_event(&event)) {
#else
        while (SDL_PollEvent(&event)) {
#endif
            handle_event(&event, &active, &quit);
        }
        if (!quit) {
            if (active) {
                run_and_draw();
            } else {
                SDL_WaitEvent(NULL);
            }
        }
    }
}

static int init_sdl(void)
{
    SDL_Log("Initializing SDL");
    Uint32 SDL_flags = SDL_INIT_AUDIO;

    // on Vita, need video init only to enable physical kbd/mouse and touch events
    SDL_flags |= SDL_INIT_VIDEO;

#if defined(__vita__) || defined(__SWITCH__)
    SDL_flags |= SDL_INIT_JOYSTICK;
#endif

    if (SDL_Init(SDL_flags) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL: %s", SDL_GetError());
        return 0;
    }
#if SDL_VERSION_ATLEAST(2, 0, 10)
    SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "0");
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
#elif SDL_VERSION_ATLEAST(2, 0, 4)
    SDL_SetHint(SDL_HINT_ANDROID_SEPARATE_MOUSE_AND_TOUCH, "1");
#endif
#ifdef __ANDROID__
    SDL_SetHint(SDL_HINT_ANDROID_TRAP_BACK_BUTTON, "1");
#endif
    SDL_Log("SDL initialized");
    return 1;
}

#ifdef SHOW_FOLDER_SELECT_DIALOG
static const char *ask_for_data_dir(int again)
{
#ifdef __ANDROID__
    if (again) {
        const SDL_MessageBoxButtonData buttons[] = {
           { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "OK" },
           { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Cancel" }
        };
        const SDL_MessageBoxData messageboxdata = {
            SDL_MESSAGEBOX_WARNING, NULL, "Wrong folder selected",
            "The selected folder is not a proper Caesar 3 folder.\n\n"
            "Please select a path directly from either the internal storage "
            "or the SD card, otherwise the path may not be recognised.\n\n"
            "Press OK to select another folder or Cancel to exit.",
            SDL_arraysize(buttons), buttons, NULL
        };
        int result;
        SDL_ShowMessageBox(&messageboxdata, &result);
        if (!result) {
            return NULL;
        }
    }
    return android_show_c3_path_dialog(again);
#else
    if (again) {
        int result = tinyfd_messageBox("Wrong folder selected",
            "Julius requires the original files from Caesar 3 to run.\n\n"
            "The selected folder is not a proper Caesar 3 folder.\n\n"
            "Press OK to select another folder or Cancel to exit.",
            "okcancel", "warning", 1);
        if (!result) {
            return NULL;
        }
    }
    return tinyfd_selectFolderDialog("Please select your Caesar 3 folder", NULL);
#endif
}
#endif

static int pre_init(const char *custom_data_dir)
{
    if (custom_data_dir) {
        SDL_Log("Loading game from %s", custom_data_dir);
        if (!platform_file_manager_set_base_path(custom_data_dir)) {
            SDL_Log("%s: directory not found", custom_data_dir);
            return 0;
        }
        return game_pre_init();
    }

    SDL_Log("Loading game from working directory");
    if (game_pre_init()) {
        return 1;
    }

    #if SDL_VERSION_ATLEAST(2, 0, 1)
    if (platform_sdl_version_at_least(2, 0, 1)) {
        char *base_path = SDL_GetBasePath();
        if (base_path) {
            if (platform_file_manager_set_base_path(base_path)) {
                SDL_Log("Loading game from base path %s", base_path);
                if (game_pre_init()) {
                    SDL_free(base_path);
                    return 1;
                }
            }
            SDL_free(base_path);
        }
    }
    #endif

    #ifdef SHOW_FOLDER_SELECT_DIALOG
        const char *user_dir = pref_data_dir();
        if (user_dir) {
            SDL_Log("Loading game from user pref %s", user_dir);
            if (platform_file_manager_set_base_path(user_dir) && game_pre_init()) {
                return 1;
            }
        }

        user_dir = ask_for_data_dir(0);
        while (user_dir) {
            SDL_Log("Loading game from user-selected dir %s", user_dir);
            if (platform_file_manager_set_base_path(user_dir) && game_pre_init()) {
                pref_save_data_dir(user_dir);
#ifdef __ANDROID__
                android_toast_message("C3 files found. Path saved.");
#endif
                return 1;
            }
            user_dir = ask_for_data_dir(1);
        }
    #else
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
            "Julius requires the original files from Caesar 3 to run.",
            "Move the Julius executable to the directory containing an existing "
            "Caesar 3 installation, or run:\njulius path-to-c3-directory",
            NULL);
    #endif

    return 0;
}

static void setup(const julius_args *args)
{
    signal(SIGSEGV, handler);
    setup_logging();

    SDL_Log("Julius version %s", system_version());

    if (!init_sdl()) {
        SDL_Log("Exiting: SDL init failed");
        exit(-1);
    }

    if (!pre_init(args->data_directory)) {
        SDL_Log("Exiting: game pre-init failed");
        exit(1);
    }

#ifdef PLATFORM_ENABLE_INIT_CALLBACK
    platform_init_callback();
#endif

    if (args->force_windowed && setting_fullscreen()) {
        int w, h;
        setting_window(&w, &h);
        setting_set_display(0, w, h);
        SDL_Log("Forcing windowed mode with size %d x %d", w, h);
    }

    // handle arguments
    if (args->display_scale_percentage) {
        config_set(CONFIG_SCREEN_DISPLAY_SCALE, args->display_scale_percentage);
    }
    if (args->cursor_scale_percentage) {
        config_set(CONFIG_SCREEN_CURSOR_SCALE, args->cursor_scale_percentage);
    }

    char title[100];
    encoding_to_utf8(lang_get_string(9, 0), title, 100, 0);
    if (!platform_screen_create(title, config_get(CONFIG_SCREEN_DISPLAY_SCALE))) {
        SDL_Log("Exiting: SDL create window failed");
        exit(-2);
    }
    // this has to come after platform_screen_create, otherwise it fails on Nintendo Switch
    system_init_cursors(config_get(CONFIG_SCREEN_CURSOR_SCALE));

    if (system_is_fullscreen_only()) {
        system_move_mouse_cursor(screen_width() / 2, screen_height() / 2);
    }

    time_set_millis(SDL_GetTicks());

    if (!game_init()) {
        SDL_Log("Exiting: game init failed");
        exit(2);
    }
}

static void teardown(void)
{
    SDL_Log("Exiting game");
    game_exit();
    platform_screen_destroy();
    SDL_Quit();
    teardown_logging();
}

int main(int argc, char **argv)
{
    julius_args args;
    platform_parse_arguments(argc, argv, &args);

    setup(&args);

    main_loop();

    teardown();
    return 0;
}
