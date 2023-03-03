//
//  rb_level.c
//  3d wireframe game engine
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_level.h"
#include "rb_platform.h"

#include <string.h>

#ifdef _WIN32
size_t getline(char** lineptr, size_t* n, FILE* stream) {
    char* bufptr = NULL;
    char* p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL) {
        return -1;
    }
    if (stream == NULL) {
        return -1;
    }
    if (n == NULL) {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }
    if (bufptr == NULL) {
        bufptr = malloc(128);
        if (bufptr == NULL) {
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while (c != EOF) {
        if ((p - bufptr) > (size - 1)) {
            size = size + 128;
            bufptr = realloc(bufptr, size);
            if (bufptr == NULL) {
                return -1;
            }
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}
#endif

extern char* platform_bundle_file_path(const char* filename, const char* extension);

void _parse_line(char* line);

level level_data;

bool level_load_file(char* filename) {
    char path[256];
#ifdef PITREX
    sprintf(path, "assets/%s.txt", filename);
#else
    sprintf(path, "%s%s.txt", platform_get_app_folder(), filename);
#endif

    rbmsg_str1("Load level file ", path);
 
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    size_t read;

    fp = fopen(path, "r");
    if (fp == NULL) {
        rbmsg_str1("Can't read level file: ", filename);
        return false;
    }
    
    while ((read = getline(&line, &len, fp)) != -1) {
        if (read < 1) {
            rblog_num1("Empty line read: ", (int)read);
        }
        else {
            _parse_line(line);
        }

        free(line);
        line = NULL;
    }
    
    fclose(fp);
    
    // Start with last line
    level_data.current_line = level_data.line_count;

    rblog_num1("Lines loaded in level: ", level_data.line_count);

    return true;
}

bool level_load_number(int number) {
    char filename[256];
    sprintf(filename, "level%d", number);
    
    return level_load_file(filename);
}

void level_rewind(void) {
    level_data.current_line = level_data.line_count;
}

bool level_get_current_line(level_line** line) {
    if (!level_has_more_lines()) {
        return false;
    }

    rblog_num1("Get level line: ", level_data.current_line);
    
    level_data.current_line--;
    *line = &(level_data.lines[level_data.current_line]);
    
    return true;
}

bool level_get_line(int index, level_line** line) {
    if (index < 0 || index >= level_data.line_count) {
        return false;
    }

    *line = &(level_data.lines[index]);
    return true;
}

bool level_has_more_lines(void) {
    return level_data.current_line > 0 ? true : false;
}

int level_get_number_of_lines(void) {
    return level_data.line_count;
}

void _add_line_objects(level_line* line, char* chn) {
    line->object_count = 0;

    for (int i = 0; i < 10; i++) {
        char ch = chn[i];
        
        level_obj obj;
        obj.type = -1;

        switch (ch) {
            // jet (standing)
            case 'j':
                obj.type = LEVEL_OBJECT_JET_STANDING;
                break;
            // jet (flying)
            case 'J':
                obj.type = LEVEL_OBJECT_JET_FLYING;
                break;
            // tank
            case 't':
                obj.type = LEVEL_OBJECT_TANK;
                break;
            // rocket
            case 'r':
                obj.type = LEVEL_OBJECT_ROCKET;
                break;
            // fuel silo
            case 'f':
                obj.type = LEVEL_OBJECT_FUELSILO;
                break;
            case '@':
                obj.type = LEVEL_OBJECT_END;
                break;
            case '.':
                // Marker
                break;
            case '-':
                // Marker
                break;
            default:
                rblog_num1("Invalid character in level line: ", ch);
                return;
        }
        
        if (obj.type != -1) {
            obj.x = i+1;
            line->objects[line->object_count] = obj;
            line->object_count++;
        }
    }
}

void _add_line(char* str) {
    int num;
    char ch, space;
    char chn[10];

    int result = sscanf(str, "%c%c%c%c%c%c%c%c%c%c%c%c", &ch, &space, &chn[0], &chn[1], &chn[2], &chn[3], &chn[4], &chn[5], &chn[6], &chn[7], &chn[8], &chn[9]);

    if (result != 12) {
        rbmsg_str1("Level line format is invalid: ", str);
        return;
    }

    level_line line;

    switch (ch) {
        case 'X':
            line.border = 10;
            break;
        case 'Y':
            line.border = 11;
            break;
        case 'Z':
            line.border = 12;
            break;
        default:
            result = sscanf(str, "%i%c%c%c%c%c%c%c%c%c%c%c", &num, &space, &chn[0], &chn[1], &chn[2], &chn[3], &chn[4], &chn[5], &chn[6], &chn[7], &chn[8], &chn[9]);

            if (result != 12) {
		        rbmsg_str1("Level line format is invalid: ", str);
                return;
            }
            
            line.border = num;
    }

    _add_line_objects(&line, &chn[0]);
    
    level_data.lines[level_data.line_count] = line;
    level_data.line_count++;
}

void _parse_line(char* str) {
    char ch;
    int num1, num2, num3, num4, num5;
    
    int result = sscanf(str, "%c", &ch);

    if (result != 1) {
        rblog_str1("Line does not contain a character at first position: ", str);
        return;
    }

    switch (ch) {
        case '#':
            // Comment only, ignore
            break;
        case 'B':
            result = sscanf(str, "%c%i", &ch, &num1);
            if (result != 2) {
                rblog_str1("Line format is invalid: ", str);
                return;
            }

            level_data.bullets = num1;
            break;
        case 'F':
            result = sscanf(str, "%c%i%i%i", &ch, &num1, &num2, &num3);
            if (result != 4) {
                rblog_str1("Line format is invalid: ", str);
                return;
            }
            
            level_data.fuel = num1;
            level_data.fuel_per_second = num2;
            level_data.fuel_fillup_per_silo = num3;
            break;
        case 'S':
            result = sscanf(str, "%c%i%i%i%i%i", &ch, &num1, &num2, &num3, &num4, &num5);
            if (result != 6) {
                rblog_str1("Line format is invalid: ", str);
                return;
            }
            
            level_data.score_jet_flying = num1;
            level_data.score_jet = num2;
            level_data.score_tank = num3;
            level_data.score_rocket = num4;
            level_data.score_silo = num5;
            break;
        default:
            _add_line(str);
            break;
    }
}
