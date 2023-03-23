//
//  rb_platform.h
//  3d wireframe game engine
//
//  Platform specific code
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#ifndef RB_PLATFORM_H
#define RB_PLATFORM_H

#ifdef __cplusplus
    extern "C" {
#endif

const char* platform_home_path(void);

int platform_create_folder(const char* name);
int platform_set_app_folder(const char* name);
const char* platform_get_app_folder(void);
int platform_list_files(const char* path, const char* ext);
const char* platform_get_file(int index, const char* path, const char* ext);

#ifdef __cplusplus
    }
#endif

#endif
