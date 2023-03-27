//
//  rb_engine.cpp
//  3d wireframe game engine
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_engine.hpp"
#include "rb_object.hpp"
#include "rb_platform.h"
#include "rb_log.h"
#include "rb_vtext.h"

#include <list>
#include <algorithm>

#ifdef _WIN32
extern "C" int win_sleep_ms(int wait);
#endif

GameEngine::GameEngine() {
    _screenWidth = 640;
    _screenHeight = 480;
}

GameEngine::~GameEngine() {}

// MARK: - Text

void GameEngine::DrawVectorString(char* msg, int x, int y, int size, byte color) {
    vtext_draw_string(x, y, msg, 1, color);
}

void GameEngine::DrawBitmapString(char* msg, int x, int y, int size, byte color) {
    vtext_draw_string(x, y, msg, 1, color);
}

// MARK: - Drawing

void GameEngine::DrawLine(int x1, int y1, int x2, int y2, byte color) {
    platform_draw_line(x1, y1, x2, y2, color, INVERT_OFF);
}

void GameEngine::SetPixel(int x, int y, uint8_t paletteColor, int brightness) {
    platform_set_pixel(x, y, paletteColor, brightness);
}

void GameEngine::DrawTriangle(Vec3D& vec1, Vec3D& vec2, Vec3D& vec3, byte color, int number) {
    DrawLine(vec1.x, vec1.y, vec2.x, vec2.y, color);
    DrawLine(vec2.x, vec2.y, vec3.x, vec3.y, color);
    DrawLine(vec3.x, vec3.y, vec1.x, vec1.y, color);
}

void GameEngine::FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint8_t paletteColor, int brightness) {
    auto SWAP = [](int &x, int &y) { int t = x; x = y; y = t; };
    auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) SetPixel(i, ny, paletteColor, brightness); };

    int t1x, t2x, y, minx, maxx, t1xp, t2xp;
    bool changed1 = false;
    bool changed2 = false;
    int signx1, signx2, dx1, dy1, dx2, dy2;
    int e1, e2;

    // Sort vertices
    if (y1>y2) { SWAP(y1, y2); SWAP(x1, x2); }
    if (y1>y3) { SWAP(y1, y3); SWAP(x1, x3); }
    if (y2>y3) { SWAP(y2, y3); SWAP(x2, x3); }

    t1x = t2x = x1; y = y1;
    dx1 = (int)(x2 - x1); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
    else signx1 = 1;
    dy1 = (int)(y2 - y1);

    dx2 = (int)(x3 - x1); if (dx2<0) { dx2 = -dx2; signx2 = -1; }
    else signx2 = 1;
    dy2 = (int)(y3 - y1);

    if (dy1 > dx1) {
        SWAP(dx1, dy1);
        changed1 = true;
    }
    
    if (dy2 > dx2) {
        SWAP(dy2, dx2);
        changed2 = true;
    }

    e2 = (int)(dx2 >> 1);
    
    // Flat top, just process the second half
    if (y1 == y2)
        goto next;
    
    e1 = (int)(dx1 >> 1);

    for (int i = 0; i < dx1;) {
        t1xp = 0; t2xp = 0;
        
        if (t1x<t2x) {
            minx = t1x; maxx = t2x;
        }
        else {
            minx = t2x; maxx = t1x;
        }
        
        // process first line until y value is about to change
        while (i<dx1) {
            i++;
            e1 += dy1;
            
            while (e1 >= dx1) {
                e1 -= dx1;
                
                if (changed1)
                    t1xp = signx1;
                else
                    goto next1;
            }
            
            if (changed1)
                break;
            else
                t1x += signx1;
        }
        
    // Move line
    next1:
        // process second line until y value is about to change
        while (1) {
            e2 += dy2;
            
            while (e2 >= dx2) {
                e2 -= dx2;
                if (changed2)
                    t2xp = signx2;
                else
                    goto next2;
            }
            
            if (changed2)
                break;
            else
                t2x += signx2;
        }
        
    next2:
        if (minx>t1x)
            minx = t1x;
        if (minx>t2x)
            minx = t2x;
        if (maxx<t1x)
            maxx = t1x;
        if (maxx<t2x) maxx = t2x;
        
        drawline(minx, maxx, y);
        
        if (!changed1)
            t1x += signx1;
        t1x += t1xp;
        
        if (!changed2)
            t2x += signx2;
        t2x += t2xp;
        
        y += 1;
        if (y == y2)
            break;
    }
    
