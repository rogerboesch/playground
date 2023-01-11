//
//  rb_graphics.c
//  3d wireframe game engine
//
//  Graphics buffer
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_graphics.h"
#include "rb_draw.h"
#include "rb_graphics.h"

#ifndef PITREX
    #include "rb_sdl.h"
#endif

#include <string.h>

extern void platform_clear_buffer(ARGB_color color);

graphics* graphics_open(int width, int height) {
    graphics* gb;

    gb = (graphics*)malloc(sizeof(graphics));
    gb->width = width;
    gb->height = height;

    return gb;
}

int graphics_close(graphics* gb) {
    if (gb != NULL) {
        free(gb);
    }
    
    return 0;
}

void graphics_fill(graphics* gb, ARGB_color color) {
#ifndef PITREX
    sdl_clear_buffer(color);
#endif
}

void graphics_setpixel(graphics* gb, pixel px, ARGB_color value) {
#ifndef PITREX
    sdl_set_pixel(px.x, px.y, value);
#endif
}

void graphics_blend_over(graphics* gb, pixel px, ARGB_color value) {
    ARGB bg, fg, color;
    bg.argb = sdl_get_pixel(px.x, px.y);
    fg.argb = value;

    float alpha = (float)fg.a / 255;
    color.r = fg.r*alpha + bg.r*(1-alpha);
    color.g = fg.g*alpha + bg.g*(1-alpha);
    color.b = fg.b*alpha + bg.b*(1-alpha);
    color.a = 255;

#ifndef PITREX
    sdl_set_pixel(px.x, px.y, value);
#endif
}

ARGB_color graphics_getpixel(graphics* gb, pixel px) {
#ifdef PITREX
    return 0; 
#else
    return sdl_get_pixel(px.x, px.y);
#endif
}

void graphics_clear(graphics* gb) {
#ifdef PITREX
#else
    sdl_clear_buffer(ARGB_BLACK);
#endif
}

ARGB_color graphics_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return (r<<8) | (r << 16) | (b << 24);
}

ARGB_color graphics_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return graphics_rgb(r, g, b) | (a << 0);
}
