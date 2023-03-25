//
//  rb_math.cpp
//  3d wireframe game engine: math functions
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#include "rb_math.hpp"
#include "rb_base.h"


Vec3D Vec3DMake(int x, int y, int z) {
    return { float(x), float(y), float(z)};
}

Vec3D Vec3DAdd(Vec3D &v1, Vec3D &v2) {
    return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

Vec3D Vec3DSub(Vec3D &v1, Vec3D &v2) {
    return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

Vec3D Vec3DMul(Vec3D &v1, float k) {
    return { v1.x * k, v1.y * k, v1.z * k };
}

Vec3D Vec3DDiv(Vec3D &v1, float k) {
    return { v1.x / k, v1.y / k, v1.z / k };
}

float Vec3DDotProduct(Vec3D &v1, Vec3D &v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z;
}

float Vec3DLength(Vec3D &v) {
    return sqrtf(Vec3DDotProduct(v, v));
}

Vec3D Vec3DNormalise(Vec3D &v) {
    float l = Vec3DLength(v);
    return { v.x / l, v.y / l, v.z / l };
}

Vec3D Vec3DCrossProduct(Vec3D &v1, Vec3D &v2) {
    Vec3D v;
    v.x = v1.y * v2.z - v1.z * v2.y;
    v.y = v1.z * v2.x - v1.x * v2.z;
    v.z = v1.x * v2.y - v1.y * v2.x;
    return v;
}

Vec3D Vec3DIntersectPlane(Vec3D &plane_p, Vec3D &plane_n, Vec3D &lineStart, Vec3D &lineEnd) {
    plane_n = Vec3DNormalise(plane_n);
    float plane_d = -Vec3DDotProduct(plane_n, plane_p);
    float ad = Vec3DDotProduct(lineStart, plane_n);
    float bd = Vec3DDotProduct(lineEnd, plane_n);
    float t = (-plane_d - ad) / (bd - ad);
    Vec3D lineStartToEnd = Vec3DSub(lineEnd, lineStart);
    Vec3D lineToIntersect = Vec3DMul(lineStartToEnd, t);
    
    return Vec3DAdd(lineStart, lineToIntersect);
}

float Vec3DAngle(Vec3D& vec1, Vec3D& vec2) {
    float dot = vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z;
    float lenSq1 = vec1.x*vec1.x + vec1.y*vec1.y + vec1.z*vec1.z;
    float lenSq2 = vec2.x*vec2.x + vec2.y*vec2.y + vec2.z*vec2.z;
    float angle = acos(dot/sqrt(lenSq1 * lenSq2));

    return RAD_TO_DEG(angle);
}

int TriangleClipAgainstPlane(Vec3D plane_p, Vec3D plane_n, Triangle &in_tri, Triangle &out_tri1, Triangle &out_tri2) {
    // Make sure plane normal is indeed normal
    plane_n = Vec3DNormalise(plane_n);

    // Return signed shortest distance from point to plane, plane normal must be normalised
    auto dist = [&](Vec3D &p) {
        return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Vec3DDotProduct(plane_n, plane_p));
    };

    // Create two temporary storage arrays to classify points either side of plane
    // If distance sign is positive, point lies on "inside" of plane
    Vec3D* inside_points[3];  int nInsidePointCount = 0;
    Vec3D* outside_points[3]; int nOutsidePointCount = 0;

    // Get signed distance of each point in triangle to plane
    float d0 = dist(in_tri.p[0]);
    float d1 = dist(in_tri.p[1]);
    float d2 = dist(in_tri.p[2]);

    if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[0]; }
    else { outside_points[nOutsidePointCount++] = &in_tri.p[0]; }
    if (d1 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[1]; }
    else { outside_points[nOutsidePointCount++] = &in_tri.p[1]; }
    if (d2 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[2]; }
    else { outside_points[nOutsidePointCount++] = &in_tri.p[2]; }

    // Now classify triangle points, and break the input triangle into
    // smaller output triangles if required. There are four possible
    // outcomes...
    if (nInsidePointCount == 0) {
        // All points lie on the outside of plane, so clip whole triangle
        // It ceases to exist

        return 0; // No returned triangles are valid
    }

    if (nInsidePointCount == 3) {
        // All points lie on the inside of plane, so do nothing
        // and allow the triangle to simply pass through
        out_tri1 = in_tri;

        return 1; // Just the one returned original triangle is valid
    }

    if (nInsidePointCount == 1 && nOutsidePointCount == 2) {
        // Triangle should be clipped. As two points lie outside
        // the plane, the triangle simply becomes a smaller triangle

        // Copy appearance info to new triangle
        out_tri1.color =  in_tri.color;

        // The inside point is valid, so keep that...
        out_tri1.p[0] = *inside_points[0];

        // but the two new points are at the locations where the
        // original sides of the triangle (lines) intersect with the plane
        out_tri1.p[1] = Vec3DIntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
        out_tri1.p[2] = Vec3DIntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);

        return 1; // Return the newly formed single triangle
    }

    if (nInsidePointCount == 2 && nOutsidePointCount == 1) {
        // Triangle should be clipped. As two points lie inside the plane,
        // the clipped triangle becomes a "quad". Fortunately, we can
        // represent a quad with two new triangles

        // Copy appearance info to new triangles
        out_tri1.color =  in_tri.color;

        out_tri2.color =  in_tri.color;

        // The first triangle consists of the two inside points and a new
        // point determined by the location where one side of the triangle
        // intersects with the plane
        out_tri1.p[0] = *inside_points[0];
        out_tri1.p[1] = *inside_points[1];
        out_tri1.p[2] = Vec3DIntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);

        // The second triangle is composed of one of he inside points, a
        // new point determined by the intersection of the other side of the
        // triangle and the plane, and the newly created point above
        out_tri2.p[0] = *inside_points[1];
        out_tri2.p[1] = out_tri1.p[2];
        out_tri2.p[2] = Vec3DIntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);

        return 2; // Return two newly formed triangles which form a quad
    }
    
    return 0;
}

