//
//  rb_sdl.h
//  3d wireframe game engine
//
//  SDL integration
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#ifndef SDL_INTERFACE_H
#define SDL_INTERFACE_H

#include "SDL.h"

#ifdef __cplusplus
    extern "C" {
#endif

void sdl_set_fullscreen();
void sdl_clear_buffer(Uint32 color);
void sdl_set_pixel(int x, int y, Uint32 color);
Uint32 sdl_get_pixel(int x, int y);

void sdl_set_fps(int fps);
int sdl_init(int width, int height, int buffer_width, int buffer_height);
int sdl_run(void);
int sdl_cleanup(void);
 
SDL_Renderer* sdl_get_renderer();

#ifdef __cplusplus
    }
#endif

#define UNUSED_VAR(x) ((void)(x))

#endif