next:
    // Second half
    dx1 = (int)(x3 - x2); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
    else signx1 = 1;
    dy1 = (int)(y3 - y2);
    t1x = x2;

    if (dy1 > dx1) {   // swap values
        SWAP(dy1, dx1);
        changed1 = true;
    }
    else changed1 = false;

    e1 = (int)(dx1 >> 1);

    for (int i = 0; i <= dx1; i++) {
        t1xp = 0; t2xp = 0;
        if (t1x<t2x) { minx = t1x; maxx = t2x; }
        else { minx = t2x; maxx = t1x; }
        // process first line until y value is about to change
        while (i<dx1) {
            e1 += dy1;
            while (e1 >= dx1) {
                e1 -= dx1;
                if (changed1) { t1xp = signx1; break; }//t1x += signx1;
                else          goto next3;
            }
            if (changed1) break;
            else                t1x += signx1;
            if (i<dx1) i++;
        }
        
    next3:
        // process second line until y value is about to change
        while (t2x != x3) {
            e2 += dy2;
            while (e2 >= dx2) {
                e2 -= dx2;
                if (changed2) t2xp = signx2;
                else          goto next4;
            }
            if (changed2)     break;
            else              t2x += signx2;
        }
        
    next4:
        if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
        if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
        drawline(minx, maxx, y);
        if (!changed1) t1x += signx1;
        t1x += t1xp;
        if (!changed2) t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y>y3) return;
    }
}

void GameEngine::Clip(int& x, int& y) {
    if (x < 0) x = 0;
    
    if (x >= _screenWidth)
        x = _screenWidth;
    
    if (y < 0) y = 0;
    
    if (y >= _screenHeight)
        y = _screenHeight;
}

