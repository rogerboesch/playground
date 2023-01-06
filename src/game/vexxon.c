//
//  target_vexxon.c
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

#define LEVEL_WIDTH          20     // Width of field
#define LEFT_BORDER         -10     // Right Border
#define RIGHT_BORDER         10     // Left Border

#define MAX_BORDER_HEIGHT    7      // Maximum height of left border
#define START_DISTANCE       50     // Z position where element start drawing

#define SPEED_GROUND        0       // Ground speed
#define SPEED_ENEMY_JET     2       // Speed of enemy jet

#define COLOR_FLOOR         ARGB_BLUE
#define COLOR_WALL          ARGB_WHITE
#define COLOR_BLOCK_WALL    ARGB_GREEN
#define COLOR_SILO          ARGB_CYAN
#define COLOR_TANK          ARGB_MAGENTA
#define COLOR_ROCKET        ARGB_YELLOW
#define COLOR_JET           ARGB_RED

void _create_section(level_line* line);
void _add_left_border_cube(int height);
void _add_right_border_cube(int height);
void _add_top_wall(int height);
void _add_bottom_wall(int height);
void _add_left_wall(void);
void _add_right_wall(void);
void _add_level_object(level_obj* obj);
void _add_flying_jet(level_obj* obj);
void _add_jet(level_obj* obj);
void _add_tank(level_obj* obj);
void _add_rocket(level_obj* obj);
void _add_fuel_silo(level_obj* obj);
void _add_end_level(level_obj* obj);

int _distance = START_DISTANCE;
int _space_area = -1;                // Here space starts (end of floor)
float _speed = 0.0f;
float _moving = 0.4f;

void vexxon_target_settings(setting* setting) {
    setting->screen_width = 1280;
    setting->screen_height = 720;
    setting->buffer_width = 1280/2;
    setting->buffer_height = 720;
    setting->fullscreen = false;
}

void vexxon_target_init(void) {
    platform_set_app_folder("vexxon");

    if (!level_load_number(1)) {
        return;
    }

    _distance = START_DISTANCE;
    _space_area = -1;
    int count = 0;
    
    while (level_has_more_lines()) {
        level_line* line;
        level_get_current_line(&line);

        _create_section(line);
        
        _distance += 10;
        
        if (_space_area == -1 && line->border == 0) {
            _space_area = count;
        }
        
        count++;
    }

    return;
}

void vexxon_target_update(float delta, camera3d* cam) {
    UNUSED_VAR(delta);
    double t = cam->rot.y;
    
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
        cam->pos.y += _moving;
    }
    else if (input_get_control(CONTROL_DOWN)) {
        cam->pos.y -= _moving;
    }

    if (input_get_button(BUTTON_1)) {
        cam->rot.x += 0.1/cam->fov;
    }
    if (input_get_button(BUTTON_2)) {
        cam->rot.x -= 0.1/cam->fov;
    }
    if (input_get_button(BUTTON_3)) {
        cam->pos.z += _moving*cos(t);
        cam->pos.x += _moving*sin(t);
    }
    if (input_get_button(BUTTON_4)) {
        cam->pos.z -= _moving*cos(t);
        cam->pos.x -= _moving*sin(t);
    }
    if (input_get_button(BUTTON_5)) {
        cam->pos.z += _moving*sin(t);
        cam->pos.x -= _moving*cos(t);
    }
    if (input_get_button(BUTTON_6)) {
        cam->pos.z -= _moving*sin(t);
        cam->pos.x += _moving*cos(t);
    }
    if (input_get_button(BUTTON_7)) {
        cam->pos.y += _moving;
    }
    if (input_get_button(BUTTON_8)) {
        cam->pos.y -= _moving;
    }
    if (input_get_button(BUTTON_9)) {
        //cam->fov *= 1.25;
        _speed = 0.5;
    }
    if (input_get_button(BUTTON_0)) {
        //cam->pos.x = 0.0; cam->pos.y = 2.0; cam->pos.z = -10.0;
        //cam->rot.x = 0.0; cam->rot.y = 0.0; cam->rot.z = 0.0;
        //cam->fov = 1.0;

        _speed *= -1.0;
    }
}

