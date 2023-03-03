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
#include "rb_graphics.h"

#include <math.h>

#define SECTION_TIME         2.0    // Time when next section gets created

#define LEVEL_WIDTH          20     // Width of field
#define LEFT_BORDER         -10     // Right Border
#define RIGHT_BORDER         10     // Left Border

#define FLIGHT_HEIGHT_LIMIT  5.0f
#define FLIGHT_WIDTH_LIMIT   6.0f
#define FLIGHT_FLOOR_LIMIT   0.1f
#define PLAYER_MOVEMENT      10.0f

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
#define COLOR_INFO          ARGB_YELLOW

void restart(camera3d* cam);

void _add_next_section(float delta);
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

static int _distance = START_DISTANCE;      // Actual distance since start
static int _space_area = -1;                // Here space starts (end of floor)
static float _border_delay_time = 0.0f;     // Delay time since last section creation
static int _section_count = -1;             // Number of sections already created

float _speed = 0.0f;                        // Current speed
float _accel = 0.3f;                        // Acceleration used
float _moving = 0.1f;                       // Moving acceleration used

static int player_id = 0;
static float player_speed = 4.0f;
static float player_movement = 10.0f;
static float camera_x_offset = 0.0f;

void vexxon_target_settings(setting* setting) {
    setting->screen_width = 330;
    setting->screen_height = 440;
    setting->buffer_width = 330;
    setting->buffer_height = 440;
    setting->fullscreen = false;
}

void vexxon_target_init(camera3d* cam) {
    platform_set_app_folder("vexxon");

    if (!level_load_number(1)) {
        return;
    }

    _distance = START_DISTANCE;
    _space_area = -1;
    int count = 0;

#if 0
    // Dont create entire level in init
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
#endif

    // Player/Space fighter
    player_id = game_object_create(GAME_OBJECT_CUBE);
    game_object_set_pos(player_id, 0.0f, 0.0f, 10.0f, COORDINATE_IS_ABSOLUTE);
    game_object_set_velocity(player_id, 0, 0, player_speed);

#if 0
    // Follow
    cam->pos = vec3_make(0.0f, 2.0f, 0.0f);
    cam->rot = vec3_make(0.0f, 0.0f, 0.0f);
#else
    // ISO view
    cam->pos = vec3_make(7.82f, 4.37f, 1.82f);
    cam->rot = vec3_make(0.0f, -0.43f, 0.0f);
#endif

    // Get camera offset for later update
    vec3 pos = game_object_get_position(player_id);
    camera_x_offset = pos.x - cam->pos.x;

    return;
}

