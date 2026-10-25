#pragma once

#include <cmath>

class Vector3Double {
    public:

        double x;
        double y;
        double z;

        Vector3Double() : x(0), y(0), z(0) {}

        Vector3Double(double x, double y, double z) : x(x), y(y), z(z) {}
        
        Vector3Double operator+(const Vector3Double& vec) const {
            return Vector3Double(x + vec.x, y + vec.y, z + vec.z);
        }

        Vector3Double operator-(const Vector3Double& vec) const {
            return Vector3Double(x - vec.x, y - vec.y, z - vec.z);
        }

        Vector3Double& operator+=(const Vector3Double& vec) {
            x += vec.x;
            y += vec.y;
            z += vec.z;

            return *this;
        }

        Vector3Double& operator-=(const Vector3Double& vec) {
            x -= vec.x;
            y -= vec.y;
            z -= vec.z;

            return *this;
        }

        Vector3Double operator*(double s) const {
            return Vector3Double(x * s, y * s, z * s);
        }

        Vector3Double operator/(double s) const {
            const double inverse = (1 / s);
            return Vector3Double(x * inverse, y * inverse, z * inverse);
        }

        double norme() const {
            return std::sqrt(norme2());
        }

        double norme2() const {
            return x * x + y * y + z * z;
        }
};

inline double scalaire(const Vector3Double& vecA, const Vector3Double& vecB) {
    return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
}

inline Vector3Double vectoriel(const Vector3Double& vecA, const Vector3Double& vecB) {
    return Vector3Double(vecA.y * vecB.z - vecA.z * vecB.y, vecA.z * vecB.x - vecA.x * vecB.z, vecA.x * vecB.y - vecA.y * vecB.x);
}