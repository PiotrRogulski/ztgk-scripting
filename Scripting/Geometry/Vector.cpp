#include "Vector.h"

namespace duck_app {
    Vector::Vector() : x(0.0F), y(0.0F), z(0.0F) { }

    Vector::Vector(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) { }

    Vector Vector::operator +(const Vector a) const {
        return {this->x + a.x, this->y + a.y, this->z + a.z};
    }

    Vector Vector::operator *(const float a) const {
        return {this->x * a, this->y * a, this->z * a};
    }

    float Vector::Dot(const Vector a, const Vector b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    Vector Vector::Cross(const Vector a, const Vector b) {
        return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    }
}
