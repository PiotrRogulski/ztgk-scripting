#pragma once

namespace duck_app {
    struct Vector {
        float x;
        float y;
        float z;

        Vector();

        Vector(float _x, float _y, float _z);

        Vector operator +(Vector a) const;

        Vector operator *(float a) const;

        static float Dot(Vector a, Vector b);

        static Vector Cross(Vector a, Vector b);
    };
}
