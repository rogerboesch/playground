//
//  rb_sdl.cpp
//
//  SDL integration
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_base.h"
#include "rb_log.h"
#include "rb_engine.hpp"
#include "rb_platform.h"

#include <string.h>
#include "SDL.h"

#if _WIN32
    extern "C" int win_get_document_path(char* path);
#endif
#ifdef __APPLE__
    extern "C" int mac_get_bundle_path(char *path, const char* filename, const char* extension);
#endif

// Call engine
extern "C" {
    const char* platform_resource_file_path(const char* filename, const char* extension);
    void game_set_control_state(int code, int state);
    void game_set_filled(int code);
}

extern "C" {
    int vexxon_start();
    int vexxon_frame();
    int vexxon_stop();
}

char s_asset_path[1024];

SDL_Window* _window = NULL;
SDL_Renderer* _renderer = NULL;
SDL_Texture* _texture = NULL;
SDL_Joystick* _gameController = NULL;

byte* _pixels = NULL;
int _screen_width = 512;
int _screen_height = 512;
int _buffer_width = 512;
int _buffer_height = 512;
bool _fullscreen = false;
Uint32 _time_per_frame = 16;
bool _draw_filled = false;

const int JOYSTICK_DEAD_ZONE = 8000;

bool _control_key = false;
bool _shift_key = false;
bool _alt_key = false;
bool _quit = false;

void _sdl_create_buffer() {
    _pixels = (byte*)malloc(_buffer_width * _buffer_height * 4);
    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, _buffer_width, _buffer_height);
}

void _sdl_clear_buffer(byte color) {
    if (_pixels == nullptr) return;
    memset(_pixels, color, _buffer_width * _buffer_height * 4);
}

void _sdl_set_pixel(int x, int y, byte color, int brightness) {
    if (_pixels == nullptr) return;

    if (x > _screen_width || x <= 0) return;
    if (y > _screen_height || y <= 0) return;

    static int left = (_buffer_width - _screen_width) / 2;
    static int top = (_buffer_height - _screen_height) / 2;

    x += left;
    y = _screen_height - y;

    VecRGB rgb = GetPaletteColor(color, brightness);

    int height = _buffer_height;
 
    _pixels[(y * height + x) * 4] = rgb.b;
    _pixels[(y * height + x) * 4 + 1] = rgb.g;
    _pixels[(y * height + x) * 4 + 2] = rgb.r;
    _pixels[(y * height + x) * 4 + 3] = 255;
}

void _sdl_draw_line(int x1, int y1, int x2, int y2, byte color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    
    // calculate steps required for generating pixels
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    
    // calculate increment in x & y for each steps
    float xInc = dx / (float) steps;
    float yInc = dy / (float) steps;
    
    float x = x1;
    float y = y1;
    for (int i = 0; i <= steps; i++) {
        _sdl_set_pixel(x, y, color, BRIGHTNESS_OFF);
        
        x += xInc;
        y += yInc;
    }
}

void _sdl_toggle_fullscreen() {
    _fullscreen = !_fullscreen;
    SDL_SetWindowFullscreen(_window, _fullscreen);
}

void _sdl_set_fps(int fps) {
    _time_per_frame = 1000 / fps;
}

int _sdl_init(int width, int height, int buffer_width, int buffer_height) {
    RBLOG("sdl_init()");

    _screen_width = width;
    _screen_height = height;
    _buffer_width = buffer_width;
    _buffer_height = buffer_height;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) != 0) {
        RBLOG_STR1("Error initializing SDL: ", SDL_GetError());
        return -1;
    }

    // Check for joysticks
    if( SDL_NumJoysticks() < 1 ) {
        RBLOG("warning: no joysticks connected!");
    }
    else {
        _gameController = SDL_JoystickOpen( 0 );
        
        if (_gameController == NULL) {
            RBLOG_STR1("WARNING: Unable to open game controller! SDL Error: ", SDL_GetError());
        }
    }

	if (width == 0 || height == 0) {
		SDL_DisplayMode dm;

		if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
            RBLOG_STR1("SDL_GetCurrentDisplayMode failed: ", SDL_GetError());
     		return 1;
		}

		_screen_width = dm.w;
		_screen_height = dm.h;

        RBLOG_NUM1("Automatically set screen width ", _screen_width);
        RBLOG_NUM1("Automatically set screen height", _screen_height);
    }
	else {
        RBLOG_NUM1("Set screen width ", _screen_width);
        RBLOG_NUM1("Set screen height", _screen_height);
	}

    _window = SDL_CreateWindow("PLAYGROUND", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _buffer_width, _buffer_height, 0);
 
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    _renderer = SDL_CreateRenderer(_window, -1, render_flags);

    _sdl_create_buffer();
    
    return 1;
}

