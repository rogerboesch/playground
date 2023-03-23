//
//  rb_draw3d.c
//  3d wireframe game engine
//
//  3D drawing line and point drawing
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_draw3d.h"
#include "rb_base.h"

// Euler ROTate around 2 axes
void _draw3d_erot2(vec3* c, const vec3* r) {
    double rx, ry, rz;

    // Yaw (around Y axis)
    double t = r->y; // theta
    rx = c->x * cos(t) - c->z * sin(t);
    rz = c->x * sin(t) + c->z * cos(t);

    // Pitch (around X axis)
    t = r->x;
    ry = c->y * cos(t) - rz * sin(t);
    rz = c->y * sin(t) + rz * cos(t);

    // store calculated values
    c->x = rx; c->y = ry; c->z = rz;
}

vec2 _draw3d_project2d(const vec3* c, double fov) {
    // this assumes that res.x > res.y
    pixel res = draw_getres();
    double aspect = (double)res.y / (double)res.x;
    double z = (c->z==0.0 ? 0.00001 : c->z); // ugly hacks

    return (vec2) {
        .x = (0.5 + c->x / z * fov * aspect) * res.x,
        .y = (0.5 - c->y / z * fov) * res.y
    };
}

double _draw3d_pdist(vec3 point) {
    const static vec3 clip_normal = {0.0, 0.0, 1.0};
    const static double cdist = 0.0;

    return vec3_dot(point, clip_normal) - cdist;
}

// Clips line by moving the "back" point to the plane intersection.
int _draw3d_clip_near(vec3* start, vec3* end, double clip_dist) {

    // If both ends are in front of the plane: we don't need to do anything
    if (start->z > clip_dist && end->z > clip_dist)
		return 0;

    // If both ends are behind us: bail out completely, ignore this line
    if (start->z <= clip_dist && end->z <= clip_dist)
		return 1;

    // otherwise line passes through clipping plane, work out which end is behind us
    int order = (start->z > end->z);
    vec3* front = order?start:end;
    vec3* back = order?end:start;

    double dist = (clip_dist - back->z), linelen = (front->z - back->z);
    double intf = dist / linelen;

    if (intf < -1 || intf > 1) {
		// this shouldn't happen
        rblog("Line probably doesn't intersect clipping plane!"); // \nfront.z=%f back.z=%f linelen=%f dist=%f intf=%f\n", front->z, back->z, linelen, dist, intf);
        return 1;
    }

    vec3 isect = {
        back->x + intf*(front->x - back->x),
        back->y + intf*(front->y - back->y),
        back->z + intf*(front->z - back->z)
    };

    *back = isect;

    return 0;
}

void draw3d_point(vec3 point, camera3d cam) {
    // Translation and rotation in 3D space
    vec3_subfrom(&point, &cam.pos);
    _draw3d_erot2(&point, &cam.rot);

    // Discard any points that are now "behind" us i.e. the Z value is negative
    if (point.z <= 0.1)
		return;

    // Map to screen coordinates and draw
    draw_point(_draw3d_project2d(&point, cam.fov));
}

void draw3d_point_alpha(vec3 point, camera3d cam, double alpha) {
    // Translation and rotation in 3D space
    vec3_subfrom(&point, &cam.pos);
    _draw3d_erot2(&point, &cam.rot);

    // Discard any points that are now "behind" us i.e. the Z value is negative
    if (point.z <= 0.1)
		return;

    // Map to screen coordinates and draw
    draw_point_alpha(_draw3d_project2d(&point, cam.fov), alpha);
}

void draw3d_line(vec3 start, vec3 end, camera3d cam) {
    // Translation in 3D space, by subtracting camera position
    vec3_subfrom(&start, &cam.pos); // start point
    vec3_subfrom(&end, &cam.pos);   // end point
    
    // Rotation in 3D space: Apply camera pitch and yaw
    _draw3d_erot2(&start, &cam.rot); // start point
    _draw3d_erot2(&end, &cam.rot);   // end point

    // Clip lines to front clipping plane, if required
    // Will return 1 if the line is entirely behind us,
    // in which case we can just ignore the line completely.
    // Front clipping plane is 0.1 units in front of camera
    if (_draw3d_clip_near(&start, &end, 0.1))
        return;

    // currently we don't clip to the edges of the view pyramid

    // Translate 3d lines to 2d screen space, applying FOV (assumes 480px high screen)
    vec2 startpos = _draw3d_project2d(&start, cam.fov);
    vec2 endpos = _draw3d_project2d(&end, cam.fov);

    // draw the 2d line on the screen. Will do its own 2d clipping
    draw_line_aa(startpos, endpos);
}

void draw3d_line_transform(vec3 start, vec3 end, vec3 pos, vec3 rot, camera3d cam) {
    vec3_subfrom(&start, &pos);
    vec3_subfrom(&end, &pos);
    
    start = vec3_rotate(start, rot);
    end = vec3_rotate(end, rot);

    vec3_addto(&start, &pos); // start point
    vec3_addto(&end, &pos);   // end point

    // Translation in 3D space, by subtracting camera position
    vec3_subfrom(&start, &cam.pos); // start point
    vec3_subfrom(&end, &cam.pos);   // end point
    
    // Rotation in 3D space: Apply camera pitch and yaw
    _draw3d_erot2(&start, &cam.rot); // start point
    _draw3d_erot2(&end, &cam.rot);   // end point

    // Clip lines to front clipping plane, if required
    // Will return 1 if the line is entirely behind us,
    // in which case we can just ignore the line completely.
    // Front clipping plane is 0.1 units in front of camera
    if (_draw3d_clip_near(&start, &end, 0.1))
        return;

    // currently we don't clip to the edges of the view pyramid

    // Translate 3d lines to 2d screen space, applying FOV (assumes 480px high screen)
    vec2 startpos = _draw3d_project2d(&start, cam.fov);
    vec2 endpos = _draw3d_project2d(&end, cam.fov);

    // draw the 2d line on the screen. Will do its own 2d clipping
    draw_line_aa(startpos, endpos);
}