void GameEngine::ClipAndDraw(std::vector<Triangle>& vecTrianglesToRaster, byte color) {
    // Loop through all transformed, viewed, projected, and sorted triangles
    for (auto &triToRaster : vecTrianglesToRaster) {
        // Clip triangles against all four screen edges, this could yield
        // a bunch of triangles, so create a queue that we traverse to
        // ensure we only test new triangles generated against planes
        Triangle clipped[2];
        std::list<Triangle> listTriangles;

        // Add initial triangle
        listTriangles.push_back(triToRaster);
        int nNewTriangles = 1;

        for (int p = 0; p < 4; p++) {
            int nTrisToAdd = 0;
            
            while (nNewTriangles > 0) {
                // Take triangle from front of queue
                Triangle test = listTriangles.front();
                listTriangles.pop_front();
                nNewTriangles--;

                // Clip it against a plane. We only need to test each
                // subsequent plane, against subsequent new triangles
                // as all triangles after a plane clip are guaranteed
                // to lie on the inside of the plane. I like how this
                // comment is almost completely and utterly justified
                switch (p) {
                    case 0: nTrisToAdd = TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                    case 1: nTrisToAdd = TriangleClipAgainstPlane({ 0.0f, (float)_screenWidth - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                    case 2: nTrisToAdd = TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                    case 3: nTrisToAdd = TriangleClipAgainstPlane({ (float)_screenWidth - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                }

                // Clipping may yield a variable number of triangles, so
                // add these new ones to the back of the queue for subsequent
                // clipping against next planes
                for (int w = 0; w < nTrisToAdd; w++) {
                    listTriangles.push_back(clipped[w]);
                }
            }
            
            nNewTriangles = (int)listTriangles.size();
        }
        
        for (auto &t : listTriangles) {
            if (_filled) FillTriangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, t.color, t.bright);
            else DrawTriangle(t.p[0], t.p[1], t.p[2], color, t.h);
        }
    }
}

void GameEngine::Transform(std::vector<Triangle>& vecTrianglesToRaster, Mesh& mesh, Vec3D& pos, Vec3D& rot, Vec3D& scale) {
    for (auto tri : mesh.tris) {
        Triangle triProjected, triTransformed, triRotated, triScaled, triViewed;

        Mat4x4 matScale = MatrixMakeScale(scale.x, scale.y, scale.z);
        triScaled.p[0] = MatrixMultiplyVector(matScale, tri.p[0]);
        triScaled.p[1] = MatrixMultiplyVector(matScale, tri.p[1]);
        triScaled.p[2] = MatrixMultiplyVector(matScale, tri.p[2]);

        Mat4x4 matRotX = MatrixMakeRotationX(rot.x);
        Mat4x4 matRotY = MatrixMakeRotationY(rot.y);
        Mat4x4 matRotZ = MatrixMakeRotationZ(rot.z);

        triRotated.p[0] = MatrixMultiplyVector(matRotX, triScaled.p[0]);
        triRotated.p[1] = MatrixMultiplyVector(matRotX, triScaled.p[1]);
        triRotated.p[2] = MatrixMultiplyVector(matRotX, triScaled.p[2]);

        triRotated.p[0] = MatrixMultiplyVector(matRotY, triRotated.p[0]);
        triRotated.p[1] = MatrixMultiplyVector(matRotY, triRotated.p[1]);
        triRotated.p[2] = MatrixMultiplyVector(matRotY, triRotated.p[2]);

        triRotated.p[0] = MatrixMultiplyVector(matRotZ, triRotated.p[0]);
        triRotated.p[1] = MatrixMultiplyVector(matRotZ, triRotated.p[1]);
        triRotated.p[2] = MatrixMultiplyVector(matRotZ, triRotated.p[2]);

        // World Matrix Transform
        triTransformed.p[0] = MatrixMultiplyVector(_matWorld, triRotated.p[0]);
        triTransformed.p[1] = MatrixMultiplyVector(_matWorld, triRotated.p[1]);
        triTransformed.p[2] = MatrixMultiplyVector(_matWorld, triRotated.p[2]);

        triTransformed.p[0] = Vec3DAdd(triTransformed.p[0], pos);
        triTransformed.p[1] = Vec3DAdd(triTransformed.p[1], pos);
        triTransformed.p[2] = Vec3DAdd(triTransformed.p[2], pos);

        // Calculate triangle normal
        Vec3D normal, line1, line2;

        line1 = Vec3DSub(triTransformed.p[1], triTransformed.p[0]);
        line2 = Vec3DSub(triTransformed.p[2], triTransformed.p[0]);
        normal = Vec3DCrossProduct(line1, line2);
        normal = Vec3DNormalise(normal);

        Vec3D _cameraRay = Vec3DSub(triTransformed.p[0], _camera);

        // If ray is aligned with normal, then triangle is visible
        if (Vec3DDotProduct(normal, _cameraRay) < 0.0f) {
            Vec3D light_direction = { 0.0f, 1.0f, -1.0f };
            light_direction = Vec3DNormalise(light_direction);

            float dp = std::max(0.1f, Vec3DDotProduct(light_direction, normal));
            triTransformed.bright = GetBrightness(dp);
            triTransformed.color = mesh.color;
            
            // Convert World Space --> View Space
            triViewed.p[0] = MatrixMultiplyVector(_matView, triTransformed.p[0]);
            triViewed.p[1] = MatrixMultiplyVector(_matView, triTransformed.p[1]);
            triViewed.p[2] = MatrixMultiplyVector(_matView, triTransformed.p[2]);
            triViewed.bright = triTransformed.bright;
            triViewed.color = triTransformed.color;

            // Clip Viewed Triangle against near plane
            int nClippedTriangles = 0;
            Triangle clipped[2];
            nClippedTriangles = TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

            // We may end up with multiple triangles form the clip, so project as required
            for (int n = 0; n < nClippedTriangles; n++) {
                // Project triangles from 3D --> 2D
                triProjected.p[0] = MatrixMultiplyVector(_matProj, clipped[n].p[0]);
                triProjected.p[1] = MatrixMultiplyVector(_matProj, clipped[n].p[1]);
                triProjected.p[2] = MatrixMultiplyVector(_matProj, clipped[n].p[2]);
                triProjected.bright = clipped[n].bright;
                triProjected.color = clipped[n].color;

                // Scale into view, we moved the normalising into cartesian space
                // out of the matrix.vector function from the previous videos, so do this manually
                triProjected.p[0] = Vec3DDiv(triProjected.p[0], triProjected.p[0].w);
                triProjected.p[1] = Vec3DDiv(triProjected.p[1], triProjected.p[1].w);
                triProjected.p[2] = Vec3DDiv(triProjected.p[2], triProjected.p[2].w);

                // X/Y are inverted so put them back
                triProjected.p[0].x *= -1.0f;
                triProjected.p[1].x *= -1.0f;
                triProjected.p[2].x *= -1.0f;
                triProjected.p[0].y *= -1.0f;
                triProjected.p[1].y *= -1.0f;
                triProjected.p[2].y *= -1.0f;

                // Offset verts into visible normalised space
                Vec3D vOffsetView = { 1,1,0 };
                triProjected.p[0] = Vec3DAdd(triProjected.p[0], vOffsetView);
                triProjected.p[1] = Vec3DAdd(triProjected.p[1], vOffsetView);
                triProjected.p[2] = Vec3DAdd(triProjected.p[2], vOffsetView);
                triProjected.p[0].x *= 0.5f * (float)_screenWidth;
                triProjected.p[0].y *= 0.5f * (float)_screenHeight;
                triProjected.p[1].x *= 0.5f * (float)_screenWidth;
                triProjected.p[1].y *= 0.5f * (float)_screenHeight;
                triProjected.p[2].x *= 0.5f * (float)_screenWidth;
                triProjected.p[2].y *= 0.5f * (float)_screenHeight;

                // Save drawing flag
                triProjected.h = tri.h;

                // Store triangle for sorting
                vecTrianglesToRaster.push_back(triProjected);
            }
        }
    }

    // Sort triangles from back to front
    sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](Triangle &t1, Triangle &t2) {
        float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
        float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
        return z1 > z2;
    });
}

void GameEngine::DrawMesh(Mesh& mesh, Vec3D& pos, Vec3D& rot, Vec3D& scale, byte color) {
    std::vector<Triangle>vecTrianglesToRaster;
    mesh.color = color;
    Transform(vecTrianglesToRaster, mesh, pos, rot, scale);
    ClipAndDraw(vecTrianglesToRaster, color);
}

// MARK: - World and camera matrix

void GameEngine::BuildWorldMatrix() {
    Mat4x4 matTrans;
    matTrans = MatrixMakeTranslation(0.0f, 0.0f, 5.0f);

    _matWorld = MatrixMakeIdentity();
    _matWorld = MatrixMultiplyMatrix(_matWorld, matTrans);
}

void GameEngine::UpdateCamera(float fYaw) {
    Vec3D vUp = { 0,1,0 };
    Vec3D vTarget = { 0,0,1 };
    Mat4x4 matCameraRot = MatrixMakeRotationY(fYaw);
    _lookDir = MatrixMultiplyVector(matCameraRot, vTarget);
    
    vTarget = Vec3DAdd(_camera, _lookDir);
    Mat4x4 matCamera = MatrixPointAt(_camera, vTarget, vUp);
    _matView = MatrixQuickInverse(matCamera);
}

// MARK: - Controls

bool GameEngine::IsControlPressed(int code) {
    return _controls[code].state == ctrlPressed;
}

bool GameEngine::IsControlHold(int code) {
    if (_controls[code].state == ctrlPressed || _controls[code].state == ctrlHold) {
        return true;
    }
    
    return false;
}

void GameEngine::ChangeControlState(int code, bool flag, float deltaTime) {
    CONTROLSTATE state = _controls[code].state;
    CONTROLSTATE newState = state;
    
    switch (state) {
        case ctrlUndefined:
            newState = flag ? ctrlPressed : ctrlUndefined;
            break;
        case ctrlPressed:
            newState = flag ? ctrlHold : ctrlReleased;
            break;
        case ctrlHold:
            newState = flag ? ctrlHold : ctrlReleased;
            break;
        case ctrlReleased:
            newState = flag ? ctrlPressed : ctrlReleased;
            break;
    }
    
    if (state != newState) {
        _controls[code].state = newState;
        _controls[code].stateTime = 0;
    }
    else {
        // update time
        _controls[code].stateTime += deltaTime;
    }
}

void GameEngine::UpdateControlStates(float deltaTime) {
#if 0 // TODO: Refactor later
    // This must be called each from outside if NEEDED by a platform (Ex. PiTrex)
    // It's not called by the engine itself anymore 
    for (int i = 0; i < MAX_CONTROLS; i ++) {
        bool state = platform_get_control_state(i+1);
        ChangeControlState(i, state, deltaTime);
    }
#endif
}

// MARK: - Lifeycle

bool GameEngine::Init(char* name, int width, int height, bool filled) {
    RBTRACELOG("GameEngine","Init");

    _screenWidth = width;
    _screenHeight = height;
    _filled = filled;
    
    // Initialise controls
    for (int i = 0; i < MAX_CONTROLS; i++) {
        CONTROL control = { ctrlUndefined, 0 };
        _controls[i] = control;
    }
    
    platform_on_init(name, width, height);

    srand(platform_get_ms());
    
    RBLOG("GameEngine");
    RBLOG_STR1( "Name ", name);
    RBLOG_NUM1(" Width ", width);
    RBLOG_NUM1(" Height", height);

    return true;
}

void GameEngine::Start() {
    RBTRACELOG("GameEngine","Start");

    _finished = !OnCreate();
    _timer1 = platform_get_ms();

    if (!_finished) {
        RBLOG("OnCreate() failed");
    }
    else {
        RBLOG("OnCreate() succeeded");
    }
}

void GameEngine::Stop() {
    RBTRACELOG("GameEngine","Stop");

    if (!OnDestroy()) {
        RBLOG("GameEngine: Stop cancelled");
        _finished = false;
    }
}

void GameEngine::SyncFrame(int ms) {
    // Disabled currently
    return;

    // Check delay time
    if (_frame_last > 0) {
        double now = platform_get_ms();
        double delay = (now - _frame_last);
        long wait = ms - delay;

        if (wait > 0) {
            //printf("GameEngine: SyncFrame waits %lu ms\n", wait);
#ifdef _WIN32
            win_sleep_ms(wait);
#else
            struct timespec ts;
            ts.tv_sec = wait / 1000;
            ts.tv_nsec = (wait % 1000) * 1000000;
            nanosleep(&ts, NULL);
#endif
        }
    }
    
    _frame_last = platform_get_ms();
}

void GameEngine::Frame() {
    if (_finished) {
        return;
    }
    
    SyncFrame(40);
    
    _timer2 = platform_get_ms();
    float deltaTime = _timer2 - _timer1;
    _timer1 = _timer2;

    deltaTime /= 1000.0f;
    
    platform_on_frame(deltaTime);
    
    bool result = OnUpdate(deltaTime);

    for (auto gameObject : m_gameObjects) {
        if (!gameObject->IsDead()) {
            
            if (result)
                gameObject->Update(deltaTime);
            
            if (!gameObject->IsHidden()) {
                DrawMesh(*gameObject->GetMesh(), gameObject->GetPosition(), gameObject->GetRotation(), gameObject->GetScale(), gameObject->GetColor());
            }
        }
    }
}

int GameEngine::GetBrightness(float lum) {    
    int light = (int)(100*lum);
    
    return light;
}

VecRGB GetPaletteColor(byte color, int brightness) {
    VecRGB rgb;
    rgb.r = 0xff; rgb.g = 0xff; rgb.b = 0xff;

    switch (color) {
    case colorGray:
        rgb.r = 0xd8; rgb.g = 0xd8; rgb.b = 0xd8;   // gray
        break;
    case colorWhite:
        rgb.r = 0xff; rgb.g = 0xff; rgb.b = 0xff;   // white
        break;
    case colorBlue:
        rgb.r = 0; rgb.g = 0; rgb.b = 0xd8;         // blue
        break;
    case colorBlueLight:                            
        rgb.r = 0; rgb.g = 0; rgb.b = 0xff;
        break;
    case colorRed:
        rgb.r = 0xd8; rgb.g = 0; rgb.b = 0;         // red
        break;
    case colorRedLight:
        rgb.r = 0xff; rgb.g = 0; rgb.b = 0;
        break;
    case colorViolett:
        rgb.r = 0xd8; rgb.g = 0; rgb.b = 0xd8;      // violet
        break;
    case colorViolettLight:
        rgb.r = 0xff; rgb.g = 0; rgb.b = 0xff;
        break;
    case colorGreen:
        rgb.r = 0; rgb.g = 0xd8; rgb.b = 0x00;      // green
        break;
    case colorGreenLight:
        rgb.r = 0; rgb.g = 0xff; rgb.b = 0x00;
        break;
    case colorCyan:
        rgb.r = 0; rgb.g = 0xd8; rgb.b = 0xd8;      // cyan
        break;
    case colorCyanLight:
        rgb.r = 0; rgb.g = 0xff; rgb.b = 0xff;
        break;
    case colorYellow:
        rgb.r = 0xd8; rgb.g = 0xd8; rgb.b = 0;      // yellow
        break;
    case colorYellowLight:
        rgb.r = 0xff; rgb.g = 0xff; rgb.b = 0;
        break;
    }

    if (brightness == BRIGHTNESS_OFF) {
        return rgb;
    }
    
#ifdef ML_LUMIN
    // A bit brighter
    rgb.r *= 0.2f;
    rgb.g *= 0.2f;
    rgb.b *= 0.2f;
#endif

    float br = (float)brightness/100;

    float r = rgb.r * br;
    float g = rgb.g * br;
    float b = rgb.b * br;

    rgb.r = fminf(255, r);
    rgb.g = fminf(255, g);
    rgb.b = fminf(255, b);

    return rgb;
}

