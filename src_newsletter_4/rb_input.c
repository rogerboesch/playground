//
//  rb_input.c
//  3d wireframe game engine
//
//  Input handler
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_input.h"
#include "rb_base.h"
#include <string.h>

bool _controls[MAX_CONTROLS];

void input_clear_controls(void) {
    memset(&_controls[0], 0, MAX_CONTROLS);    
}

void input_set_control(uint8_t number, bool flag) {
    _controls[number] = flag;
}

bool input_get_control(uint8_t direction) {
    return _controls[direction];
}

bool input_get_button(uint8_t button) {
    return _controls[button];
}
