//
//  rb_vector.h
//  3d wireframe game engine
//
//  Vector calculation
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#ifndef RB_VECTOR_H
#define RB_VECTOR_H

typedef struct {
    double x;
    double y;
} vec2;

typedef struct {
    double x; // (Euler: pitch)
    double y; // (Euler: yaw)
    double z; // (Euler: roll)
} vec3;

typedef struct {
    double x;
    double y;
    double z;
    double w;
} vec4;

vec3 vec3_make(float x, float y, float z);
vec3 vec3_add(const vec3, const vec3);
void vec3_addto(vec3*, const vec3*);
vec3 vec3_sub(const vec3, const vec3);
void vec3_subfrom(vec3*, const vec3*);
vec3 vec3_scale(const vec3, const double);
vec3 vec3_mult(const vec3, const vec3);
double vec3_dot(const vec3, const vec3);

vec3 vec3_rotate(vec3 v, vec3 rot);

#endif
