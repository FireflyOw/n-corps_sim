#pragma once

#include <cmath>

class Vector3Double {
    public:

        double x;
        double y;
        double z;

        Vector3Double(double x, double y, double z);

        Vector3Double();
        
        Vector3Double operator+(const Vector3Double& vec) const;
        Vector3Double operator-(const Vector3Double& vec) const;

        Vector3Double& operator-=(const Vector3Double& vec);
        Vector3Double& operator+=(const Vector3Double& vec);

        Vector3Double operator*(double scalaire) const;

        double norme() const;

        double norme2() const;
};

inline double scalaire(const Vector3Double& vecA, const Vector3Double& vecB);

inline Vector3Double vectoriel(const Vector3Double& vecA, const Vector3Double& vecB);