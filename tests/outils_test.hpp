#pragma once

#include <iostream>
#include <cmath>

#include "vecteurs.hpp"

inline void verifier(bool condition, const char* nom, int& echecs) {
    if (!condition) {
        std::cout << "ECHEC : " << nom << "\n";
        ++echecs;
    }
}

inline bool egal(double a, double b) {
    return std::abs(a - b) < 1e-12;
}

inline bool egal(const double3& a, const double3& b) {
    return egal(a.x, b.x) && egal(a.y, b.y) && egal(a.z, b.z);
}

// Comme egal(), mais avec une tolérance choisie : utile pour les simulations,
// où le résultat n'est qu'approché (erreur liée au pas de temps).
inline bool proche(double a, double b, double tolerance) {
    return std::abs(a - b) < tolerance;
}

inline bool proche(const double3& a, const double3& b, double tolerance) {
    return (a - b).norme() < tolerance;
}