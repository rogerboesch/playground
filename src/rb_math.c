//
//  rb_math.c
//  3d wireframe game engine
//
//  Mathematic
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_math.h"
#include "rb_vector.h"

#ifndef _WIN32 
    #include <unistd.h>
#endif

#include <math.h>
#include <stdint.h>

unsigned int iabs(int val) {
    int temp = val >> 8*(sizeof(int)-1);
    val ^= temp;

    return val - temp;
}

double fast_rsqrt(double number) {
    int64_t i;
    double x2, y;
    const double threehalfs = 1.5;

    x2 = number * 0.5;
    y  = number;
    i  = *(int64_t*) &y;                        // evil floating point bit level hacking
    i  = 0x5fe6eb50c7b537a9 - ( i >> 1 );       // what the fuck?
    y  = *(double*) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration

    return y;
}

unsigned int fast_dist(int x, int y) {
    unsigned int dx = iabs(x), dy = iabs(y), w;
    
    if (dy < dx) {
        w = dy >> 2;
        return (dx + w + (w >> 1));
    }
    else {
        w = dx >> 2;
        return (dy + w + (w >> 1));
    }
}

unsigned int fast_dist2(int x, int y) {
    unsigned int dx = iabs(x), dy = iabs(y), min, max;
    min = (dx<dy)?dx:dy;
    max = (dx<dy)?dy:dx;

    // coefficients equivalent to ( 123/128 * max ) and ( 51/128 * min )
    return ((( max << 8 ) + ( max << 3 ) - ( max << 4 ) - ( max << 1 ) +
           ( min << 7 ) - ( min << 5 ) + ( min << 3 ) - ( min << 1 )) >> 8 );
}
