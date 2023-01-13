//
//  rb_pitrex.c
//  platform layer between game engine and pitrex
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_pitrex.h"
#include "rb_input.h"
#include "rb_engine.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// PiTrex includes
#include <pitrex/pitrexio-gpio.h>
#include <vectrex/vectrexInterface.h>
#include "rb_pitrex_window.h"

#define LIGHT_LOW       80
#define LIGHT_HIGH      110
#define LIGHT_DEFAULT   95
#define LIGHT_STEPS     (LIGHT_HIGH-LIGHT_LOW)/10

static int _light = LIGHT_DEFAULT;
static bool _quit = false;
static int _time_per_frame = 16;

double pitrex_get_ticks(void) {
    return v_millis() / 1000.0f;
}

void pitrex_frame(void) {
    v_WaitRecal();
    v_readButtons();
    v_readJoystick1Analog();
}

void pitrex_delay(void) {
    // TODO: Implement
}

void pitrex_draw_line(float x1, float y1, float x2, float y2) {
    v_line(x1, y1, x2, y2, _light); /* TODO: Get from graphics */
}

void pitrex_draw_vtext(float x, float y, char* str) {
#if 0
    v_printString(x, y, str, size, _light); /* TODO: Get from graphics */
#else
    v_setBrightness(color);
    v_printStringRaster(x, y, msg, 40, -7, 0);
#endif
}

int pitrex_init(void) {    
    vectrexinit(1);

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
    v_window(0, 0, 362, 482, 0); /* TODO: Put into settings */

    return 1;
}

void pitrex_handle_input() {
    // Handle joystick 1
    if (get_current_joy1_x() < -50) {
        input_set_control(CONTROL_LEFT, true);
    }
    else {
        input_set_control(CONTROL_LEFT, false);
    }    

    if (get_current_joy1_x() > 50) {
        input_set_control(CONTROL_RIGHT, true);
    }
    else {
        input_set_control(CONTROL_RIGHT, false);
    }    

    if (get_current_joy1_y() > 50) {
        input_set_control(CONTROL_UP, true);
    }
    else {
        input_set_control(CONTROL_UP, false);
    }    

    if (get_current_joy1_y() < -50) {
        input_set_control(CONTROL_DOWN, true);
    }
    else {
        input_set_control(CONTROL_DOWN, false);
    }    

    // Handle buttons from joystick 1
    input_set_control(BUTTON_1, (currentButtonState&0x01) == (0x01) ? true : false);
    input_set_control(BUTTON_2, (currentButtonState&0x02) == (0x02) ? true : false);
    input_set_control(BUTTON_3, (currentButtonState&0x04) == (0x04) ? true : false);
    input_set_control(BUTTON_4, (currentButtonState&0x08) == (0x08) ? true : false);
}

int pitrex_run() {
    rbmsg_str1("App folder is: ", platform_get_app_folder());
    rblog_num1("pitrex_run(): FPS=", 1000/_time_per_frame);

    _quit = false;

    input_clear_controls();

    int64_t next_frame = pitrex_get_ticks() + _time_per_frame;

    while (!_quit) {
        pitrex_frame();
        pitrex_handle_input();

        engine_step();
        engine_after_render();

        Uint32 now = pitrex_get_ticks();
        if (now < next_frame) {
            pitrex_dely(next_frame - now);
        }
        else {
            next_frame = now;
        }

        next_frame += _time_per_frame;
    }

    return 0;
}
