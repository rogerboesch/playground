//
//  rb_base.h
//  Game engine base code
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

#define UNUSED_VAR(x) ((void)(x))

// Base math stuff
#define P_PI 3.14159265358979323846264338327950288
#define DEG_TO_RAD(angleInDegrees) ((angleInDegrees) * P_PI / 180.0)
#define RAD_TO_DEG(angleInRadians) ((angleInRadians) * 180.0 / P_PI)

#endif
