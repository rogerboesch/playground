//
//  rb_draw3d.h
//  3d wireframe game engine
//
//  3D drawing line and point drawing
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#ifndef RB_DRAW3D_H
#define RB_DRAW3D_H

#include "rb_vector.h"
#include "rb_draw.h"

typedef struct {
    vec3 pos;
    vec3 rot;
    double fov;
    double distort; // Not used yet
} camera3d;

void draw3d_point(vec3 point, camera3d cam);
void draw3d_point_alpha(vec3 point, camera3d cam, double alpha);
void draw3d_line(vec3 start, vec3 end, camera3d cam);
void draw3d_line_transform(vec3 start, vec3 end, vec3 pos, vec3 rot, camera3d cam);
void draw3d_line_transform(vec3 start, vec3 end, vec3 pos, vec3 rot, camera3d cam);

#endif
