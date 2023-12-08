//
//  rb_engine.hpp
//  3d wireframe game engine
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#pragma once

#include "rb_math.hpp"
#include "rb_mesh.hpp"
#include "rb_types.hpp"

#include <vector>

#define COLOR_MODE_AUTO     -1
#define COLOR_MODE_RED      -2

#define MAX_CONTROLS        16
#define CONTROL1_BTN1        0
#define CONTROL1_BTN2        1
#define CONTROL1_BTN3        2
#define CONTROL1_BTN4        3
#define CONTROL1_JOY_LEFT    4
#define CONTROL1_JOY_RIGHT   5
#define CONTROL1_JOY_UP      6
#define CONTROL1_JOY_DOWN    7

// Not yet supported
#define CONTROL2_BTN1        8
#define CONTROL2_BTN2        9
#define CONTROL2_BTN3       10
#define CONTROL2_BTN4       11
#define CONTROL2_JOY_LEFT   12
#define CONTROL2_JOY_RIGHT  13
#define CONTROL2_JOY_UP     14
#define CONTROL2_JOY_DOWN   15

enum COLOR {
    colorWhite, colorGray,
    colorBlue, colorBlueLight,
    colorRed, colorRedLight,
    colorViolett, colorViolettLight,
    colorGreen, colorGreenLight,
    colorCyan, colorCyanLight,
    colorYellow, colorYellowLight
};

enum CONTROLSTATE { ctrlUndefined, ctrlPressed, ctrlHold, ctrlReleased };

typedef struct CONTROL_TAG {
    CONTROLSTATE state;
    float stateTime;
} CONTROL;

VecRGB GetPaletteColor(byte color, int brightness);

class GameObject;

class GameEngine {
public:
    GameEngine();
    ~GameEngine();
    
// Drawing
public:
    void SetPixel(int x, int y, uint8_t paletteColor, int brightness);
    void DrawLine(int x1, int y1, int x2, int y2, byte color);
    void DrawTriangle(Vec3D& vec1, Vec3D& vec2, Vec3D& vec3, byte color, int number);
    void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint8_t paletteColor, int brightness);
    void Transform(std::vector<Triangle>& vecTrianglesToRaster, Mesh& mesh, Vec3D& pos, Vec3D& rot, Vec3D& scale);
    void Clip(int& x, int& y);
    void ClipAndDraw(std::vector<Triangle>& vecTrianglesToRaster, byte color);
    void DrawMesh(Mesh& mesh, Vec3D& pos, Vec3D& rot, Vec3D& scale, byte color);
    
// Text
public:
    void DrawVectorString(char* msg, int x, int y, int size, byte color);
    void DrawString(char* msg, int x, int y, int size, byte color);

// World and camera matrix
public:
    void BuildWorldMatrix();
    void UpdateCamera(float fYaw);

// Lifeycle
public:
    bool Init(char* name, int width, int height, bool filled = false);
    void Start();
    void Stop();
    void SyncFrame(int ms);
    void Frame();

// Overridables in game class
public:
    virtual bool OnCreate() = 0;
    virtual bool OnUpdate(float deltaTime) = 0;
    virtual bool OnDestroy() { return true; }

// Controls
public:
    bool IsControlPressed(int code);
    bool IsControlHold(int code);
    CONTROL GetControl(int code) { return _controls[code]; }
    bool IsJoystick1Up() { return IsControlPressed(CONTROL1_JOY_UP); }
    bool IsJoystick1Down() { return IsControlPressed(CONTROL1_JOY_DOWN); }
    bool IsJoystick1Left() { return IsControlPressed(CONTROL1_JOY_LEFT); }
    bool IsJoystick1Right() { return IsControlPressed(CONTROL1_JOY_RIGHT); }

    void SetControlState(int code, CONTROLSTATE state) { _controls[code].state = state; }

// Getter/Setter
public:
    void AddGameObject(GameObject* object) { m_gameObjects.push_back(object); }
    bool IsFinished() { return _finished; }
    void SetFinished() { _finished = true; }
    int GetScreenWidth() { return _screenWidth; }
    int GetScreenHeight() { return _screenHeight; }
    void SetProjectionMatrix(Mat4x4& matProj) { _matProj = matProj; }
    void SetCameraPos(float x, float y, float z) { _camera.x = x; _camera.y = y; _camera.z = z; }
    void SetCameraPos(Vec3D pos) { _camera.x = pos.x; _camera.y = pos.y; _camera.z = pos.z; }
    void SetCameraPosX(float x) { _camera.x = x; }
    void SetCameraPosY(float y) { _camera.y = y; }
    void SetCameraPosZ(float z) { _camera.z = z; }
    void ChangeCameraPosX(float x) { _camera.x += x; }
    void ChangeCameraPosY(float y) { _camera.y += y; }
    void ChangeCameraPosZ(float z) { _camera.z += z; }
    void ChangeCameraAdd(Vec3D vec) { _camera = Vec3DAdd(_camera, vec); }
    void ChangeCameraSub(Vec3D vec) { _camera = Vec3DSub(_camera, vec); }
    Vec3D GetCameraPos() { return _camera; }
    Vec3D GetLookDirectionVector() { return _lookDir; }

    void SetFilled(bool flag) { _filled = flag; }
    
    void SetAutoUpdate(bool flag) { _autoUpdate = flag; }

// Helper
public:
    int GetBrightness(float lum);
    void ChangeControlState(int code, bool flag, float deltaTime);
    void UpdateControlStates(float deltaTime);
    
protected:
    std::vector<GameObject *> m_gameObjects;

private:
    Mat4x4 _matProj;    // Matrix that converts from view space to screen space
    Vec3D _camera;      // Location of camera in world space
    Vec3D _lookDir;     // Direction vector along the direction camera points
    
    Mat4x4 _matWorld;
    Mat4x4 _matView;

    int _screenWidth;
    int _screenHeight;
    double _timer1;
    double _timer2;
    double _frame_last = -1;
    bool _finished;
    bool _filled = false;
    bool _autoUpdate = true;            // If true then game objects get updated by engine
    CONTROL _controls[MAX_CONTROLS];
};
