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

#define CONTROL_LEFT     0
#define CONTROL_RIGHT    1
#define CONTROL_UP       2
#define CONTROL_DOWN     3

#define BUTTON_1         4
#define BUTTON_2         5
#define BUTTON_3         6
#define BUTTON_4         7
#define BUTTON_5         8
#define BUTTON_6         9
#define BUTTON_7        10
#define BUTTON_8        11
#define BUTTON_9        12
#define BUTTON_0        13

#define MAX_CONTROLS    14

uint8_t input_get_control(uint8_t direction);
uint8_t input_get_button(uint8_t button);

void input_clear_controls(void);
void input_set_control(uint8_t number, bool flag);

#endif
