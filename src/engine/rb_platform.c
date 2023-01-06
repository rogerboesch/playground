//
//  rb_platform.c
//  3d wireframe game engine
//
//  Platform specific code
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_base.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define _DEFAULT_SOURCE
#define __USE_MISC

#ifndef _WIN32 
    #include <unistd.h>
    #include <pwd.h>
    #include <dirent.h>
#endif

const char* platform_home_path(void);

// Message and log handling

char logpath[256];
int firstlog = 1;

void _rb_write_to_log(const char* str) {
	printf("%s", str);

	if (firstlog) {
		sprintf(logpath, "%s/classics-coder-log.txt", platform_home_path());
		firstlog = 0;

		printf(" >> Create log file %s\n", logpath);

		// Test for file
		FILE* file = fopen(logpath, "a+");
		if (file == NULL) {
			printf(" >> Can't create log file %s\n", logpath);
			return;
		}

		fprintf(file, "\n\n--- NEW LOG ----------------------------------------------------------\n\n");
		fprintf(file, "%s", str);
		fclose(file);
	}
	else {
		FILE* file = fopen(logpath, "a+");
		if (file == NULL) {
			printf(" >> Can't add to log file %s\n", logpath);
			return;
		}

		fprintf(file, "%s", str);
		fclose(file);
	}
}

void _rb_write_user_message(const char* str) {
}

void rb_message(const char* msg, bool user, bool cr) {
	_rb_write_to_log(msg);
	if (cr == 1) {
		_rb_write_to_log("\n");
	}

	if (user != 1) {
		return;
	}

	_rb_write_user_message(msg);
	if (cr == 1) {
		_rb_write_user_message("\n");
	}
}

void rb_message_str1(const char* msg, const char* str1, bool user, bool cr) {
	char str[1000];
	sprintf(str, "%s%s", msg, str1);

	rb_message(str, user, cr);
}

void rb_message_num1(const char* msg, int num1, bool user, bool cr) {
	char str[1000];
	sprintf(str, "%s%d", msg, num1);

	rb_message(str, user, cr);
}

// Path and file handling
char app_folder[256];

const char* platform_home_path(void) {
    const char* dir;

#ifdef _WIN32 
    dir = getenv("USERPROFILE");
#else
    if ((dir = getenv("HOME")) == NULL) {
        dir = getpwuid(getuid())->pw_dir;
    }
#endif

    return dir;
}

int platform_create_folder(const char* name) {
    struct stat st = {0};

    char path[256];
    sprintf(path, "%s/%s", platform_home_path(), name);

    if (stat(path, &st) == -1) {
        mkdir(path, 0700);
        return 1;
    }

    return 0;
}

int platform_set_app_folder(const char* name) {
#if _WIN32
    sprintf(app_folder, "%s\\%s\\", platform_home_path(), name);
#else
    sprintf(app_folder, "%s/%s/", platform_home_path(), name);
#endif
    return platform_create_folder(name);
}

const char* platform_get_app_folder(void) {
    return app_folder;
}

int platform_list_files(const char* path, const char* ext) {
#ifdef _WIN32 
    printf("platform_list_files() not yet implemented\n");
    return 0;
#else
    struct dirent *dir;

    DIR *d = opendir(path);
    char full_path[1000];
    
    int file_count = 0;

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {
                if (ext == NULL || (ext != NULL && strstr(dir->d_name, ext))) {
                    full_path[0]='\0';

                    strcat(full_path,path);
                    strcat(full_path,"/");
                    strcat(full_path,dir->d_name);

					rbprint_num(file_count);
					rbprint("> ");
					rbprint_newline(full_path);
					
                    file_count++;
                }
            }
        }

        closedir(d);
    }

    return file_count;     
#endif
}

char _platform_get_file_name[256];
const char* platform_get_file(int index, const char* path, const char* ext) {
#ifdef _WIN32 
    printf("platform_get_file() not yet implemented\n");
    return NULL;
#else
    struct dirent *dir;

    DIR *d = opendir(path);
    char full_path[1000];
    
    int file_count = 0;

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {
                if (ext == NULL || (ext != NULL && strstr(dir->d_name, ext))) {
                    full_path[0]='\0';

                    strcat(full_path, path);
                    strcat(full_path,"/");
                    strcat(full_path, dir->d_name);

					rbprint_num(file_count);
					rbprint("> ");
					rbprint_newline(full_path);

                    if (index == file_count) {
                        sprintf(_platform_get_file_name, "%s", dir->d_name);
                        return _platform_get_file_name;
                    }
    
                    file_count++;
                }
            }
        }

        closedir(d);
    }

    return NULL;     
#endif
}
