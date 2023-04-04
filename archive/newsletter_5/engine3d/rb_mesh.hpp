//
//  rb_mesh.hpp
//  3d wireframe game engine: mesh handling
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#pragma once

#include "rb_types.hpp"

#include <vector>
#include <string>

// MARK: - Mesh data

struct Mesh {
    std::vector<Triangle> tris;
    byte color;
    
    bool LoadObjectFile(std::string filename);
};
