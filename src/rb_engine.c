//
//  rb_engine.c
//  3d wireframe game engine
//
//  Engine and gamne objects
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_engine.h"
#include "rb_primitive.h"
#include "rb_draw3d.h"
#include "rb_keys.h"
#include "rb_vtext.h"

#ifdef PITREX
#include "rb_pitrex.h"
#else
#include "rb_sdl.h"
#endif

int counter = 0;
game_object* list_of_objects[MAX_GAME_OBJECTS];
bool is_running = 0;
camera3d cam;
float fps = 0.0;
double t1 = 0;
double t2 = 0;
graphics* fb = 0;
setting settings;
game_fp target;

// platform externals
extern void platform_render(unsigned char* buffer, int width, int height);
extern double platform_get_ms(void);

extern void game_main(void);

void game_objects_update(float delta);
void game_objects_render(camera3d cam);
void game_objects_cleanup(void);

bool use_external_vline(void) {
    return target.draw_line == NULL ? false : true;
}

bool use_external_vtext(void) {
    return target.draw_vtext == NULL ? false : true;
}

void engine_draw_line_extern(float x1, float y1, float x2, float y2) {
    target.draw_line(x1, y1, x2, y2);
}

void engine_vtext_extern(float x, float y, char* str) {
    target.draw_vtext(x, y, str);
}

double engine_get_ticks(void) {
#ifdef PITREX
    return pitrex_get_ticks();
#else
    return SDL_GetTicks();
#endif
}

void engine_set_functions(game_fp fp) {
#ifdef PITREX
    fp.draw_line = pitrex_draw_line;
    fp.draw_vtext = pitrex_draw_vtext;
#else
    fp.draw_line = NULL;
    fp.draw_vtext = NULL;
#endif

	target = fp;
    
    t1 = engine_get_ticks();
}

void engine_init(void) {
#ifndef PITREX
    if (settings.fullscreen) {
		rblog("Goto fullscreen mode");

        sdl_set_fullscreen();
    }
#endif
    
    fb = graphics_open(settings.buffer_width, settings.buffer_width);
    draw_setgb(fb);

     // camera position/rotation
    cam.pos = vec3_make(0.0, 2.0, 0.0);
    cam.rot = vec3_make(0.0, 0.0, 0.0);
    cam.fov = 1.0;
    cam.distort = 0.0;

    target.init();

    is_running = true;
}

bool engine_handle_key(RBEvent event) {
    // Handle all game controls: Left,Right,Up,Down,0-9
    
    if (event.control) {
        return false;
    }
    
    bool down = event.type == RBEVT_KeyPressed ? true : false;
    
    switch (event.key_code) {
        // Game keys
        case RB_KEYCODE_LEFT:
            input_set_control(CONTROL_LEFT, down);
            return true;
        case RB_KEYCODE_RIGHT:
            input_set_control(CONTROL_RIGHT, down);
            return true;
        case RB_KEYCODE_UP:
            input_set_control(CONTROL_UP, down);
            return true;
        case RB_KEYCODE_DOWN:
            input_set_control(CONTROL_DOWN, down);
            return true;

        // Game buttons
        case RB_KEYCODE_1:
            input_set_control(BUTTON_1, down);
            return true;
        case RB_KEYCODE_2:
            input_set_control(BUTTON_2, down);
            return true;
        case RB_KEYCODE_3:
            input_set_control(BUTTON_3, down);
            return true;
        case RB_KEYCODE_4:
            input_set_control(BUTTON_4, down);
            return true;
            
        default:
            break;
    }
    
    return target.handle_key(event);
}

void engine_update(float delta) {
    game_objects_update(delta);
    target.update(delta, &cam);
}

void engine_render(camera3d cam) {
    target.pre_render(&cam);
    game_objects_render(cam);
    target.post_render(&cam);
}

void engine_after_render(void) {
    target.after_render();
}

void engine_cleanup(void) {
    game_objects_cleanup();
    target.cleanup();
}

int engine_step(void) {
    float delta = 0.0;
    t2 = engine_get_ticks();
    delta = t2-t1;
    fps = 1000.0 / delta;
    t1 = engine_get_ticks();

    engine_update(delta);
    engine_render(cam);
    
    char str[255];
    sprintf(str, "FPS: %.0f", fps);
    vtext_draw_string(560, 10, str, 1.0f);

    return 0;
}

int main(int argc, char *argv[]) {
    UNUSED_VAR(argc);
    UNUSED_VAR(argv);

	// Initialize available targets and select one
    game_main();

    settings.screen_width = 0;
    settings.screen_height = 0;
    settings.buffer_width = 0;
    settings.buffer_height = 0;
    settings.fullscreen = false;
    settings.fps = 60;

    target.settings(&settings);

#ifdef PITREX
    if (!pitrex_init()) {
        return -1;
    }

    engine_init();
    pitrex_run();
    engine_cleanup();

#else
    if (!sdl_init(settings.screen_width, settings.screen_height, settings.buffer_width, settings.buffer_height)) {
        return -1;
    }

    sdl_set_fps(settings.fps);
	
    engine_init();
    sdl_run();
    engine_cleanup();
    sdl_cleanup();
#endif

    return 0;
}

