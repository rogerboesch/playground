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
    int pitrex_init(const char* name, int width, int height);
    void pitrex_frame(void);
    void pitrex_handle_input(void);
    void pitrex_draw_line(float x1, float y1, float x2, float y2);
    void pitrex_draw_vtext(float x, float y, char* str);
    double pitrex_get_ticks(void);
}

char s_asset_path[1024];
int s_screen_height = 0;

void _store_asset_path() {
    const char* home_dir = getenv("HOME");
    sprintf(s_asset_path, "%s/RBProjects/", home_dir);

    RBLOG_STR1("Asset path", s_asset_path);
}

void _game_main() {
    vexxon_start();

    int quit = 0;
    while (!quit) {
        pitrex_frame();
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
    void platform_on_frame(float deltaTime) {
        game_update_controls(deltaTime);
    }

    void platform_on_init(const char* name, int width, int height) {
        s_screen_height = height;
        pitrex_init(name, width, height);
    }

    void platform_set_pixel(int x, int y, byte color, int brightness) { }

    void platform_draw_line(int x1, int y1, int x2, int y2, byte color, int invert) {
        // Must always be inverted on PiTrex platform
        y1 = s_screen_height - y1;
        y2 = s_screen_height - y2;

        pitrex_draw_line(x1, y1, x2, y2);
    }

    void vtext_draw_string(int x, int y, char* str, float scale) {
        pitrex_draw_vtext(x, y, str);
    }
    
    double platform_get_ms() {
        return pitrex_get_ticks();
    }

    char s_path[1024];
    const char* platform_resource_file_path(const char* filename, const char* extension) {
        sprintf(s_path, "%s%s.%s", s_asset_path, filename, extension);

        return s_path;
    }

}
