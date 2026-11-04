#pragma once

#include <cmath>

struct float3 {
    float x, y, z;

    float3() : x(0), y(0), z(0) {}
    float3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    float3 operator+(const float3& vec) const {
        return float3(x + vec.x, y + vec.y, z + vec.z);
    }

    float3 operator-(const float3& vec) const {
        return float3(x - vec.x, y - vec.y, z - vec.z);
    }

    float3& operator+=(const float3& vec) {
        x += vec.x;
        y += vec.y;
        z += vec.z;

        return *this;
    }

    float3& operator-=(const float3& vec) {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;

        return *this;
    }

    float3 operator*(float s) const {
        return float3(x * s, y * s, z * s);
    }

    float3 operator/(float s) const {
        const float inverse = (1 / s);
        return float3(x * inverse, y * inverse, z * inverse);
    }

    float norme() const {
        return std::sqrt(norme2());
    }

    float norme2() const {
        return x * x + y * y + z * z;
    }
};

struct double3 {
    double x, y, z;

    double3() : x(0), y(0), z(0) {}
    double3(double x, double y, double z) : x(x), y(y), z(z) {}
    
    double3 operator+(const double3& vec) const {
        return double3(x + vec.x, y + vec.y, z + vec.z);
    }

    double3 operator-(const double3& vec) const {
        return double3(x - vec.x, y - vec.y, z - vec.z);
    }

    double3& operator+=(const double3& vec) {
        x += vec.x;
        y += vec.y;
        z += vec.z;

        return *this;
    }

    double3& operator-=(const double3& vec) {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;

        return *this;
    }

    double3 operator*(double s) const {
        return double3(x * s, y * s, z * s);
    }

    double3 operator/(double s) const {
        const double inverse = (1 / s);
        return double3(x * inverse, y * inverse, z * inverse);
    }

    double norme() const {
        return std::sqrt(norme2());
    }

    double norme2() const {
        return x * x + y * y + z * z;
    }
};

inline double scalaire(const double3& vecA, const double3& vecB) {
    return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
}

inline float scalaire(const float3& vecA, const float3& vecB) {
    return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
}

inline double3 vectoriel(const double3& vecA, const double3& vecB) {
    return double3(vecA.y * vecB.z - vecA.z * vecB.y, vecA.z * vecB.x - vecA.x * vecB.z, vecA.x * vecB.y - vecA.y * vecB.x);
}

inline float3 vectoriel(const float3& vecA, const float3& vecB) {
    return float3(vecA.y * vecB.z - vecA.z * vecB.y, vecA.z * vecB.x - vecA.x * vecB.z, vecA.x * vecB.y - vecA.y * vecB.x);
}