//
//  rb_draw.h
//  3d wireframe game engine
//
//  2D drawing
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#ifndef RB_DRAW_H
#define RB_DRAW_H

#include "rb_graphics.h"
#include "rb_vector.h"

#define ARGB_BLACK      0xFF000000u
#define ARGB_WHITE      0xFFFFFFFFu
#define ARGB_RED        0xFFFF0000u
#define ARGB_GREEN      0xFF00FF00u
#define ARGB_BLUE       0xFF0000FFu
#define ARGB_YELLOW     0xFFFFFF00u
#define ARGB_MAGENTA    0xFFFF00FFu
#define ARGB_CYAN       0xFF00FFFFu
#define ARGB_GREY       0xFF666666u
#define ARGB_LIGHTGREY  0xFFAAAAAAu

void draw_setgb(graphics* gb);
graphics* draw_getgb(void);
pixel draw_getres(void);
void draw_setcolor(ARGB_color color);

void draw_line(vec2 start, vec2 end);
void draw_line_aa(vec2 start, vec2 end);

void draw_point(vec2 point);
void draw_point_alpha(vec2 point, double alpha);

void draw_clear(void);

void draw_rainbow(int height);

#endif
