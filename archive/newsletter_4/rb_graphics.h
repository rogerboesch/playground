//
//  rb_graphics.h
//  3d wireframe game engine
//
//  Graphics buffer
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#ifndef RB_GRAPHICS_H
#define RB_GRAPHICS_H

#include "rb_base.h"

typedef struct {
    int x;
    int y;
} pixel;

typedef struct {
    uint16_t width, height;
} graphics;

typedef uint32_t ARGB_color;

typedef union {
    ARGB_color argb;
    struct {
        uint8_t a;
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };
} ARGB;

graphics* graphics_open(int width, int height);
int graphics_close(graphics* gb);

void graphics_fill(graphics* gb, uint32_t color);
void graphics_clear(graphics* gb);

void graphics_setpixel(graphics* gb, pixel px, uint32_t value);
void graphics_blend_over(graphics* gb, pixel px, uint32_t value);
ARGB_color graphics_getpixel(graphics* gb, pixel px);

ARGB_color graphics_rgb(uint8_t r, uint8_t g, uint8_t b);
ARGB_color graphics_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

#endif
