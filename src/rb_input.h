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

// Keyboard support
#define KEY_A			 8
#define KEY_B			 9
#define KEY_C			10
#define KEY_D			11
#define KEY_E			12
#define KEY_F			13
#define KEY_G			14
#define KEY_H			15
#define KEY_I			16
#define KEY_J			17
#define KEY_K			18
#define KEY_L			19
#define KEY_M			20
#define KEY_N			21
#define KEY_O			22
#define KEY_P			23
#define KEY_Q			24
#define KEY_R			25
#define KEY_S			26
#define KEY_T			27
#define KEY_U			28
#define KEY_V			29
#define KEY_W			30
#define KEY_X			31
#define KEY_Y			32
#define KEY_Z			33
#define KEY_SPACE		34

#define MAX_CONTROLS    35

uint8_t input_get_control(uint8_t direction);
uint8_t input_get_button(uint8_t button);

void input_clear_controls(void);
void input_set_control(uint8_t number, bool flag);

#endif