void vexxon_target_pre_render(camera3d* cam) {
    graphics_fill(draw_getgb(), ARGB_BLACK);

/*
    draw_rainbow(120);

    pixel res = draw_getres();
    draw_setcolor(ARGB_RED);
    draw_line((vec2){0.0, 200}, (vec2){res.x, 200});

    draw_setcolor(ARGB_GREEN);
    draw_line((vec2){0.0, 300}, (vec2){res.x, 300});

    draw_setcolor(ARGB_BLUE);
    draw_line((vec2){0.0, 400}, (vec2){res.x, 400});

    draw_setcolor(ARGB_YELLOW);
    draw_line((vec2){0.0, 500}, (vec2){res.x, 500});

    draw_setcolor(ARGB_CYAN);
    draw_line((vec2){0.0, 600}, (vec2){res.x, 600});

    vec3 pos = {0,0,0};
    vec3 scl = {1,1,1};
    vec3 rot = {0,0,0};
    primitive_draw_cube(pos, scl, rot, *cam);
*/

    // Draw horizontal line
    pixel res = draw_getres();

    draw_setcolor(ARGB_GREY);
    double hpos = (0.5+tan(cam->rot.x)*cam->fov)*res.y;
    draw_line((vec2){0.0, hpos}, (vec2){res.x, hpos});

#if _WIN32
    draw_setcolor(COLOR_FLOOR);

    int offset = START_DISTANCE/10;
    
    for (int i = 0; i < _space_area+offset; i++) {
        vec3 start = {LEFT_BORDER, 0, 5+i*10};
        vec3 end = {LEFT_BORDER+LEVEL_WIDTH, 0, 5+i*10};
        
        draw3d_line(start, end, *cam);
    }

    primitive_draw_point_grid(*cam);
#endif
}

void vexxon_target_post_render(camera3d* cam) {
    UNUSED_VAR(cam);
    
    draw_setcolor(ARGB_WHITE);
    vtext_draw_string(10, 10, "VEXXON");
}

void vexxon_target_cleanup(void) {}

void vexxon_target_after_render(void) {
}

bool vexxon_target_handle_key(RBEvent event) {
	return false;
}

void game_main(void) {
    game_fp fp;

    fp.settings = vexxon_target_settings;
    fp.init = vexxon_target_init;
    fp.update = vexxon_target_update;
    fp.pre_render = vexxon_target_pre_render;
    fp.post_render = vexxon_target_post_render;
    fp.after_render = vexxon_target_after_render;
    fp.cleanup = vexxon_target_cleanup;
    fp.handle_key = vexxon_target_handle_key;

    engine_set_functions(fp);
}

void _create_section(level_line* line) {
    int height = line->border-2;
    
    // Wall types
    switch (line->border) {
        // No border
        case 0:
            rblog(" Add no border");
            break;
        // Has border
        case 1:
            rblog(" Add small border");
            _add_left_border_cube(3);
            _add_right_border_cube(0);
            break;
        // End level border
        case 2:
            rblog(" Add end level border");
            _add_left_border_cube(1);
            _add_right_border_cube(1);
            break;
        // Has border: height
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            rblog_num1(" Add wall with height: ", height);
            _add_left_border_cube(MAX_BORDER_HEIGHT);
            _add_right_border_cube(height+1);
            _add_bottom_wall(height);
            break;
        case 10:
            rblog(" Add fly trough wall (X)");
            _add_left_border_cube(MAX_BORDER_HEIGHT);
            _add_right_border_cube(MAX_BORDER_HEIGHT);
            _add_bottom_wall(2);
            _add_top_wall(2);
            break;
        case 11:
            rblog(" Add left open wall (Y)");
            _add_left_border_cube(MAX_BORDER_HEIGHT);
            _add_right_border_cube(MAX_BORDER_HEIGHT);
            _add_right_wall();
            break;
        case 12:
            rblog(" Add right open wall (Z)");
            _add_left_border_cube(MAX_BORDER_HEIGHT);
            _add_right_border_cube(MAX_BORDER_HEIGHT);
            _add_left_wall();
            break;
    }
    
    for (int i = 0; i < line->object_count; i++) {
        _add_level_object(&line->objects[i]);
    }
}

void _add_left_border_cube(int height) {
    int number = game_object_add(GAME_OBJECT_CUBE);

    game_object_set_pos(number, LEFT_BORDER, 0, _distance);
    game_object_set_color(number, COLOR_WALL);
    game_object_set_speed(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, 1, height, 1);
}

void _add_right_border_cube(int height) {
    int number = game_object_add(GAME_OBJECT_CUBE);

    game_object_set_pos(number, RIGHT_BORDER, 0, _distance);
    game_object_set_color(number, COLOR_WALL);
    game_object_set_speed(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, 1, height, 1);

    if (height == 0) {
        game_object_set_scl(number, 1, 0.2, 1);
    }
}

