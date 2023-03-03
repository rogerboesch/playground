//
//  rb_math.h
//  3d wireframe game engine
//
//  Mathematic
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#ifndef RB_MATH_H
#define RB_MATH_H

#define HALF_PI 1.57079632679

unsigned int iabs(int val);
double fast_rsqrt(double number);
unsigned int fast_dist(int x, int y);
unsigned int fast_dist2(int x, int y);
int random_int(int min, int max);

#endif
