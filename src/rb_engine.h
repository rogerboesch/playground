//
//  rb_engine.h
//  3d wireframe game engine
//
//  Engine and gamne objects
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#ifndef RB_ENGINE_H
#define RB_ENGINE_H

#include "rb_base.h"
#include "rb_keys.h"
#include "rb_draw3d.h"
#include "rb_graphics.h"
#include "rb_input.h"
#include "rb_platform.h"

#define MAX_GAME_OBJECTS    200

#define GAME_OBJECT_CUBE    0
#define GAME_OBJECT_PLANE   1
#define GAME_OBJECT_LAST    2

#define ENGINE_FLAG_VECTOR_BASED 0
#define ENGINE_FLAGS_MAX 1

typedef struct {
    uint16_t screen_width, screen_height;
    uint16_t buffer_width, buffer_height;
    bool fullscreen;
    bool use_pixels;
    bool use_vtext;
    int fps;
} setting;

typedef struct {
    int id;
    int type;
    vec3 pos;
    vec3 scl;
    vec3 rot;
    ARGB_color color;
    vec3 speed;
    vec3 rot_speed;
    uint8_t hidden;
    uint8_t dead;
    float lifetime;
    float life;
} game_object;

typedef void (*fp_settings)(setting*);
typedef void (*fp_init)(void);
typedef void (*fp_update)(float, camera3d*);
typedef void (*fp_pre_render)(camera3d*);
typedef void (*fp_post_render)(camera3d*);
typedef void (*fp_after_render)(void);
typedef void (*fp_cleanup)(void);
typedef bool (*fp_handle_key)(RBEvent);

typedef void (*fp_draw_line)(float x1, float y1, float x2, float y2);
typedef void (*fp_draw_vtext)(float x, float y, char* str);

typedef struct {
	fp_settings settings;
	fp_init init;
	fp_update update;
	fp_pre_render pre_render;
	fp_post_render post_render;
	fp_after_render after_render;
	fp_cleanup cleanup;
	fp_handle_key handle_key;
    fp_draw_line draw_line;
    fp_draw_vtext draw_vtext;
} game_fp;

bool use_external_vline(void);
bool use_external_vtext(void);

void engine_set_functions(game_fp fp);
void engine_draw_line_extern(float x1, float y1, float x2, float y2);
void engine_vtext_extern(float x, float y, char* str);

void game_objects_update(float delta);
void game_objects_render(camera3d cam);
void engine_after_render(void);

int game_object_add(int type);
void game_object_set_pos(int id, float x, float y, float z);
void game_object_set_scl(int id, float x, float y, float z);
void game_object_set_rot(int id, float x, float y, float z);
void game_object_set_speed(int id, float x, float y, float z);
void game_object_set_rot_speed(int id, float x, float y, float z);
void game_object_set_color(int id, ARGB_color color);
void game_object_set_hidden(int id, bool flag);
void game_object_set_dead(int id, bool flag);

#endif
