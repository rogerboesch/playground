//
//  rb_object.hpp
//  3d wireframe game engine: game object
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#pragma once

#include "rb_mesh.hpp"
#include "rb_math.hpp"

#include <string>

#define GAME_OBJECT_TYPE_CUBE       1
#define GAME_OBJECT_TYPE_RECTANGLE  2
#define GAME_OBJECT_TYPE_MESH       3

class GameObject {
public:
    GameObject(int type, int tag = 0);
    GameObject(std::string name, int tag = 0);
    GameObject(Mesh* mesh, int tag = 0);

private:
    void Initialise();
    
public:
    void SetPosition(float x, float y, float z) { _position = Vec3DMake(x, y, z); }
    void SetPosition(Vec3D pos) { _position = pos; }
    Vec3D& GetPosition() { return _position; }
    void SetRotation(float x, float y, float z) { _rotation = Vec3DMake(x, y, z); }
    Vec3D& GetRotation() { return _rotation; }
    void SetScale(float x, float y, float z) { _scale = Vec3DMake(x, y, z); }
    Vec3D& GetScale() { return _scale; }
    void SetColor(int color) { _color = color; }
    int GetColor() { return _color; }
    void SetSpeed(float x, float y, float z) { _speed = Vec3DMake(x, y, z); }
    Vec3D& GetSpeed() { return _speed; }
    void SetRotationSpeed(float x, float y, float z) { _rotationSpeed = Vec3DMake(x, y, z); }
    Vec3D& GetRotationSpeed() { return _rotationSpeed; }
    Mesh* GetMesh() { return _mesh; }
    
    void SetPlayer(bool flag) { _isPlayer = flag; }
    void SetHidden(bool flag) { _isHidden = flag; }
    void ToggleHidden() { _isHidden = !_isHidden; }
    bool IsHidden() { return _isHidden; }
    void SetDead() { _isDead = true; }
    void SetAlive() { _isDead = false; }
    bool IsDead() { return _isDead; }
    void SetLifeTime(float sec) { _lifetime = sec; }

    int GetID() { return _id; }
    int GetTag() { return _tag; }

    void Move(float x, float y, float z) { _position.x += x;  _position.y += y; _position.z += z; }
    void Update(float delta);

    void Dump();
    void Dump(int id);

    float GetMinX() { return _position.x; }
    float GetMinY() { return _position.y; }
    float GetMinZ() { return _position.z; }
    float GetMaxX() { return _position.x + _scale.x; }
    float GetMaxY() { return _position.y + _scale.y; }
    float GetMaxZ() { return _position.z + _scale.z; }

    bool IsColliding(GameObject& other);

private:
    const std::vector<Triangle> PrimtiveGetCube();
    const std::vector<Triangle> PrimtiveGetRectangle();

private:
    int _id;
    int _tag;
    int _type;
    int _color;
    Mesh* _mesh;
    Vec3D _position;
    Vec3D _rotation;
    Vec3D _scale;
    Vec3D _speed;
    Vec3D _rotationSpeed;
    float _lifetime;
    float _elapsed;
    bool _isHidden = false;
    bool _isDead;
    bool _isPlayer = false;
    static Mesh* s_cube;
};
