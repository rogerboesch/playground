//
//  rb_mesh.cpp
//  3d wireframe game engine: mesh handling
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_mesh.hpp"
#include "rb_math.hpp"
#include "rb_platform.h"
#include "rb_log.h"
#include "rb_file.hpp"

#include <fstream>
#include <strstream>
#include <algorithm>

bool Mesh::LoadObjectFile(std::string filename) {
    FileReader reader;
    if (!reader.Load(filename, "obj")) {
        return false;
    }

    std::vector<Vec3D> verts;

    while (!reader.Eof()) {
        std::string line = reader.ReadLine();
        if (!reader.Eof()) {
            std::strstream s;
            s << line;

            char junk;

            if (line[0] == 'v') {
                Vec3D v;
                s >> junk >> v.x >> v.y >> v.z;
                verts.push_back(v);
            }

            if (line[0] == 'f') {
                int f[3];
                s >> junk >> f[0] >> f[1] >> f[2];
                tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
            }
        }
    }
    
    RBLOG_NUM1("Model loaded (# of tris)", tris.size());

    return true;
}
