//
// Created by John Rockwell on 2/6/25.
//

#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <raylib.h>

class MathHelper {
public:
    static Vector3 AddVector3D(const Vector3& a, const Vector3& b) {
        return Vector3{a.x + b.x, a.y + b.y, a.z + b.z };
    }

    static Vector3 SubtractVector3D(Vector3& a, Vector3& b) {
        return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    static Vector3 CrossVector3D(const Vector3& a, const Vector3& b) {
        return { a.y * b.z - a.z * b.y,
                    a.z * b.x - a.x * b.z,
                        a.x * b.y - a.y * b.x };
        }

    static Matrix MatrixIdentity() {
        Matrix m = { 0 };
        m.m0 = 1.0f;
        m.m5 = 1.0f;
        m.m10 = 1.0f;
        m.m15 = 1.0f;
        return m;
    }

    static Matrix MatrixTranslate(const float x, const float y, const float z) {
        Matrix m = { 0 };
        m.m0 = 1.0f;
        m.m5 = 1.0f;
        m.m10 = 1.0f;
        m.m15 = 1.0f;
        m.m12 = x;
        m.m13 = y;
        m.m14 = z;
        return m;
    }

    static float LengthVector3D(const Vector3& v) {
        return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    }

    static Vector3 ScaleVector3D(const Vector3& v, const float s) {
        return { v.x * s, v.y * s, v.z * s };
    }

    static Vector3 NormalizeVector3D(const Vector3& v) {
        const float len = LengthVector3D(v);
        if (len == 0.0f) return v;
        return ScaleVector3D(v, 1.0f / len);
    }

    static float DotVector3D(const Vector3& a, const Vector3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
};

#endif //MATH_H