void vexxon_target_update(float delta, camera3d* cam) {
    // Instead we create here the next section when we need it
    _add_next_section(delta);

    // End of level?
    vec3 player_pos = game_object_get_position(player_id);

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

    player_pos = game_object_get_position(player_id);
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

void vexxon_target_pre_render(camera3d* cam) {
    graphics_fill(draw_getgb(), ARGB_BLACK);

    // Draw horizontal line
    pixel res = draw_getres();

    draw_setcolor(ARGB_GREY);
    double hpos = (0.5 + tan(cam->rot.x) * cam->fov)* res.y;
    draw_line((vec2) { 0.0, hpos }, (vec2) { res.x, hpos });

    draw_setcolor(COLOR_INFO);
    vtext_draw_string(10, 40, "ARROW LEFT   MOVE LEFT", 0.5f);
    vtext_draw_string(10, 50, "ARROW RIGHT  MOVE RIGHT", 0.5f);
    vtext_draw_string(10, 60, "ARROW UP     MOVE FORWARD", 0.5f);
    vtext_draw_string(10, 70, "ARROW DOWN   MOVE BACK", 0.5f);
}

void vexxon_target_post_render(camera3d* cam) {
    UNUSED_VAR(cam);

    draw_setcolor(ARGB_WHITE);
    vtext_draw_string(10, 10, "VEXXON", 1.0f);
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

void restart(camera3d* cam) {
    game_object_set_pos(player_id, 0.0f, 1.0f, 10.0f, COORDINATE_IS_ABSOLUTE);
    cam->pos = vec3_make(7.82f, 4.37f, 1.82f);
}

void _add_next_section(float delta) {
    _border_delay_time += delta;

    if (_border_delay_time >= SECTION_TIME) {
        if (level_has_more_lines()) {
            level_line* line;
            level_get_current_line(&line);

            _create_section(line);

            _distance += 10;

            if (_space_area == -1 && line->border == 0) {
                _space_area = _section_count;
            }

            _section_count++;
        }

        _border_delay_time = 0.0f;
    }
}

void _create_section(level_line* line) {
    int height = line->border - 2;

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
        _add_right_border_cube(height + 1);
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
    int number = game_object_create(GAME_OBJECT_CUBE);

    game_object_set_pos(number, LEFT_BORDER, 0, _distance, false);
    game_object_set_color(number, COLOR_WALL);
    game_object_set_velocity(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, 1, height, 1, false);
}

void _add_right_border_cube(int height) {
    int number = game_object_create(GAME_OBJECT_CUBE);

    game_object_set_pos(number, RIGHT_BORDER, 0, _distance, false);
    game_object_set_color(number, COLOR_WALL);
    game_object_set_velocity(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, 1, height, 1, false);

    if (height == 0) {
        game_object_set_scl(number, 1, 0.2, 1, false);
    }
}

void _add_top_wall(int height) {
    int number = game_object_create(GAME_OBJECT_CUBE);

    game_object_set_pos(number, 0, MAX_BORDER_HEIGHT - height, _distance, false);
    game_object_set_color(number, COLOR_BLOCK_WALL);
    game_object_set_velocity(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, LEVEL_WIDTH - 1, height, 1, false);
}

void _add_bottom_wall(int height) {
    int number = game_object_create(GAME_OBJECT_CUBE);

    game_object_set_pos(number, 0, 0, _distance, false);
    game_object_set_color(number, COLOR_BLOCK_WALL);
    game_object_set_velocity(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, LEVEL_WIDTH - 1, height, 1, false);
}

void _add_left_wall(void) {
    int number = game_object_create(GAME_OBJECT_CUBE);

    game_object_set_pos(number, -LEVEL_WIDTH / 4 + 0.5, 0, _distance, false);
    game_object_set_color(number, COLOR_BLOCK_WALL);
    game_object_set_velocity(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, LEVEL_WIDTH / 2, MAX_BORDER_HEIGHT, 1, false);
}

void _add_right_wall(void) {
    int number = game_object_create(GAME_OBJECT_CUBE);

    game_object_set_pos(number, LEVEL_WIDTH / 4 - 0.5, 0, _distance, false);
    game_object_set_color(number, COLOR_BLOCK_WALL);
    game_object_set_velocity(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, LEVEL_WIDTH / 2, MAX_BORDER_HEIGHT, 1, false);
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
    int number = game_object_create(GAME_OBJECT_CUBE);
    int x = LEFT_BORDER + 2 * obj->x - 1;

    game_object_set_pos(number, x, 0, _distance, false);
    game_object_set_color(number, COLOR_JET);
    game_object_set_velocity(number, 0, 0, SPEED_ENEMY_JET);
    game_object_set_rot_velocity(number, -1, 0, 0);
    game_object_set_scl(number, 1, 1, 2, false);
}

void _add_jet(level_obj* obj) {
    int number = game_object_create(GAME_OBJECT_CUBE);
    int x = LEFT_BORDER + 2 * obj->x - 1;

    game_object_set_pos(number, x, 0, _distance, false);
    game_object_set_color(number, COLOR_JET);
    game_object_set_velocity(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, 1, 1, 2, false);
}

void _add_tank(level_obj* obj) {
    int number = game_object_create(GAME_OBJECT_CUBE);
    int x = LEFT_BORDER + 2 * obj->x - 1;

    game_object_set_pos(number, x, 0, _distance, false);
    game_object_set_color(number, COLOR_TANK);
    game_object_set_velocity(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, 1, 1, 2, false);
}

void _add_rocket(level_obj* obj) {
    int number = game_object_create(GAME_OBJECT_CUBE);
    int x = LEFT_BORDER + 2 * obj->x - 1;

    game_object_set_pos(number, x, 0, _distance, false);
    game_object_set_color(number, COLOR_ROCKET);
    game_object_set_velocity(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, 0.5, 2, 0.5, false);
}

void _add_fuel_silo(level_obj* obj) {
    int number = game_object_create(GAME_OBJECT_CUBE);
    int x = LEFT_BORDER + 2 * obj->x - 1;

    game_object_set_pos(number, x, 0, _distance, false);
    game_object_set_color(number, COLOR_SILO);
    game_object_set_velocity(number, 0, 0, SPEED_GROUND);
    game_object_set_scl(number, 1.5, 3, 1.5, false);
}

void _add_end_level(level_obj* obj) {
    UNUSED_VAR(obj);

    int number = game_object_create(GAME_OBJECT_CUBE);
    int x = LEFT_BORDER;

    game_object_set_pos(number, x, 0, _distance, false);
    game_object_set_color(number, ARGB_MAGENTA);
    game_object_set_velocity(number, 0, 0, 0);
    game_object_set_scl(number, 1, 20, 1, false);
    game_object_set_hidden(number, true);
}
