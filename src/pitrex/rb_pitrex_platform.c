//
//  rb_pitrex.c
//  platform layer between game engine and pitrex
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_pitrex_platform.h"
#include "rb_pitrex_window.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// PiTrex includes
#include <pitrex/pitrexio-gpio.h>
#include <vectrex/vectrexInterface.h>

void game_set_control_state(int code, int state);

// Must be aligned to constants in rb_engine.hpp
#define CONTROL1_BTN1        0
#define CONTROL1_BTN2        1
#define CONTROL1_BTN3        2
#define CONTROL1_BTN4        3
#define CONTROL1_JOY_LEFT    4
#define CONTROL1_JOY_RIGHT   5
#define CONTROL1_JOY_UP      6
#define CONTROL1_JOY_DOWN    7

#define LIGHT_LOW       80
#define LIGHT_HIGH      110
#define LIGHT_DEFAULT   95
#define LIGHT_STEPS     (LIGHT_HIGH-LIGHT_LOW)/10

static int _light = LIGHT_DEFAULT;
static int _quit = 0;
static int _time_per_frame = 16;
static int _screen_width = 0;
static int _screen_height = 0;

int pitrex_get_brightness(int color) {    
    int light = (int)(10*color);
    int bright = LIGHT_LOW + (light*LIGHT_STEPS);
    bright = fmin(LIGHT_HIGH, bright);
    
    return bright;
}

double pitrex_get_ticks(void) {
    return v_millis() / 1000.0f;
}

void pitrex_frame(void) {
    v_WaitRecal();
    v_readButtons();
    v_readJoystick1Analog();
}

void pitrex_delay(int64_t ms) {
    // TODO: Implement
}

void pitrex_draw_line(float x1, float y1, float x2, float y2) {
    v_line(_screen_width-x1, y1, _screen_width-x2, y2, _light);
}

void pitrex_draw_vtext(float x, float y, char* str) {
    v_setBrightness(LIGHT_DEFAULT);
    v_printStringRaster(x-127, 127-y, str, 40, -7, 0);
}

int pitrex_init(int width, int height) {    
    vectrexinit(1);

    _screen_width = width;
    _screen_height = height;

#ifndef FREESTANDING
    v_setName("VEXXON"); /* TODO: Put into settings */
#endif

    v_init();
    usePipeline = 1;

#ifdef FREESTANDING
    v_setupIRQHandling();
    v_enableJoystickAnalog(1,1,0,0);
    v_enableButtons(1);
#endif

    v_setRefresh(50);
    v_setBrightness(60);

    v_set_hardware_orientation(VECTREX_INVERTED);
    v_window(0, 0, _screen_width, _screen_height, 0);

    return 1;
}

void pitrex_handle_input() {
    // Handle joystick 1
    if (currentJoy1X < -50) {
        game_set_control_state(CONTROL1_JOY_LEFT, true);
    }
    else {
        game_set_control_state(CONTROL1_JOY_LEFT, false);
    }    

    if (currentJoy1X > 50) {
        game_set_control_state(CONTROL1_JOY_RIGHT, true);
    }
    else {
        game_set_control_state(CONTROL1_JOY_RIGHT, false);
    }    

    if (currentJoy1Y > 50) {
        game_set_control_state(CONTROL1_JOY_UP, true);
    }
    else {
        game_set_control_state(CONTROL1_JOY_UP, false);
    }    

    if (currentJoy1Y < -50) {
        game_set_control_state(CONTROL1_JOY_DOWN, true);
    }
    else {
        game_set_control_state(CONTROL1_JOY_DOWN, false);
    }    

    // Handle buttons from joystick 1
    game_set_control_state(CONTROL1_BTN1, (currentButtonState&0x01) == (0x01) ? true : false);
    game_set_control_state(CONTROL1_BTN2, (currentButtonState&0x02) == (0x02) ? true : false);
    game_set_control_state(CONTROL1_BTN3, (currentButtonState&0x04) == (0x04) ? true : false);
    game_set_control_state(CONTROL1_BTN4, (currentButtonState&0x08) == (0x08) ? true : false);
}
