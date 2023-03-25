//
//  rb_platform.h
//  Game engine base code
//
//  Base types
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#ifndef RB_PLATFORM_H
#define RB_PLATFORM_H
   
#ifdef __cplusplus
extern "C" {
#endif

#include "rb_base.h"

#define INVERT_ON       1
#define INVERT_OFF      0
#define BRIGHTNESS_OFF  100 // means 100/1=1

// Must be implemented in the host app
void platform_set_window_title(const char* title);
void platform_clear_buffer(byte color);

// Used to fill meshes, so needs brightness info
void platform_set_pixel(int x, int y, byte color, int brightness);

// Only color, but invert flag, to differentiate between text (invert=true) and the rest
void platform_draw_line(int x1, int y1, int x2, int y2, byte color, int invert);

byte platform_get_input(byte code);
byte platform_get_control_state(byte code);

const char* platform_resource_file_path(const char* filename, const char* extension);

double platform_get_ms(void);

void platform_on_init(const char* name, int width, int height);
void platform_on_frame(double deltaTime);

#ifdef __cplusplus
}
#endif

#endif
