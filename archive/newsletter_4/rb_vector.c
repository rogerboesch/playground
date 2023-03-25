//
//  rb_vector.c
//  3d wireframe game engine
//
//  Vector calculation
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_vector.h"
#include <math.h>

vec3 vec3_make(float x, float y, float z) {
    vec3 result;
    result.x = x;
    result.y = y;
    result.z = z;
    
    return result;
}

vec3 vec3_add(const vec3 first, const vec3 second) {
    vec3 result = first;
    vec3_addto(&result, &second);
    return result;
}

void vec3_addto(vec3* first, const vec3* second) {
    first->x += second->x;
    first->y += second->y;
    first->z += second->z;
}

vec3 vec3_sub(const vec3 first, const vec3 second) {
    vec3 result = first;
    vec3_subfrom(&result, &second);
    
    return result;
}

void vec3_subfrom(vec3* first, const vec3* second) {
    first->x -= second->x;
    first->y -= second->y;
    first->z -= second->z;
}

vec3 vec3_scale(const vec3 vec, const double scalar) {
    return (vec3) {
        .x = scalar * vec.x,
        .y = scalar * vec.y,
        .z = scalar * vec.z
    };
}

vec3 vec3_mult(const vec3 a, const vec3 b) {
    return (vec3) {
        .x = a.x*b.x,
        .y = a.y*b.y,
        .z = a.z*b.z
    };
}

double vec3_dot(const vec3 a, const vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

typedef struct {
    float m[4][4];
} mat4x4;

mat4x4 _matrix_make_rotation_X(float fAngleRad) {
    mat4x4 matrix = { 0 };
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = cosf(fAngleRad);
    matrix.m[1][2] = sinf(fAngleRad);
    matrix.m[2][1] = -sinf(fAngleRad);
    matrix.m[2][2] = cosf(fAngleRad);
    matrix.m[3][3] = 1.0f;
    
    return matrix;
}

mat4x4 _matrix_make_rotation_Y(float fAngleRad) {
    mat4x4 matrix = { 0 };
    matrix.m[0][0] = cosf(fAngleRad);
    matrix.m[0][2] = sinf(fAngleRad);
    matrix.m[2][0] = -sinf(fAngleRad);
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = cosf(fAngleRad);
    matrix.m[3][3] = 1.0f;
    
    return matrix;
}

mat4x4 _matrix_make_rotation_Z(float fAngleRad) {
    mat4x4 matrix = { 0 };
    matrix.m[0][0] = cosf(fAngleRad);
    matrix.m[0][1] = sinf(fAngleRad);
    matrix.m[1][0] = -sinf(fAngleRad);
    matrix.m[1][1] = cosf(fAngleRad);
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    
    return matrix;
}

vec3 _matrix_multiply_vector(mat4x4 m, vec3 a) {
    vec4 i;
    i.x = a.x; i.y = a.y; i.z = a.z; i.w = 1;
    
    vec4 v;
    v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
    v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
    v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
    v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
    
    a.x = v.x; a.y = v.y; a.z = v.z;
    return a;
}

vec3 vec3_rotate(vec3 v, vec3 rot) {
    if (rot.x != 0) {
        mat4x4 matRotX = _matrix_make_rotation_X(rot.x);
        v = _matrix_multiply_vector(matRotX, v);
    }

    if (rot.y != 0) {
        mat4x4 matRotY = _matrix_make_rotation_Y(rot.y);
        v = _matrix_multiply_vector(matRotY, v);
    }

    if (rot.z != 0) {
        mat4x4 matRotZ = _matrix_make_rotation_Z(rot.z);
        v = _matrix_multiply_vector(matRotZ, v);
    }
    
    return v;
}