Vec3D MatrixMultiplyVector(Mat4x4 &m, Vec3D &i) {
    Vec3D v;
    v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
    v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
    v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
    v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
    
    return v;
}

Mat4x4 MatrixMakeIdentity() {
    Mat4x4 matrix = { 0 };
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    
    return matrix;
}

Mat4x4 MatrixMakeRotationX(float fAngleRad) {
    Mat4x4 matrix = { 0 };
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = cosf(fAngleRad);
    matrix.m[1][2] = sinf(fAngleRad);
    matrix.m[2][1] = -sinf(fAngleRad);
    matrix.m[2][2] = cosf(fAngleRad);
    matrix.m[3][3] = 1.0f;
    
    return matrix;
}

Mat4x4 MatrixMakeRotationY(float fAngleRad) {
    Mat4x4 matrix = { 0 };
    matrix.m[0][0] = cosf(fAngleRad);
    matrix.m[0][2] = sinf(fAngleRad);
    matrix.m[2][0] = -sinf(fAngleRad);
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = cosf(fAngleRad);
    matrix.m[3][3] = 1.0f;
    
    return matrix;
}

Mat4x4 MatrixMakeRotationZ(float fAngleRad) {
    Mat4x4 matrix = { 0 };
    matrix.m[0][0] = cosf(fAngleRad);
    matrix.m[0][1] = sinf(fAngleRad);
    matrix.m[1][0] = -sinf(fAngleRad);
    matrix.m[1][1] = cosf(fAngleRad);
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    
    return matrix;
}

Mat4x4 MatrixMakeScale(float sx, float sy, float sz) {
    Mat4x4 matrix = { 0 };
    matrix.m[0][0] = sx;
    matrix.m[0][1] = 0;
    matrix.m[0][2] = 0;
    matrix.m[0][3] = 0;
    
    matrix.m[1][0] = 0;
    matrix.m[1][1] = sy;
    matrix.m[1][2] = 0;
    matrix.m[1][3] = 0;
    
    matrix.m[2][0] = 0;
    matrix.m[2][1] = 0;
    matrix.m[2][2] = sz;
    matrix.m[2][3] = 0;
    
    matrix.m[3][0] = 0;
    matrix.m[3][1] = 0;
    matrix.m[3][2] = 0;
    matrix.m[3][3] = 1;

    return matrix;
}