void _sdl_handle_joystick_event(SDL_Event event) {
    if (event.jaxis.which == 0) {                        
        if (event.jaxis.axis == 0) {
            if (event.jaxis.value < -JOYSTICK_DEAD_ZONE) {
//                game_set_control_state()
//                input_set_control(CONTROL_LEFT, true);
            }
            else if (event.jaxis.value > JOYSTICK_DEAD_ZONE) {
//                input_set_control(CONTROL_RIGHT, true);
            }
            else {
//                input_set_control(CONTROL_LEFT, false);
//                input_set_control(CONTROL_RIGHT, false);
            }
        }
        else if (event.jaxis.axis == 1) {
            if (event.jaxis.value < -JOYSTICK_DEAD_ZONE) {
//                input_set_control(CONTROL_DOWN, true);
            }
            else if (event.jaxis.value > JOYSTICK_DEAD_ZONE) {
//                input_set_control(CONTROL_UP, true);
            }
            else {
//                input_set_control(CONTROL_DOWN, false);
//                input_set_control(CONTROL_UP, false);
            }
        }
    }
}

bool _sdl_handle_keydown_event(SDL_Event event) {
    switch (event.key.keysym.sym) {
        case SDLK_RCTRL:
        case SDLK_LCTRL:
            _control_key = true;
            break;

        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
            _shift_key = true;
            break;
        
        case SDLK_LALT:
        case SDLK_RALT:
            _alt_key = true;
            break;
        
        case SDLK_q:
            if (_control_key) {
                _quit = 1;
            }
            else {
                return false;
            }
            break;
        case SDLK_f:
            if (_control_key) {
                _sdl_toggle_fullscreen();
            }
            else {
                _draw_filled = !_draw_filled;
                game_set_filled(_draw_filled);
            }
            break;

        case SDLK_LEFT:
            game_set_control_state(CONTROL1_JOY_LEFT, true);
            break;
        case SDLK_RIGHT:
            game_set_control_state(CONTROL1_JOY_RIGHT, true);
            break;
        case SDLK_UP:
            game_set_control_state(CONTROL1_JOY_UP, true);
            break;
        case SDLK_DOWN:
            game_set_control_state(CONTROL1_JOY_DOWN, true);
            break;

        case SDLK_1:
            game_set_control_state(CONTROL1_BTN1, true);
            break;
        case SDLK_2:
            game_set_control_state(CONTROL1_BTN2, true);
            break;
        case SDLK_3:
            game_set_control_state(CONTROL1_BTN3, true);
            break;
        case SDLK_4:
            game_set_control_state(CONTROL1_BTN4, true);
            break;

        default:
            return false;
    }

    return true;
}

bool _sdl_handle_keyup_event(SDL_Event event) {
    switch (event.key.keysym.sym) {
        case SDLK_RCTRL:
        case SDLK_LCTRL:
            _control_key = false;
            break;

        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
            _shift_key = false;
            break;
        
        case SDLK_LALT:
        case SDLK_RALT:
            _alt_key = false;
            break;

        case SDLK_LEFT:
            game_set_control_state(CONTROL1_JOY_LEFT, false);
            break;
        case SDLK_RIGHT:
            game_set_control_state(CONTROL1_JOY_RIGHT, false);
            break;
        case SDLK_UP:
            game_set_control_state(CONTROL1_JOY_UP, false);
            break;
        case SDLK_DOWN:
            game_set_control_state(CONTROL1_JOY_DOWN, false);
            break;

        case SDLK_1:
            game_set_control_state(CONTROL1_BTN1, false);
            break;
        case SDLK_2:
            game_set_control_state(CONTROL1_BTN2, false);
            break;
        case SDLK_3:
            game_set_control_state(CONTROL1_BTN3, false);
            break;
        case SDLK_4:
            game_set_control_state(CONTROL1_BTN4, false);
            break;

        default:
            return false;
    }

    return true;
}

bool _sdl_route_key_event(SDL_Event event) {
    UNUSED_VAR(event);
    return false;
    //return engine_handle_key(rb_event);
}

