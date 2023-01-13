//
//  rb_input.h
//  3d wireframe game engine
//
//  Input handler
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#ifndef RB_INPUT_H
#define RB_INPUT_H

#include <stdint.h>
#include "rb_base.h"

// TODO: Also support 2nd Controller
#define BUTTON_1         0
#define BUTTON_2         1
#define BUTTON_3         2
#define BUTTON_4         3
#define CONTROL_LEFT     4
#define CONTROL_RIGHT    5
#define CONTROL_UP       6
#define CONTROL_DOWN     7

#define MAX_CONTROLS    8

uint8_t input_get_control(uint8_t direction);
uint8_t input_get_button(uint8_t button);

void input_clear_controls(void);
void input_set_control(uint8_t number, bool flag);

#endif