void game_objects_update(float delta) {
    for (int i = 0; i < counter; i++) {
        if (list_of_objects[i]->dead == 0) {
            list_of_objects[i]->pos.x += (list_of_objects[i]->speed.x * delta);
            list_of_objects[i]->pos.y += (list_of_objects[i]->speed.y * delta);
            list_of_objects[i]->pos.z += (list_of_objects[i]->speed.z * delta);

            list_of_objects[i]->rot.x += (list_of_objects[i]->rot_speed.x * delta);
            list_of_objects[i]->rot.y += (list_of_objects[i]->rot_speed.y * delta);
            list_of_objects[i]->rot.z += (list_of_objects[i]->rot_speed.z * delta);

            if (list_of_objects[i]->lifetime > 0) {
                list_of_objects[i]->life += delta;

                if (list_of_objects[i]->life > list_of_objects[i]->lifetime) {
                    list_of_objects[i]->dead = 1;
                }
            }
        }
    }
}

void _game_object_render(game_object* obj, camera3d cam) {
    // TODO: Change later, for now all are cubes
    draw_setcolor(obj->color);
    primitive_draw_cube(obj->pos, obj->scl, obj->rot, cam);
}

void game_objects_render(camera3d cam) {
    for (int i = 0; i < counter; i++) {
        if (list_of_objects[i]->dead == 0 && list_of_objects[i]->hidden == 0) {
            _game_object_render(list_of_objects[i], cam);
        }
    }
}

void game_objects_cleanup(void) {
    for (int i = 0; i < counter; i++) {
        free(list_of_objects[i]);
    }
    
    counter = 0;
}

int game_object_create(int type) {
    if (type < 0 || type >= GAME_OBJECT_LAST) {
        rblog_num1("Unknown game object type specified: ", type);
        return 0;
    }

    if (counter >= MAX_GAME_OBJECTS) {
        rblog_num1("Maximum of game objects reached: ", counter);
        return 0;
    }

    game_object* obj = (game_object*)malloc(sizeof(game_object));
    obj->type = type;
    obj->pos.x = 0.0; obj->pos.y = 0.0; obj->pos.z = 0.0;
    obj->scl.x = 1.0; obj->scl.y = 1.0; obj->scl.z = 1.0;
    obj->rot.x = 0.0; obj->rot.y = 0.0; obj->rot.z = 0.0;
    obj->color = ARGB_WHITE;
    obj->speed.x = 0.0; obj->speed.y = 0.0; obj->speed.z = 0.0;
    obj->rot_speed.x = 0.0; obj->rot_speed.y = 0.0; obj->rot_speed.z = 0.0;
    obj->hidden = 0;
    obj->dead = 0;
    obj->lifetime = 0;
    obj->life = 0;

    obj->id = counter + 1;
    list_of_objects[counter] = obj;
    counter++;

    return obj->id;
}

void game_object_set_pos(int id, float x, float y, float z, bool add) {
    if (id <= 0 || id > counter) {
        rblog_num1("This object id does not exist: ", id);
        return;
    }
    
    if (add) {
        list_of_objects[id - 1]->pos.x += x;
        list_of_objects[id - 1]->pos.y += y;
        list_of_objects[id - 1]->pos.z += z;
    }
    else {
        list_of_objects[id - 1]->pos.x = x;
        list_of_objects[id - 1]->pos.y = y;
        list_of_objects[id - 1]->pos.z = z;
    }
}

void game_object_set_scl(int id, float x, float y, float z, bool add) {
    if (id <= 0 || id > counter) {
        rblog_num1("This object id does not exist: ", id);
        return;
    }

    if (add) {
        list_of_objects[id - 1]->scl.x += x;
        list_of_objects[id - 1]->scl.y += y;
        list_of_objects[id - 1]->scl.z += z;
    }
    else {
        list_of_objects[id - 1]->scl.x = x;
        list_of_objects[id - 1]->scl.y = y;
        list_of_objects[id - 1]->scl.z = z;
    }
}

void game_object_set_rot(int id, float x, float y, float z, bool add) {
    if (id <= 0 || id > counter) {
        rblog_num1("This object id does not exist: ", id);
        return;
    }

    if (add) {
        list_of_objects[id - 1]->rot.x += x;
        list_of_objects[id - 1]->rot.y += y;
        list_of_objects[id - 1]->rot.z += z;
    }
    else {
        list_of_objects[id - 1]->rot.x = x;
        list_of_objects[id - 1]->rot.y = y;
        list_of_objects[id - 1]->rot.z = z;
    }
}

void game_object_set_speed(int id, float x, float y, float z) {
    if (id <= 0 || id > counter) {
        rblog_num1("This object id does not exist: ", id);
        return;
    }
    
    list_of_objects[id-1]->speed.x = x;
    list_of_objects[id-1]->speed.y = y;
    list_of_objects[id-1]->speed.z = z;
}

void game_object_set_rot_speed(int id, float x, float y, float z) {
    if (id <= 0 || id > counter) {
        rblog_num1("This object id does not exist: ", id);
        return;
    }
    
    list_of_objects[id-1]->rot_speed.x = x;
    list_of_objects[id-1]->rot_speed.y = y;
    list_of_objects[id-1]->rot_speed.z = z;
}

void game_object_set_color(int id, ARGB_color color) {
    if (id <= 0 || id > counter) {
        rblog_num1("This object id does not exist: ", id);
        return;
    }
    
    list_of_objects[id-1]->color = color;
}

void game_object_set_hidden(int id, bool flag) {
    if (id <= 0 || id > counter) {
        rblog_num1("This object id does not exist: ", id);
        return;
    }
    
    list_of_objects[id-1]->hidden = flag;
}

void game_object_set_dead(int id, bool flag) {
    if (id <= 0 || id > counter) {
        rblog_num1("This object id does not exist: ", id);
        return;
    }
    
    list_of_objects[id-1]->dead = flag;
}
