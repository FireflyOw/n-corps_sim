#pragma once

class Vector3Double {
    public:

        double x;
        double y;
        double z;

        Vector3Double(double x, double y, double z);

        Vector3Double();
        
        Vector3Double operator+(const Vector3Double& vec);
        Vector3Double operator+=(const Vector3Double& vec);

        Vector3Double operator-(const Vector3Double& vec);
        Vector3Double operator-=(const Vector3Double& vec);

        Vector3Double operator*(double scalaire);

        double norme() const;
}