Mat4x4 MatrixMakeTranslation(float x, float y, float z) {
    Mat4x4 matrix = { 0 };
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    matrix.m[3][0] = x;
    matrix.m[3][1] = y;
    matrix.m[3][2] = z;
    
    return matrix;
}

Mat4x4 MatrixMakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar) {
    float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
    Mat4x4 matrix = { 0 };
    matrix.m[0][0] = fAspectRatio * fFovRad;
    matrix.m[1][1] = fFovRad;
    matrix.m[2][2] = fFar / (fFar - fNear);
    matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matrix.m[2][3] = 1.0f;
    matrix.m[3][3] = 0.0f;
    
    return matrix;
}

// Multiply matrix specified by result with a perspective matrix and return new matrix in result
// result Specifies the input matrix.  new matrix is returned in result.
// left, right Coordinates for the left and right vertical clipping planes
// bottom, top Coordinates for the bottom and top horizontal clipping planes
// nearZ, farZ Distances to the near and far depth clipping planes.  These values are negative if plane is behind the viewer
Mat4x4 MatrixMakeOrtho(float left, float right, float bottom, float top, float nearZ, float farZ) {
    float deltaX = right - left;
    float deltaY = top - bottom;
    float deltaZ = farZ - nearZ;

    Mat4x4 matrix = { 0 };

    if ((deltaX == 0.0f) || (deltaY == 0.0f) || (deltaZ == 0.0f)) {
        return matrix;
    }
    
    matrix.m[0][0] = 2.0/deltaX;
    matrix.m[0][3] = -1.0;
    matrix.m[1][1] = 2.0/deltaY;
    matrix.m[1][3] = -1.0;
    matrix.m[2][2] = -2.0/(farZ-nearZ);
    matrix.m[3][3] = 1.0;
    
    return matrix;
}

Mat4x4 MatrixMultiplyMatrix(Mat4x4 &m1, Mat4x4 &m2) {
    Mat4x4 matrix = { 0 };
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
    
    return matrix;
}

Mat4x4 MatrixPointAt(Vec3D &pos, Vec3D &target, Vec3D &up) {
    // Calculate new forward direction
    Vec3D newForward = Vec3DSub(target, pos);
    newForward = Vec3DNormalise(newForward);

    // Calculate new Up direction
    Vec3D a = Vec3DMul(newForward, Vec3DDotProduct(up, newForward));
    Vec3D newUp = Vec3DSub(up, a);
    newUp = Vec3DNormalise(newUp);

    // New Right direction is easy, its just cross product
    Vec3D newRight = Vec3DCrossProduct(newUp, newForward);

    // Construct Dimensioning and Translation Matrix
    Mat4x4 matrix = { 0 };
    matrix.m[0][0] = newRight.x;    matrix.m[0][1] = newRight.y;    matrix.m[0][2] = newRight.z;    matrix.m[0][3] = 0.0f;
    matrix.m[1][0] = newUp.x;        matrix.m[1][1] = newUp.y;        matrix.m[1][2] = newUp.z;        matrix.m[1][3] = 0.0f;
    matrix.m[2][0] = newForward.x;    matrix.m[2][1] = newForward.y;    matrix.m[2][2] = newForward.z;    matrix.m[2][3] = 0.0f;
    matrix.m[3][0] = pos.x;            matrix.m[3][1] = pos.y;            matrix.m[3][2] = pos.z;            matrix.m[3][3] = 1.0f;
    
    return matrix;

}

// Only for Rotation/Translation Matrices
Mat4x4 MatrixQuickInverse(Mat4x4 &m) {
    Mat4x4 matrix = { 0 };
    matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
    matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
    matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
    matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
    matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
    matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
    matrix.m[3][3] = 1.0f;
    
    return matrix;
}
