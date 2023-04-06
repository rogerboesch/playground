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
    int h = 0; // hide flag (fast solution: number of line not to draw, middle line of a quad)
};

struct Mat4x4 {
    float m[4][4] = {{ 0 }};
};
