//
//  rb_object.cpp
//  3d wireframe game engine: game object
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_object.hpp"
#include "rb_log.h"

#include <float.h>

static int object_counter = 0;
Mesh* GameObject::s_cube = nullptr;

GameObject::GameObject(int type, int tag) {
    Initialise();

    _type = type;
    _tag = tag;
    
    if (_type == GAME_OBJECT_TYPE_CUBE) {
        if (s_cube == nullptr) {
            s_cube = new Mesh();
            s_cube->tris = PrimtiveGetCube();
        }
    
        _mesh = s_cube;
    }
    else if (_type == GAME_OBJECT_TYPE_RECTANGLE) {
        _mesh = new Mesh();
        _mesh->tris = PrimtiveGetRectangle();
    }
    else {
        RBLOG("Unknow game object type");
    }
}

GameObject::GameObject(std::string name, int tag) {
    Initialise();

    _type = GAME_OBJECT_TYPE_MESH;
    _tag = tag;

    _mesh = new Mesh();
    _mesh->LoadObjectFile(name);
}

GameObject::GameObject(Mesh* mesh, int tag) {
    Initialise();

    _type = GAME_OBJECT_TYPE_MESH;
    _tag = tag;

    _mesh = mesh;
}

void GameObject::Initialise() {
    _id = ++object_counter;

    _isHidden = false;
    _color = -1;
    _isDead = false;
    _lifetime = FLT_MAX;
    _elapsed = 0;
    
    _position = Vec3DMake(0, 0, 0);
    _scale = Vec3DMake(1, 1, 1);
    _rotation = Vec3DMake(0, 0, 0);

    _speed = Vec3DMake(0, 0, 0);
    _rotationSpeed = Vec3DMake(0, 0, 0);
}

void GameObject::Update(float delta) {
    _position.x += _speed.x * delta;
    _position.y += _speed.y * delta;
    _position.z += _speed.z * delta;

    _rotation.x += _rotationSpeed.x * delta;
    _rotation.y += _rotationSpeed.y * delta;
    _rotation.z += _rotationSpeed.z * delta;
    
    _elapsed += delta;
    if (_elapsed > _lifetime) {
        SetDead();
    }
}

bool GameObject::IsColliding(GameObject& other) {
    return (
        GetMinX() <= other.GetMaxX() &&
        GetMaxX() >= other.GetMinX() &&
        GetMinY() <= other.GetMaxY() &&
        GetMaxY() >= other.GetMinY() &&
        GetMinZ() <= other.GetMaxZ() &&
        GetMaxZ() >= other.GetMinZ()
    );
}

void GameObject::Dump() {
    printf("Game object: %d (%d)\n", _id, _tag);
    printf("- Position: %.2f,%.2f,%.2f (%.2f,%.2f,%.2f)\n", _position. x, _position.y, _position.z, _speed.x, _speed.y, _speed.z);
    printf("- Rotation: %.2f,%.2f,%.2f (%.2f,%.2f,%.2f)\n", _rotation. x, _rotation.y, _rotation.z, _rotationSpeed.x, _rotationSpeed.y, _rotationSpeed.z);
    printf("- Lifetime: %f of %f (%d)\n", _elapsed, _lifetime, _isDead);
    printf("- Color: %s (%d)\n", _color == -1 ? "automatic" : "fix", _color);
    printf("- Type: %d\n\n", _type);
}

void GameObject::Dump(int id) {
    if (_id != id)
        return;
    
    Dump();
}

const std::vector<Triangle> GameObject::PrimtiveGetCube() {
    std::vector<Triangle> triangeList;

    // TODO: Optimze, dont re-create
    // 
    // SOUTH
    triangeList.push_back(Triangle(Vec3DMake(0.0f, 0.0f, 0.0f), Vec3DMake(0.0f, 1.0f, 0.0f), Vec3DMake(1.0f, 1.0f, 0.0f)));
    triangeList.push_back(Triangle(Vec3DMake(0.0f, 0.0f, 0.0f), Vec3DMake(1.0f, 1.0f, 0.0f), Vec3DMake(1.0f, 0.0f, 0.0f)));

    // EAST
    triangeList.push_back(Triangle(Vec3DMake(1.0f, 0.0f, 0.0f), Vec3DMake(1.0f, 1.0f, 0.0f), Vec3DMake(1.0f, 1.0f, 1.0f)));
    triangeList.push_back(Triangle(Vec3DMake(1.0f, 0.0f, 0.0f), Vec3DMake(1.0f, 1.0f, 1.0f), Vec3DMake(1.0f, 0.0f, 1.0f)));

    // NORTH
    triangeList.push_back(Triangle(Vec3DMake(1.0f, 0.0f, 1.0f), Vec3DMake(1.0f, 1.0f, 1.0f), Vec3DMake(0.0f, 1.0f, 1.0f)));
    triangeList.push_back(Triangle(Vec3DMake(1.0f, 0.0f, 1.0f), Vec3DMake(0.0f, 1.0f, 1.0f), Vec3DMake(0.0f, 0.0f, 1.0f)));

    // WEST
    triangeList.push_back(Triangle(Vec3DMake(0.0f, 0.0f, 1.0f), Vec3DMake(0.0f, 1.0f, 1.0f), Vec3DMake(0.0f, 1.0f, 0.0f)));
    triangeList.push_back(Triangle(Vec3DMake(0.0f, 0.0f, 1.0f), Vec3DMake(0.0f, 1.0f, 0.0f), Vec3DMake(0.0f, 0.0f, 0.0f)));

    // TOP
    triangeList.push_back(Triangle(Vec3DMake(0.0f, 1.0f, 0.0f), Vec3DMake(0.0f, 1.0f, 1.0f), Vec3DMake(1.0f, 1.0f, 1.0f)));
    triangeList.push_back(Triangle(Vec3DMake(0.0f, 1.0f, 0.0f), Vec3DMake(1.0f, 1.0f, 1.0f), Vec3DMake(1.0f, 1.0f, 0.0f)));

    // BOTTOM
    triangeList.push_back(Triangle(Vec3DMake(1.0f, 0.0f, 1.0f), Vec3DMake(0.0f, 0.0f, 1.0f), Vec3DMake(0.0f, 0.0f, 0.0f)));
    triangeList.push_back(Triangle(Vec3DMake(1.0f, 0.0f, 1.0f), Vec3DMake(0.0f, 0.0f, 0.0f), Vec3DMake(1.0f, 0.0f, 0.0f)));

    return triangeList;
}

const std::vector<Triangle> GameObject::PrimtiveGetRectangle() {
    std::vector<Triangle> triangeList;
    
    triangeList.push_back(Triangle(Vec3DMake(1, 0, 1), Vec3DMake(0, 0, 1), Vec3DMake(0, 0, 0)));
    triangeList.push_back(Triangle(Vec3DMake(1, 0, 1), Vec3DMake(0, 0, 0), Vec3DMake(1, 0, 0)));

    return triangeList;
}
