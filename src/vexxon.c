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

static const float LEVEL_WIDTH = 30.f;
static const float LEVEL_WIDTH_HALF = 15.0f;
static const float FLIGHT_WIDTH_LIMIT = 6.0f;
static const float FLIGHT_HEIGHT_LIMIT = 5.0f;
static const float FLIGHT_FLOOR_LIMIT = 0.1f;

static int player_id = 0;               
static float player_speed = 10.0f;
static float player_movement = 10.0f;

static float camera_x_offset = 0.0f;

void vexxon_settings(setting* setting) {
    setting->screen_width = 360*2;
    setting->screen_height = 480*2;
    setting->buffer_width = 360*2;
    setting->buffer_height = 480*2;
    setting->fullscreen = false;
}

void vexxon_init(camera3d* cam) {
    // Follow
    // cam->pos = vec3_make(0.0f, 2.0f, 0.0f);
    // cam->rot = vec3_make(0.0f, 0.0f, 0.0f);

    // ISO view
    cam->pos = vec3_make(7.82f, 4.37f, 1.82f);
    cam->rot = vec3_make(0.0f, -0.43f, 0.0f);

    // Player/Space fighter
    player_id = game_object_create(GAME_OBJECT_CUBE);
    game_object_set_pos(player_id, 0.0f, 1.0f, 10.0f, COORDINATE_IS_ABSOLUTE);
    game_object_set_speed(player_id, 0, 0, player_speed);

    // Get camera offset for later update
    vec3 pos = game_object_get_position(player_id);
    camera_x_offset = pos.x - cam->pos.x;

    // Create some random level elements
    for (int i = 0; i < 10; i++) {
        int id = game_object_create(GAME_OBJECT_CUBE);

        int offset = random_int(-5, 5);
        int scale = random_int(1, 5);

        game_object_set_pos(id, 0.0f+offset, 1.0f, i*30.0f+15.0f, COORDINATE_IS_ABSOLUTE);
        game_object_set_scl(id, scale, 1.0f, 1.0f, COORDINATE_IS_ABSOLUTE);
        game_object_set_color(id, ARGB_RED);
    }

    return;
}

void vexxon_update(float delta, camera3d* cam) {
    UNUSED_VAR(delta);

    // Move camera forward
    cam->pos.z += player_speed * delta;
 
    if (input_get_control(CONTROL_LEFT)) {
        game_object_set_pos(player_id, -player_movement * delta, 0.0f, 0.0f, COORDINATE_IS_RELATIVE);

        vec3 pos = game_object_get_position(player_id);
        if (pos.x < -FLIGHT_WIDTH_LIMIT) {
            game_object_set_pos(player_id, -FLIGHT_WIDTH_LIMIT, pos.y, pos.z, COORDINATE_IS_ABSOLUTE);
        }
    }
    if (input_get_control(CONTROL_RIGHT)) {
        game_object_set_pos(player_id, player_movement * delta, 0.0f, 0.0f, COORDINATE_IS_RELATIVE);

        vec3 pos = game_object_get_position(player_id);
        if (pos.x > FLIGHT_WIDTH_LIMIT) {
            game_object_set_pos(player_id, FLIGHT_WIDTH_LIMIT, pos.y, pos.z, COORDINATE_IS_ABSOLUTE);
        }
    }
 
    vec3 player_pos = game_object_get_position(player_id);
    cam->pos.x = player_pos.x - camera_x_offset;

    if (input_get_control(CONTROL_UP)) {
        game_object_set_pos(player_id, 0.0f, player_movement * delta, 0.0f, COORDINATE_IS_RELATIVE);

        vec3 pos = game_object_get_position(player_id);
        if (pos.y > FLIGHT_HEIGHT_LIMIT) {
            game_object_set_pos(player_id, pos.x, FLIGHT_HEIGHT_LIMIT, pos.z, COORDINATE_IS_ABSOLUTE);
        }
    }
    if (input_get_control(CONTROL_DOWN)) {
        game_object_set_pos(player_id, 0.0f, -player_movement * delta, 0.0f, COORDINATE_IS_RELATIVE);

        vec3 pos = game_object_get_position(player_id);
        if (pos.y < FLIGHT_FLOOR_LIMIT) {
            game_object_set_pos(player_id, pos.x, FLIGHT_FLOOR_LIMIT, pos.z, COORDINATE_IS_ABSOLUTE);
        }
    }
}

void vexxon_pre_render(camera3d* cam) {
    // Fill background
    graphics_fill(draw_getgb(), ARGB_BLACK);

    // Get resolution (width, height)
    pixel res = draw_getres();

    // Draw fix horizontal line (horizon)
    draw_setcolor(ARGB_GREY);

    double hpos = (0.5f + tan(cam->rot.x) * cam->fov) * res.y;
    draw_line((vec2) { 0.0f, hpos }, (vec2) { res.x, hpos });

    // Draw horizontal lines in 3d space (floor)
    draw_setcolor(ARGB_BLUE);

    for (int i = 0; i < 10; i++) {
        vec3 start = { -LEVEL_WIDTH_HALF, 0.0f, i * 30.0f };
        vec3 end = { LEVEL_WIDTH_HALF, 0.0f, i * 30.0f };

        draw3d_line(start, end, *cam);
    }
}

void vexxon_post_render(camera3d* cam) {
    UNUSED_VAR(cam);
    
    draw_setcolor(ARGB_WHITE);
    vtext_draw_string(10, 10, "VEXXON", 1.0f);

    draw_setcolor(ARGB_YELLOW);
    vtext_draw_string(10, 40, "ARROW LEFT   MOVE LEFT", 0.5f);
    vtext_draw_string(10, 50, "ARROW RIGHT  MOVE RIGHT", 0.5f);
    vtext_draw_string(10, 60, "ARROW UP     MOVE FORWARD", 0.5f);
    vtext_draw_string(10, 70, "ARROW DOWN   MOVE BACK", 0.5f);
    vtext_draw_string(10, 80, "BUTTON 1     MOVE UP", 0.5f);
    vtext_draw_string(10, 90, "BUTTON 2     MOVE DOWN", 0.5f);
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