void _add_top_wall(int height) {
    int number = game_object_add(GAME_OBJECT_CUBE);

    game_object_set_pos(number, 0, MAX_BORDER_HEIGHT-height, _distance);
    game_object_set_color(number, COLOR_BLOCK_WALL);
    game_object_set_speed(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, LEVEL_WIDTH-1, height, 1);
}

void _add_bottom_wall(int height) {
    int number = game_object_add(GAME_OBJECT_CUBE);

    game_object_set_pos(number, 0, 0, _distance);
    game_object_set_color(number, COLOR_BLOCK_WALL);
    game_object_set_speed(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, LEVEL_WIDTH-1, height, 1);
}

void _add_left_wall(void) {
    int number = game_object_add(GAME_OBJECT_CUBE);

    game_object_set_pos(number, -LEVEL_WIDTH/4+0.5, 0, _distance);
    game_object_set_color(number, COLOR_BLOCK_WALL);
    game_object_set_speed(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, LEVEL_WIDTH/2, MAX_BORDER_HEIGHT, 1);
}

void _add_right_wall(void) {
    int number = game_object_add(GAME_OBJECT_CUBE);

    game_object_set_pos(number, LEVEL_WIDTH/4-0.5, 0, _distance);
    game_object_set_color(number, COLOR_BLOCK_WALL);
    game_object_set_speed(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, LEVEL_WIDTH/2, MAX_BORDER_HEIGHT, 1);
}

void _add_level_object(level_obj* obj) {
    switch (obj->type) {
        case LEVEL_OBJECT_JET_FLYING:
            _add_flying_jet(obj);
            break;
        case LEVEL_OBJECT_JET_STANDING:
            _add_jet(obj);
            break;
        case LEVEL_OBJECT_TANK:
            _add_tank(obj);
            break;
        case LEVEL_OBJECT_ROCKET:
            _add_rocket(obj);
            break;
        case LEVEL_OBJECT_FUELSILO:
            _add_fuel_silo(obj);
            break;
        case LEVEL_OBJECT_END:
            _add_end_level(obj);
            break;
    }
}

void _add_flying_jet(level_obj* obj) {
    int number = game_object_add(GAME_OBJECT_CUBE);
    int x = LEFT_BORDER+2*obj->x-1;

    game_object_set_pos(number, x, 0, _distance);
    game_object_set_color(number, COLOR_JET);
    game_object_set_speed(number, 0, 0, SPEED_ENEMY_JET);
    game_object_set_scl(number, 1, 1, 2);
}

void _add_jet(level_obj* obj) {
    int number = game_object_add(GAME_OBJECT_CUBE);
    int x = LEFT_BORDER+2*obj->x-1;

    game_object_set_pos(number, x, 0, _distance);
    game_object_set_color(number, COLOR_JET);
    game_object_set_speed(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, 1, 1, 2);
}

void _add_tank(level_obj* obj) {
    int number = game_object_add(GAME_OBJECT_CUBE);
    int x = LEFT_BORDER+2*obj->x-1;

    game_object_set_pos(number, x, 0, _distance);
    game_object_set_color(number, COLOR_TANK);
    game_object_set_speed(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, 1, 1, 2);
}

void _add_rocket(level_obj* obj) {
    int number = game_object_add(GAME_OBJECT_CUBE);
    int x = LEFT_BORDER+2*obj->x-1;

    game_object_set_pos(number, x, 0, _distance);
    game_object_set_color(number, COLOR_ROCKET);
    game_object_set_speed(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, 0.5, 2, 0.5);
}

void _add_fuel_silo(level_obj* obj) {
    int number = game_object_add(GAME_OBJECT_CUBE);
    int x = LEFT_BORDER+2*obj->x-1;
    
    game_object_set_pos(number, x, 0, _distance);
    game_object_set_color(number, COLOR_SILO);
    game_object_set_speed(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, 1.5, 3, 1.5);
}

void _add_end_level(level_obj* obj) {
    UNUSED_VAR(obj);
    
    int number = game_object_add(GAME_OBJECT_CUBE);
    int x = LEFT_BORDER;
    
    game_object_set_pos(number, x, 0, _distance);
    game_object_set_color(number, ARGB_MAGENTA);
    game_object_set_speed(number, 0, 0, 0);
    game_object_set_scl(number, 1, 20, 1);
    game_object_set_hidden(number, true);
}
