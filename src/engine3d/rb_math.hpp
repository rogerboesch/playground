//
//  rb_math.hpp
//  3d wireframe game engine: math functions
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#pragma once

#include "rb_types.hpp"

#include <math.h>

Vec3D Vec3DMakeZero();
Vec3D Vec3DMake(int x, int y, int z);
Vec3D Vec3DMakef(float x, float y, float z);

Vec3D Vec3DAdd(Vec3D &v1, Vec3D &v2);
Vec3D Vec3DSub(Vec3D &v1, Vec3D &v2);
Vec3D Vec3DMul(Vec3D &v1, float k);
Vec3D Vec3DDiv(Vec3D &v1, float k);
float Vec3DDotProduct(Vec3D &v1, Vec3D &v2);
float Vec3DLength(Vec3D &v);
Vec3D Vec3DNormalise(Vec3D &v);
Vec3D Vec3DCrossProduct(Vec3D &v1, Vec3D &v2);
Vec3D Vec3DIntersectPlane(Vec3D &plane_p, Vec3D &plane_n, Vec3D &lineStart, Vec3D &lineEnd);
float Vec3DAngle(Vec3D& vec1, Vec3D& vec2);

int TriangleClipAgainstPlane(Vec3D plane_p, Vec3D plane_n, Triangle &in_tri, Triangle &out_tri1, Triangle &out_tri2);

Vec3D MatrixMultiplyVector(Mat4x4 &m, Vec3D &i);
Mat4x4 MatrixMakeIdentity();
Mat4x4 MatrixMakeRotationX(float fAngleRad);
Mat4x4 MatrixMakeRotationY(float fAngleRad);
Mat4x4 MatrixMakeRotationZ(float fAngleRad);
Mat4x4 MatrixMakeScale(float sx, float sy, float sz);
Mat4x4 MatrixMakeTranslation(float x, float y, float z);
Mat4x4 MatrixMultiplyMatrix(Mat4x4 &m1, Mat4x4 &m2);
Mat4x4 MatrixQuickInverse(Mat4x4 &m);

Mat4x4 MatrixPointAt(Vec3D &pos, Vec3D &target, Vec3D &up);
Mat4x4 MatrixMakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
Mat4x4 MatrixMakeOrtho(float left, float right, float bottom, float top, float nearZ, float farZ);
