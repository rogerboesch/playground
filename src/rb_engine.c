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
    cam.pos = vec3_make(0.0f, 0.0f, 0.0f);
    cam.rot = vec3_make(0.0f, 0.0f, 0.0f);
    cam.fov = 1.0f;

    target.init(&cam);

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
            // Game buttons

        // Keys
        case RB_KEYCODE_a:
            input_set_control(KEY_A, down);
            return true;
        case RB_KEYCODE_b:
            input_set_control(KEY_B, down);
            return true;
        case RB_KEYCODE_c:
            input_set_control(KEY_C, down);
            return true;
        case RB_KEYCODE_d:
            input_set_control(KEY_D, down);
            return true;
        case RB_KEYCODE_e:
            input_set_control(KEY_E, down);
            return true;
        case RB_KEYCODE_f:
            input_set_control(KEY_F, down);
            return true;
        case RB_KEYCODE_g:
            input_set_control(KEY_G, down);
            return true;
        case RB_KEYCODE_h:
            input_set_control(KEY_H, down);
            return true;
        case RB_KEYCODE_i:
            input_set_control(KEY_I, down);
            return true;
        case RB_KEYCODE_j:
            input_set_control(KEY_J, down);
            return true;
        case RB_KEYCODE_k:
            input_set_control(KEY_K, down);
            return true;
        case RB_KEYCODE_l:
            input_set_control(KEY_L, down);
            return true;
        case RB_KEYCODE_m:
            input_set_control(KEY_M, down);
            return true;
        case RB_KEYCODE_n:
            input_set_control(KEY_N, down);
            return true;
        case RB_KEYCODE_o:
            input_set_control(KEY_O, down);
            return true;
        case RB_KEYCODE_p:
            input_set_control(KEY_P, down);
            return true;
        case RB_KEYCODE_q:
            input_set_control(KEY_Q, down);
            return true;
        case RB_KEYCODE_r:
            input_set_control(KEY_R, down);
            return true;
        case RB_KEYCODE_s:
            input_set_control(KEY_S, down);
            return true;
        case RB_KEYCODE_t:
            input_set_control(KEY_T, down);
            return true;
        case RB_KEYCODE_u:
            input_set_control(KEY_U, down);
            return true;
        case RB_KEYCODE_v:
            input_set_control(KEY_V, down);
            return true;
        case RB_KEYCODE_w:
            input_set_control(KEY_W, down);
            return true;
        case RB_KEYCODE_x:
            input_set_control(KEY_X, down);
            return true;
        case RB_KEYCODE_y:
            input_set_control(KEY_Y, down);
            return true;
        case RB_KEYCODE_z:
            input_set_control(KEY_Z, down);
            return true;
        case RB_KEYCODE_SPACE:
            input_set_control(KEY_SPACE, down);
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
    float delta = 0.016f;

#ifndef PITREX
    t2 = engine_get_ticks();
    delta = t2-t1;
    fps = 1000.0 / delta;
    t1 = engine_get_ticks();

    delta = delta / 1000.0f;
#endif

    engine_update(delta);
    engine_render(cam);

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
    if (!pitrex_init(settings.screen_width, settings.screen_height)) {
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
            list_of_objects[i]->pos.x += (list_of_objects[i]->velocity.x * delta);
            list_of_objects[i]->pos.y += (list_of_objects[i]->velocity.y * delta);
            list_of_objects[i]->pos.z += (list_of_objects[i]->velocity.z * delta);

            list_of_objects[i]->rot.x += (list_of_objects[i]->rot_velocity.x * delta);
            list_of_objects[i]->rot.y += (list_of_objects[i]->rot_velocity.y * delta);
            list_of_objects[i]->rot.z += (list_of_objects[i]->rot_velocity.z * delta);

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

    if (obj == NULL) {
        rblog_num1("Cant allocate memory for object %d\n", counter+1);
        return 0;
    }

    obj->type = type;
    obj->pos.x = 0.0; obj->pos.y = 0.0; obj->pos.z = 0.0;
    obj->scl.x = 1.0; obj->scl.y = 1.0; obj->scl.z = 1.0;
    obj->rot.x = 0.0; obj->rot.y = 0.0; obj->rot.z = 0.0;
    obj->color = ARGB_WHITE;
    obj->velocity.x = 0.0; obj->velocity.y = 0.0; obj->velocity.z = 0.0;
    obj->rot_velocity.x = 0.0; obj->rot_velocity.y = 0.0; obj->rot_velocity.z = 0.0;
    obj->hidden = 0;
    obj->dead = 0;
    obj->lifetime = 0;
    obj->life = 0;

    obj->id = counter + 1;
    list_of_objects[counter] = obj;
    counter++;

    return obj->id;
}

void game_object_set_pos(int id, float x, float y, float z, bool relative) {
    if (id <= 0 || id > counter) {
        rblog_num1("This object id does not exist: ", id);
        return;
    }

    if (relative) {
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

vec3 game_object_get_position(int id) {
    vec3 pos = { 0.0f, 0.0f, 0.0f };

    if (id <= 0 || id > counter) {
        rblog_num1("This object id does not exist: ", id);
        return pos;
    }

    return list_of_objects[id - 1]->pos;
}

void game_object_set_scl(int id, float x, float y, float z, bool relative) {
    if (id <= 0 || id > counter) {
        rblog_num1("This object id does not exist: ", id);
        return;
    }

    if (relative) {
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

void game_object_set_rot(int id, float x, float y, float z, bool relative) {
    if (id <= 0 || id > counter) {
        rblog_num1("This object id does not exist: ", id);
        return;
    }

    if (relative) {
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

void game_object_set_velocity(int id, float x, float y, float z) {
    if (id <= 0 || id > counter) {
        rblog_num1("This object id does not exist: ", id);
        return;
    }
    
    list_of_objects[id-1]->velocity.x = x;
    list_of_objects[id-1]->velocity.y = y;
    list_of_objects[id-1]->velocity.z = z;
}

void game_object_set_rot_velocity(int id, float x, float y, float z) {
    if (id <= 0 || id > counter) {
        rblog_num1("This object id does not exist: ", id);
        return;
    }
    
    list_of_objects[id-1]->rot_velocity.x = x;
    list_of_objects[id-1]->rot_velocity.y = y;
    list_of_objects[id-1]->rot_velocity.z = z;
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

void print_camera_data(camera3d* cam) {
    printf("Camera: %f,%f,%f - %f,%f,%f : %f\n", cam->pos.x, cam->pos.y, cam->pos.z, cam->rot.x, cam->rot.y, cam->rot.z, cam->fov);
}

void print_object_data(int id) {
    if (id <= 0 || id > counter) {
        rblog_num1("This object id does not exist: ", id);
        return;
    }

    game_object* obj = list_of_objects[id - 1];
    printf("Object %d: P %.02f,%0.2f,%0.2f-S %0.2f,%0.2f,%0.2f-R %0.2f,%0.2f,%0.2f-V %0.2f,%0.2f,%0.2f-H:%d-D:%d\n", obj->id, obj->pos.x, obj->pos.y, obj->pos.z, obj->scl.x, obj->scl.y, obj->scl.z, obj->rot.x, obj->rot.y, obj->rot.z, obj->velocity.x, obj->velocity.y, obj->velocity.z, obj->hidden, obj->dead);
}