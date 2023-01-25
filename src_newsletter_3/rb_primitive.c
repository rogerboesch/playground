//
//  rb_primitive.c
//  3d wireframe game engine
//
//  3D primitives drawing
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_primitive.h"
#include "rb_base.h"
#include "rb_math.h"

#define EOV 9999

// Define the cube lines
double cube[13][6] = {
    {-0.5, 0.0, -0.5, 0.5, 0.0, -0.5},
    {-0.5, 0.0, -0.5, -0.5, 0.0, 0.5},
    {0.5, 0.0, -0.5, 0.5, 0.0, 0.5},
    {-0.5, 0.0, 0.5, 0.5, 0.0, 0.5},
    {-0.5, 1.0, -0.5, 0.5, 1.0, -0.5},
    {-0.5, 1.0, -0.5, -0.5, 1.0, 0.5},
    {0.5, 1.0, -0.5, 0.5, 1.0, 0.5},
    {-0.5, 1.0, 0.5, 0.5, 1.0, 0.5},
    {-0.5, 0.0, -0.5, -0.5, 1.0, -0.5},
    {-0.5, 0.0, 0.5, -0.5, 1.0, 0.5},
    {0.5, 0.0, -0.5, 0.5, 1.0, -0.5},
    {0.5, 0.0, 0.5, 0.5, 1.0, 0.5},
    {EOV, NAN, NAN, NAN, NAN, NAN}
};

// Define the plane lines
double plane[23][6] = {
    {-50.0, 0.0, -50.0, -50.0, 0.0, 50.0},
    {-40.0, 0.0, -50.0, -40.0, 0.0, 50.0},
    {-30.0, 0.0, -50.0, -30.0, 0.0, 50.0},
    {-20.0, 0.0, -50.0, -20.0, 0.0, 50.0},
    {-10.0, 0.0, -50.0, -10.0, 0.0, 50.0},
    {0.0, 0.0, -50.0, 0.0, 0.0, 50.0},
    {10.0, 0.0, -50.0, 10.0, 0.0, 50.0},
    {20.0, 0.0, -50.0, 20.0, 0.0, 50.0},
    {30.0, 0.0, -50.0, 30.0, 0.0, 50.0},
    {40.0, 0.0, -50.0, 40.0, 0.0, 50.0},
    {50.0, 0.0, -50.0, 50.0, 0.0, 50.0},
    {-50.0, 0.0, -50.0, 50.0, 0.0, -50.0},
    {-50.0, 0.0, -40.0, 50.0, 0.0, -40.0},
    {-50.0, 0.0, -30.0, 50.0, 0.0, -30.0},
    {-50.0, 0.0, -20.0, 50.0, 0.0, -20.0},
    {-50.0, 0.0, -10.0, 50.0, 0.0, -10.0},
    {-50.0, 0.0, 0.0, 50.0, 0.0, 0.0},
    {-50.0, 0.0, 10.0, 50.0, 0.0, 10.0},
    {-50.0, 0.0, 20.0, 50.0, 0.0, 20.0},
    {-50.0, 0.0, 30.0, 50.0, 0.0, 30.0},
    {-50.0, 0.0, 40.0, 50.0, 0.0, 40.0},
    {-50.0, 0.0, 50.0, 50.0, 0.0, 50.0},
    {EOV, NAN, NAN, NAN, NAN, NAN}
};

void primitive_draw_cube(vec3 pos, vec3 scl, vec3 rot, camera3d cam) {
    UNUSED_VAR(rot);
    
    for (double* line=(double*)cube; line[0] != EOV; line+=6) {
        vec3 start = {line[0]*scl.x+pos.x, line[1]*scl.y+pos.y, line[2]*scl.z+pos.z};
        vec3 end = {line[3]*scl.x+pos.x, line[4]*scl.y+pos.y, line[5]*scl.z+pos.z};
        draw3d_line_transform(start, end, pos, rot, cam);
    }
}

void primitive_draw_plane(float y,float z, float scale, camera3d cam) {
    for (double* line=(double*)plane;!isnan(line[0]);line+=6) {
        vec3 start = {line[0], line[1]+y, line[2]};
        start.z += (50 + z);
        start.z *= scale; start.x *= scale;
        
        vec3 end = {line[3], line[4]+y, line[5]};
        end.z += (50 + z);
        end.z *= scale; end.x *= scale;

        draw3d_line(start, end, cam);
    }
}

void primitive_draw_point_grid(camera3d cam) {
    // Draw point grid
    int sx = (int)cam.pos.x & 0xFFFFFFFE, sz = (int)cam.pos.z & 0xFFFFFFFE;
    const int GRIDSIZE = 160;
    int endx = sx+GRIDSIZE, endz = sz+GRIDSIZE;
    double alpha;

    for (int x=sx-GRIDSIZE; x<=endx; x+=2) {
        for (int z=sz-GRIDSIZE; z<=endz; z+=2) {
            if (!((x&15)&&(z&15))) {
                alpha = 1.0 - (double)fast_dist2(sx-x, sz-z) / GRIDSIZE;

                if (alpha > 0.0)
                    draw3d_point_alpha((vec3){x, 0.0, z}, cam, alpha*alpha);
            }
        }
    }
}
