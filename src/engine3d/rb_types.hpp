//
//  rb_types.hpp
//  3d wireframe game engine: basic primitives
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#pragma once

typedef unsigned char byte;

struct Vec2D {
    float x;
    float y;
};

struct Vec3D {
    float x;
    float y;
    float z;
    float w;

    Vec3D() { x = 0.0f; y = 0.0f; z = 0.0f; w = 1.0f; }
    Vec3D(float xx, float yy, float zz) { x = xx; y = yy; z = zz; w = 1.0f; }
};

struct VecRGB {
    byte r = 0;
    byte g = 0;
    byte b = 0;
};

struct Triangle {
    Vec3D p[3];
    byte color;
    int bright;
    int h;          // hide flag (fast solution: number of line not to draw, middle line of a quad)

    Triangle() { p[0] = Vec3D(); p[1] = Vec3D(); p[2] = Vec3D(), color = 0; bright = 0; h = 0; }
    Triangle(Vec3D v1, Vec3D v2, Vec3D v3) { p[0] = v1; p[1] = v2; p[2] = v3, color = 0; bright = 0; h = 0; }
};

struct Mat4x4 {
    float m[4][4] = {{ 0 }};
};
