//
//  vexxon.c
//  3d wireframe game engine
//
//  Vexxon game
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_engine.h"
#include "rb_math.h"
#include "rb_vtext.h"
#include "rb_primitive.h"
#include "rb_level.h"

#include <math.h>

static float _speed = 0.0f;
static float _accel = 0.3f;
static float _moving = 0.1f;
static int cube_id = 0;

void vexxon_settings(setting* setting) {
    setting->screen_width = 360*2;
    setting->screen_height = 480*2;
    setting->buffer_width = 360;
    setting->buffer_height = 480;
    setting->fullscreen = false;
}

void vexxon_init(void) {
    platform_set_app_folder("vexxon");

    cube_id = game_object_create(GAME_OBJECT_CUBE);
    game_object_set_pos(cube_id, 0, 3, 10, COORDINATE_IS_ABSOLUITE);

    return;
}

void vexxon_update(float delta, camera3d* cam) {
    UNUSED_VAR(delta);
    double t = cam->rot.y;

    game_object_set_rot(cube_id, 0, 1/delta, 0, COORDINATE_IS_RELATIVE);

    cam->pos.z += _speed;

    if (input_get_control(CONTROL_LEFT)) {
        cam->pos.z += _moving*sin(t);
        cam->pos.x -= _moving*cos(t);
    }
    else if (input_get_control(CONTROL_RIGHT)) {
        cam->pos.z -= _moving*sin(t);
        cam->pos.x += _moving*cos(t);
    }
    
    if (input_get_control(CONTROL_UP)) {
        cam->pos.z += _accel * cos(t);
        cam->pos.x += _accel * sin(t);
    }
    else if (input_get_control(CONTROL_DOWN)) {
        cam->pos.z -= _accel * cos(t);
        cam->pos.x -= _accel * sin(t);
    }

    if (input_get_button(BUTTON_1)) {
        cam->pos.y += _moving;
    }
    else if (input_get_button(BUTTON_2)) {
        cam->pos.y -= _moving;
    }
}

void vexxon_pre_render(camera3d* cam) {
    graphics_fill(draw_getgb(), ARGB_BLACK);

    // Draw horizontal line
    pixel res = draw_getres();

    draw_setcolor(ARGB_GREY);
    double hpos = (0.5+tan(cam->rot.x)*cam->fov)*res.y;
    draw_line((vec2){0.0, hpos}, (vec2){res.x, hpos});

    draw_setcolor(ARGB_BLUE);

    for (int i = 0; i < 10; i++) {
        vec3 start = {-15, 0, i*30};
        vec3 end = {15, 0, i*30};
        
        draw3d_line(start, end, *cam);
    }

    draw_setcolor(ARGB_YELLOW);
    vtext_draw_string(10, 40, "ARROW LEFT   MOVE LEFT", 0.5f);
    vtext_draw_string(10, 50, "ARROW RIGHT  MOVE RIGHT", 0.5f);
    vtext_draw_string(10, 60, "ARROW UP     MOVE FORWARD", 0.5f);
    vtext_draw_string(10, 70, "ARROW DOWN   MOVE BACK", 0.5f);
    vtext_draw_string(10, 80, "BUTTON 1     MOVE UP", 0.5f);
    vtext_draw_string(10, 90, "BUTTON 2     MOVE DOWN", 0.5f);
}

void vexxon_post_render(camera3d* cam) {
    UNUSED_VAR(cam);
    
    draw_setcolor(ARGB_WHITE);
    vtext_draw_string(10, 10, "VEXXON", 1.0f);
}

void vexxon_cleanup(void) {}

void vexxon_after_render(void) {}

bool vexxon_handle_key(RBEvent event) { return false; }

void game_main(void) {
    game_fp fp;

    fp.settings = vexxon_settings;
    fp.init = vexxon_init;
    fp.update = vexxon_update;
    fp.pre_render = vexxon_pre_render;
    fp.post_render = vexxon_post_render;
    fp.after_render = vexxon_after_render;
    fp.cleanup = vexxon_cleanup;
    fp.handle_key = vexxon_handle_key;

    engine_set_functions(fp);
}
