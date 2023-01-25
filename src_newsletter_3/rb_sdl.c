//
//  rb_sdl.cpp
//  3d wireframe game engine
//
//  SDL integration
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_sdl.h"
#include "rb_base.h"
#include "rb_draw.h"
#include "rb_input.h"
#include "rb_platform.h"
#include "rb_keys.h"

extern void engine_step(void);
extern void engine_after_render(void);
extern bool engine_handle_key(RBEvent event);

SDL_Window* _window = NULL;
SDL_Renderer* _renderer = NULL;
SDL_Texture* _texture = NULL;
SDL_Joystick* _gameController = NULL;
Uint32* _pixels = NULL;

int _screen_width = 320;
int _screen_height = 200;
int _buffer_width = 320;
int _buffer_height = 200;
bool _fullscreen = false;
Uint32 _time_per_frame = 16;

const int JOYSTICK_DEAD_ZONE = 8000;

bool _control_key = false;
bool _shift_key = false;
bool _alt_key = false;
bool _quit = false;

SDL_Renderer* sdl_get_renderer() {
    return _renderer;
}

void _sdl_create_buffer(void) {
    _pixels = malloc(_buffer_width * _buffer_height * sizeof(Uint32));
    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, _buffer_width, _buffer_height);
}

void sdl_clear_buffer(Uint32 color) {
    memset(_pixels, color, _buffer_width * _buffer_height * sizeof(Uint32));
}

void sdl_set_pixel(int x, int y, Uint32 color) {
    if (x >= _buffer_width || y >= _buffer_height) return;
    _pixels[y * _buffer_width + x] = color;    
}

Uint32 sdl_get_pixel(int x, int y) {
    if (x >= _buffer_width || y >= _buffer_height) return ARGB_WHITE;

    return _pixels[y * _buffer_width + x];    
}

void sdl_set_fullscreen() {
    SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);
    _fullscreen = true;
}

void sdl_toggle_fullscreen() {
    _fullscreen = !_fullscreen;
    SDL_SetWindowFullscreen(_window, _fullscreen);
}

void sdl_set_fps(int fps) {
    _time_per_frame = 1000 / fps;
}

int sdl_init(int width, int height, int buffer_width, int buffer_height) {
    rblog("sdl_init()");
    rbmsg_str1("home path is: ", platform_home_path());

    _screen_width = width;
    _screen_height = height;
    _buffer_width = buffer_width;
    _buffer_height = buffer_height;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) != 0) {
        rblog_str1("Error initializing SDL: ", SDL_GetError());
        return -1;
    }

    // Check for joysticks
    if( SDL_NumJoysticks() < 1 ) {
        rbmsg("warning: no joysticks connected!");
    }
    else {
        _gameController = SDL_JoystickOpen( 0 );
        
        if (_gameController == NULL) {
            rblog_str1("Warning: Unable to open game controller! SDL Error: ", SDL_GetError());
        }
    }

	if (width == 0 || height == 0) {
		SDL_DisplayMode dm;

		if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
     		rblog_str1("SDL_GetCurrentDisplayMode failed: ", SDL_GetError());
     		return 1;
		}

		_screen_width = dm.w;
		_screen_height = dm.h;

		rblog_num1("Automatically set screen width to ", _screen_width);
		rblog_num1("Automatically set screen height to ", _screen_height);
	}
	else {
		rblog_num1("Screen width is ", _screen_width);
		rblog_num1("Screen height is ", _screen_height);
	}

    _window = SDL_CreateWindow("PLAYGROUND", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screen_width, _screen_height, 0);
 
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    _renderer = SDL_CreateRenderer(_window, -1, render_flags);

    _sdl_create_buffer();

    return 1;
}

void _sdl_create_screenshot() {
	rbprint("Create screenshot");

    SDL_Surface* surface = SDL_CreateRGBSurface(0, _screen_width, _screen_height, 32, 0, 0, 0, 0);
    if (surface == NULL) {
        rblog("can't create surface for screenshot");
        return;        
    }

    char path[256];
    sprintf(path, "%s/screenshot.bmp", platform_home_path());
    SDL_RenderReadPixels(_renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
    SDL_SaveBMP(surface, path);
    rbmsg_str1("Screenshot saved to ", path);

    SDL_FreeSurface(surface);
}

void _sdl_handle_joystick_event(SDL_Event event) {
    if (event.jaxis.which == 0) {                        
        if (event.jaxis.axis == 0) {
            if (event.jaxis.value < -JOYSTICK_DEAD_ZONE) {
                input_set_control(CONTROL_LEFT, true);
            }
            else if (event.jaxis.value > JOYSTICK_DEAD_ZONE) {
                input_set_control(CONTROL_RIGHT, true);
            }
            else {
                input_set_control(CONTROL_LEFT, false);
                input_set_control(CONTROL_RIGHT, false);
            }
        }
        else if (event.jaxis.axis == 1) {
            if (event.jaxis.value < -JOYSTICK_DEAD_ZONE) {
                input_set_control(CONTROL_DOWN, true);
            }
            else if (event.jaxis.value > JOYSTICK_DEAD_ZONE) {
                input_set_control(CONTROL_UP, true);
            }
            else {
                input_set_control(CONTROL_DOWN, false);
                input_set_control(CONTROL_UP, false);
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
        
        case SDLK_p:
            if (_control_key) {
                _sdl_create_screenshot();
            }
            else {
                return false;
            }
            break;
        case SDLK_q:
            if (_control_key) {
                _quit = 1;
            }
            else {
                return false;
            }
            return true;
        case SDLK_f:
            if (_control_key) {
                sdl_toggle_fullscreen();
            }
            else {
                return false;
            }
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
 
        default:
            return false;
    }

    return true;
}

bool _sdl_route_key_event(SDL_Event event) {
    RBEvent rb_event;
    rb_event.type = event.type;
    rb_event.key_code = (RBKeyCode)event.key.keysym.sym;
    rb_event.scan_code = (RBScanCode)event.key.keysym.scancode;
    rb_event.control = _control_key;
    rb_event.alt = _alt_key;
    rb_event.shift = _shift_key;

    return engine_handle_key(rb_event);
}

int sdl_run() {
    rbmsg_str1("App folder is: ", platform_get_app_folder());
    rblog_num1("sdl_run(): FPS=", 1000/_time_per_frame);

    _quit = false;

    input_clear_controls();

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

            engine_step();

#if 0
			// Scaled
            float scale = (float)(_screen_width-20)/2/_buffer_width;

            SDL_Rect source = { 0, 0, _buffer_width, _buffer_height };
            SDL_Rect dest = { _screen_width/2, 20, _buffer_width*scale, _buffer_height*scale };
            
			SDL_UpdateTexture(_texture, NULL, _pixels, _buffer_width * sizeof(Uint32));
            SDL_RenderCopy(_renderer, _texture, &source, &dest);
#else
            SDL_UpdateTexture(_texture, NULL, _pixels, _buffer_width * sizeof(Uint32));
            SDL_RenderCopy(_renderer, _texture, NULL, NULL);
#endif

            engine_after_render();

            SDL_RenderPresent(_renderer);

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

    return 0;
}

int sdl_cleanup(void) {
    rblog("sdl_cleanup()");

    free(_pixels);
    
    SDL_DestroyTexture(_texture);

    SDL_JoystickClose(_gameController);
    _gameController = NULL;

    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
 
    return 0;
}

void platform_set_window_title(const char* title) {
    SDL_SetWindowTitle(_window, title);
}

void platform_copy_buffer(unsigned char* buffer, int width, int height) {
    memcpy(_pixels, buffer, width*height*sizeof(Uint32));
}
