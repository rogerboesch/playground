//
//  rb_base.h
//  3d wireframe game engine
//
//  Base types
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#ifndef RB_BASE_H
#define RB_BASE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifndef byte
    typedef unsigned char byte;
#endif

#ifndef __cplusplus

typedef uint8_t bool;
#define true 1
#define false 0

#endif

#define UNUSED_VAR(x) ((void)(x))

#ifdef __cplusplus
extern "C" {
#endif

void rb_message(const char* msg, uint8_t user, uint8_t cr);
void rb_message_str1(const char* msg, const char* str1, uint8_t user, uint8_t cr);
void rb_message_num1(const char* msg, int num1, uint8_t user, uint8_t cr);

#ifdef __cplusplus
}
#endif

// Messages added to log only (CR is added)
#define rblog(msg) rb_message(msg, 0, 1);
#define rblog_str1(msg, str1) rb_message_str1(msg, str1, 0, 1);
#define rblog_num1(msg, num1) rb_message_num1(msg, num1, 0, 1);

// Messages are showed to user and added to log (CR is added)
#define rbmsg(msg) rb_message(msg, true, true);
#define rbmsg_str1(msg, str1) rb_message_str1(msg, str1, 1, 1);
#define rbmsg_num1(msg, num1) rb_message_num1(msg, num1, 1, 1);

// Print functionality (To print CR use rbprint_newline)
#define rbprint(str) rb_message(str, 1, 0);
#define rbprint_newline(str) rb_message(str, 1, 1);
#define rbprint_num(num) rb_message_num1("", num, 1, 0);

#endif
