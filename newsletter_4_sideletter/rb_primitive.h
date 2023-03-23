//
//  rb_primitive.h
//  3d wireframe game engine
//
//  3D primitives drawing
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#ifndef RB_PRIMITIVE_H
#define RB_PRIMITIVE_H

#include "rb_vector.h"
#include "rb_draw3d.h"

void primitive_draw_cube(vec3 pos, vec3 scl, vec3 rot, camera3d cam);
void primitive_draw_plane(float y, float z, float scale, camera3d cam);

void primitive_draw_point_grid(camera3d cam);

#endif
