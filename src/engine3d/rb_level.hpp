//
//  rb_level.cpp
//  3d wireframe game engine: level loading
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#pragma once

#include "rb_types.hpp"

#include <vector>
#include <string>

#define LEVEL_OBJECT_JET_STANDING   1
#define LEVEL_OBJECT_JET_FLYING     2
#define LEVEL_OBJECT_TANK           3
#define LEVEL_OBJECT_ROCKET         4
#define LEVEL_OBJECT_FUELSILO       5
#define LEVEL_OBJECT_BULLET         6
#define LEVEL_OBJECT_PLAYER_BULLET  7
#define LEVEL_OBJECT_LEVEL          8
#define LEVEL_OBJECT_END            10

// MARK: - Level data

struct LevelObject {
    int type;
    int x, y;
};

struct LevelLine {
    int border = 0;
    std::vector<LevelObject> gameObjects;
};

struct Level {
    int bullets = 0;
    int fuel = 0, fuel_per_second = 0, fuel_fillup_per_silo = 0;
    int score_jet = 0, score_jet_flying = 0;
    int score_tank = 0, score_rocket = 0, score_silo = 0;
    int currentLine = 0;
    std::vector<LevelLine> lines;

    bool LoadFile(std::string filename);
    bool LoadNumber(int number);

    bool GetLine(LevelLine& line);
    bool HasMoreLines();
    void Rewind();
    
private:
    void AddLine(int number, std::string data);
};
