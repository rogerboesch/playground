//
//  rb_level.cpp
//  3d wireframe game engine: level loading
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_level.hpp"
#include "rb_math.hpp"
#include "rb_platform.h"
#include "rb_log.h"
#include "rb_file.hpp"

#include <fstream>
#include <strstream>
#include <algorithm>

#define DATA_LENGTH 10  // Level width

void Level::AddLine(int number, std::string data) {
    LevelLine line = {};
    line.border = number;
    
    for (int i = 0; i < data.size(); i++) {
        char ch = data.at(i);
        
        LevelObject object = {};
        object.x = i+1;

        switch (ch) {
            // jet (standing)
            case 'j':
                object.type = LEVEL_OBJECT_JET_STANDING;
                break;
            // jet (flying)
            case 'J':
                object.type = LEVEL_OBJECT_JET_FLYING;
                break;
            // tank
            case 't':
                object.type = LEVEL_OBJECT_TANK;
                break;
            // rocket
            case 'r':
                object.type = LEVEL_OBJECT_ROCKET;
                break;
            // fuel silo
            case 'f':
                object.type = LEVEL_OBJECT_FUELSILO;
                break;
            case '@':
                object.type = LEVEL_OBJECT_END;
                break;
            default:
                continue;
        }
        
        line.gameObjects.push_back(object);
    }
    
    lines.push_back(line);
}

bool Level::LoadNumber(int number) {
    char temp[256];
    sprintf(temp, "level%d", number);
    
    return LoadFile(temp);
}

bool Level::LoadFile(std::string filename) {
    FileReader reader;
    if (!reader.Load(filename, "txt")) {
        return false;
    }

    std::vector<Vec3D> verts;

    int line_number = 0;
    while (!reader.Eof()) {
        std::string line = reader.ReadLine();
        if (!reader.Eof()) {
            line_number++;

            std::strstream s;
            s << line;

            char junk;

            // Scoring
            if (line[0] == 'B') {
                int number;

                s >> junk >> number;
                bullets = number;
                
                RBLOG_NUM1(" Bullets", bullets);
            }
            else if (line[0] == 'F') {
                int number, fps, fillup;

                s >> junk >> number >> fps >> fillup;
                fuel = number;
                fuel_per_second = fps;
                fuel_fillup_per_silo = fillup;

                RBLOG_NUM1(" Fuel  ", fuel);
                RBLOG_NUM1(" Usage ", fuel_per_second);
                RBLOG_NUM1(" Fillup", fuel_fillup_per_silo);
            }
            else if (line[0] == 'S') {
                int score1, score2, score3, score4, score5;

                s >> junk >> score1 >> score2 >> score3 >> score4 >> score5;
                
                score_jet_flying = score1;
                score_jet = score2;
                score_tank = score3;
                score_rocket = score4;
                score_silo = score5;

                RBLOG(" Score");
                RBLOG_NUM1("Jet (flying)", score_jet_flying);
                RBLOG_NUM1("Jet         ", score_jet);
                RBLOG_NUM1("Tank        ", score_tank);
                RBLOG_NUM1("Rocket      ", score_rocket);
                RBLOG_NUM1("Silo        ", score_silo);
            }

            // Level data with walls
            if (line[0] == '0' ||line[0] == '1' || line[0] == '2' || line[0] == '3' || line[0] == '4' ||
                line[0] == '5' || line[0] == '6' || line[0] == '7' || line[0] == '8' || line[0] == '9') {
                int number;
                std::string data;
                s >> number >> data;
                
                if (data.length() != DATA_LENGTH) {
                    RBLOG_NUM1("Error in level file, wrong length in line", line_number);
                    return false;
                }
                
                AddLine(number, data);
            }
            else if (line[0] == 'X' || line[0] == 'Y' || line[0] == 'Z') {
                // Level data with special walls
                char special;
                std::string data;
                s >> special >> data;

                int number = 0;
                
                if (special == 'X')
                    number = 10;
                else if (special == 'Y')
                    number = 11;
                else if (special == 'Z')
                    number = 12;
                else {
                    RBLOG_NUM1("Error in level file, wrong special character in line", line_number);
                    return false;
                }

                if (data.length() != DATA_LENGTH) {
                    RBLOG_NUM1("Error in level file, wrong length in line", line_number);
                    return false;
                }
                
                AddLine(number, data);
            }
        }
    }

    // Start with last line
    currentLine = (int)lines.size();

    RBLOG_NUM1("Level loaded lines", lines.size());
    
    return true;
}

bool Level::GetLine(LevelLine& line) {
    if (!HasMoreLines()) {
        return false;
    }

    RBLOG_NUM1("Get level line", currentLine);
    line = lines[--currentLine];
    
    return true;
}

bool Level::HasMoreLines() {
    return currentLine > 0 ? true : false;
}

void Level::Rewind() {
    // Start with last line
    currentLine = (int)lines.size();
}
