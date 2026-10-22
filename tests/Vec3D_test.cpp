#include <iostream>
#include <cmath>
#include "Vector3Double.hpp"

static int echecs = 0;

void verifier(bool condition, const char* nom) {
    if (!condition) {
        std::cout << "ECHEC : " << nom << "\n";
        ++echecs;
    }
}

bool egal(double a, double b) {
    return std::abs(a - b) < 1e-12;
}

bool egal(const Vector3Double& a, const Vector3Double& b) {
    return egal(a.x, b.x) && egal(a.y, b.y) && egal(a.z, b.z);
}

int main() {
    Vector3Double v(3, 4, 0);

    // Tests opérateurs

    // Test normes
    verifier(egal(v.norme(), 5.0), "norme de (3,4,0)");
    verifier(egal(v.norme2(), 25.0), "norme2 de (3,4,0)");

    // Test produit scalaire
    verifier(egal(scalaire(Vector3Double(1, 2, 3), Vector3Double(4, 5, 6)), 32.0), "scalaire (1,2,3)(4,5,6)");

    // Tests produits vectoriel
    verifier(egal(vectoriel(Vector3Double(1, 0, 0), Vector3Double(0, 1, 0)), Vector3Double(0, 0, 1)), "vectoriel (1,0,0)(0,1,0)");
    verifier(egal(vectoriel(Vector3Double(0, 1, 0), Vector3Double(1, 0, 0)), Vector3Double(0, 0, -1)), "vectoriel (0,1,0)(1,0,0)");


    if (echecs == 0) std::cout << "Tous les tests passent\n";
    return echecs;
}