int _sdl_run() {
    _quit = false;

    vexxon_start();

    Uint32 next_frame = SDL_GetTicks() + _time_per_frame;

    while (!_quit) {
        SDL_Event event;
 
        while (SDL_PollEvent(&event)) {
            // handle_sdl_event(event);

            switch (event.type) {
                case SDL_QUIT:
                    _quit = 1;
                    break;
                case SDL_JOYAXISMOTION:
                    _sdl_handle_joystick_event(event);
                    break;

                case SDL_KEYDOWN:
                    if (!_sdl_handle_keydown_event(event)) {
                        _sdl_route_key_event(event);
                    }
                    break;
                case SDL_KEYUP:
                    if (!_sdl_handle_keyup_event(event)) {
                        _sdl_route_key_event(event);
                    }
                    break;
                case SDL_TEXTEDITING:
                    if (!_sdl_handle_keydown_event(event)) {
                        _sdl_route_key_event(event);
                    }
                    break;
            }
        }

        if (!_quit) {
            SDL_RenderClear(_renderer);
            
            vexxon_frame();

            SDL_UpdateTexture(_texture, NULL, _pixels, _buffer_width * sizeof(Uint32));
            SDL_RenderCopy(_renderer, _texture, NULL, NULL);
            SDL_RenderPresent(_renderer);

            _time_per_frame = 100;
            Uint32 now = SDL_GetTicks();
            if (now < next_frame) {
                SDL_Delay(next_frame - now);
            }
            else {
                next_frame = now;
            }

            next_frame += _time_per_frame;
        }
    }

    vexxon_stop();

    return 0;
}

int _sdl_cleanup(void) {
    RBLOG("sdl_cleanup()");

    free(_pixels);
    
    SDL_DestroyTexture(_texture);

    SDL_JoystickClose(_gameController);
    _gameController = NULL;

    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
 
    return 0;
}

void _store_asset_path() {
#if _WIN32
    char path[1024];
    if (win_get_document_path(path)) {
        sprintf(s_asset_path, "%s\\RBProjects\\", path);
    }
    else {
        sprintf(s_asset_path, "");
    }
#elif __APPLE__
    char path[1024];
    if (mac_get_bundle_path(path, "level1", "txt")) {
        // A bit hacky, needs some error checking
        char temp[1024];
        int len = strlen(path)-10;
        strncpy(temp, path, len);
        temp[len] = '\0';
        sprintf(s_asset_path, "%s", temp);
    }
    else {
        sprintf(s_asset_path, "");
    }
#else
    const char* home_dir = getenv("HOME");
    sprintf(s_asset_path, "%s/Documents/RBProjects/", home_dir);
#endif

    RBLOG_STR1("Asset path", s_asset_path);
}

int main(int argc, char *argv[]) {
    UNUSED_VAR(argc);
    UNUSED_VAR(argv);

    _store_asset_path();

    _sdl_set_fps(30);
    _sdl_run();
    _sdl_cleanup();

    return 0;
}

// Callback support for engine -------------------------------------------------------------------------------

extern "C" {    
    char s_path[1024];

    void platform_set_window_title(const char* title) {
        SDL_SetWindowTitle(_window, title);
    }

    void platform_clear_buffer(byte color) {
        _sdl_clear_buffer(color);
    }

    void platform_set_pixel(int x, int y, byte color, int brightness) {
        _sdl_set_pixel(x, y, color, brightness);
    }

    void platform_draw_line(int x1, int y1, int x2, int y2, byte color, int invert) {
        if (invert == INVERT_ON) {
            y1 = _screen_height - y1;
            y2 = _screen_height - y2;
        }

        _sdl_draw_line(x1, y1, x2, y2, color);

#if 0
        _sdl_draw_line(1, 1, _buffer_width, 1, 3, 0);
        _sdl_draw_line(_buffer_width, 1, buffer_width, buffer_height, 3, 0);
        _sdl_draw_line(_buffer_width, buffer_height, 1, buffer_height, 3, 0);
        _sdl_draw_line(1, _buffer_width, 1, 1, 3, 0);
#endif
#if 0
        for (int i = 0; i <= 13; i++) {
            _sdl_draw_line(i*10, 1, i*10, 100, i, 0);
        }
#endif
    }

    byte platform_get_input(byte code) {
        UNUSED_VAR(code);
        return 0;
    }

    byte platform_get_control_state(byte code) {
        UNUSED_VAR(code);
        return 0;
    }

    const char* platform_resource_file_path(const char* filename, const char* extension) {
        UNUSED_VAR(filename);
        UNUSED_VAR(extension);

        sprintf(s_path, "%s%s.%s", s_asset_path, filename, extension);

        return s_path;
    }

    double platform_get_ms(void) {
        return SDL_GetTicks();
    }

    // Events
    void platform_on_init(const char* name, int width, int height) {
        platform_set_window_title(name);
        _sdl_init(width, height, 512, 512);
    }

    void platform_on_frame() {
        _sdl_clear_buffer(0);
    }
}
