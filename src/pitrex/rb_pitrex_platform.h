//
//  rb_pitrex.h
//  platform layer between game engine and PiTrex
//
//  04-08-2022, created by Roger Boesch
//  Copyright © 2022 by Roger Boesch - use only with permission
//

#ifndef rb_pitrex_h
#define rb_pitrex_h

#include "rb_platform.h"

#define P_PI 3.14159265358979323846264338327950288
#define DEG_TO_RAD(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define RAD_TO_DEG(angleInRadians) ((angleInRadians) * 180.0 / M_PI)

#define DEFAULT_TEXT_SIZE       7
#define DEFAULT_TEXT_SMALL_SIZE 6
#define DEFAULT_INPUT_WAIT_TIME 30

double pitrex_get_ticks(void);
int pitrex_get_brightness(int color);
void pitrex_delay(int64_t ms);

void pitrex_draw_line(float x1, float y1, float x2, float y2);
void pitrex_draw_vtext(float x, float y, char* str);

int pitrex_init(int width, int height);
void pitrex_frame(void);

#endif
