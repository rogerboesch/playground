//
//  rb_level.h
//  3d wireframe game engine
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#ifndef RB_LEVEL_H
#define RB_LEVEL_H

#include "rb_base.h"

// MARK: - Object data

#define LEVEL_OBJECT_JET_STANDING   1
#define LEVEL_OBJECT_JET_FLYING     2
#define LEVEL_OBJECT_TANK           3
#define LEVEL_OBJECT_ROCKET         4
#define LEVEL_OBJECT_FUELSILO       5
#define LEVEL_OBJECT_BULLET         6
#define LEVEL_OBJECT_END            10

typedef struct {
    int type;
    int x;
} level_obj;

// MARK: - Level data

#define MAX_OBJECTS_PER_LINE    20
#define MAX_LINES_PER_LEVEL     100

typedef struct level_line {
    int border;
    int object_count;
    level_obj objects[MAX_OBJECTS_PER_LINE];
} level_line;

typedef struct {
    int bullets;
    int fuel;
    int fuel_per_second;
    int fuel_fillup_per_silo;
    int score_jet;
    int score_jet_flying;
    int score_tank;
    int score_rocket;
    int score_silo;
    int current_line;
    int line_count;
    level_line lines[MAX_LINES_PER_LEVEL];
} level;

bool level_load_file(char* filename);
bool level_load_number(int number);
void level_rewind(void);

bool level_get_current_line(level_line** line);
bool level_get_line(int index, level_line** line);
bool level_has_more_lines(void);
int level_get_number_of_lines(void);

#endif
