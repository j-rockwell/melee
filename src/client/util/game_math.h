//
// Created by John Rockwell on 2/6/25.
//

#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <raylib.h>

/**
 * TODO: @deprecated - Stop using this
 * Use raylib_math.h instead. This was used back when there
 * was a weird linker issue that has since been resolved.
 */
class MathHelper {
public:
    static Vector3 NormalizeVector3D(Vector3 v) {
        const float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
        if (len == 0.0f) {
            return v;
        }

        v.x /= len;
        v.y /= len;
        v.z /= len;
        return v;
    }

    static Vector3 AddVector3D(Vector3 a, Vector3 b) {
        return Vector3{a.x + b.x, a.y + b.y, a.z + b.z };
    }

    static Vector3 SubtractVector3D(Vector3 a, Vector3 b) {
        return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    static Matrix MatrixIdentity() {
        Matrix m = { 0 };
        m.m0 = 1.0f;
        m.m5 = 1.0f;
        m.m10 = 1.0f;
        m.m15 = 1.0f;
        return m;
    }

    static Matrix MatrixTranslate(float x, float y, float z) {
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
};

#endif //MATH_H
