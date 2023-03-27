//
//  rb_pitrex_main.cpp
//
//  Main PiTrex integration
//
//  04-08-2022, created by Roger Boesch
//  Copyright © 2022 by Roger Boesch - use only with permission
//

#include "rb_base.h"
#include "rb_log.h"
#include "rb_engine.hpp"
#include "rb_platform.h"

extern "C" {
    int vexxon_start();
    int vexxon_frame();
    int vexxon_stop();
    void game_update_controls(float deltaTime);
}

extern "C" {
    void pitrex_frame(void);
    void pitrex_handle_input(void);
    void pitrex_draw_line(float x1, float y1, float x2, float y2);
    void pitrex_draw_vtext(float x, float y, char* str);
}

char s_asset_path[1024];

void _store_asset_path() {
    const char* home_dir = getenv("HOME");
    sprintf(s_asset_path, "%s/RBProjects/", home_dir);

    RBLOG_STR1("Asset path", s_asset_path);
}

void _game_main() {
    pitrex_frame();

    vexxon_start();

    int quit = 0;
    while (!quit) {
        pitrex_handle_input();

        if (vexxon_frame() == 0) 
            quit = 1;
    }

    vexxon_stop();

    return;
}

int main(int argc, char *argv[]) {
    UNUSED_VAR(argc);
    UNUSED_VAR(argv);

    _store_asset_path();
    _game_main();

    return 0;
}

// Bind platform to Pitrex calls

extern "C" {
    void platform_set_pixel(int x, int y, byte color, int brightness) { }

    void platform_draw_line(int x1, int y1, int x2, int y2, byte color, int invert) {
        if (invert == INVERT_ON) {
            y1 = _screen_height - y1;
            y2 = _screen_height - y2;
        }

        pitrex_draw_line(xy, y1, x2, y2);
    }

    void platform_on_frame(float deltaTime) {
        game_update_controls(deltaTime);
    }

    void vtext_draw_string(int x, int y, char* str, float scale) {
        pitrex_draw_vtext(x, y, str);
    }
}
