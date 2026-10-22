#include <iostream>
#include <cmath>

#include "Outils_test.hpp"
#include "Vector3Double.hpp"

static int echecs = 0;

int main() {
    Vector3Double v(3, 4, 0);

    // Tests opérateurs

    // Test normes
    verifier(egal(v.norme(), 5.0), "norme de (3,4,0)", echecs);
    verifier(egal(v.norme2(), 25.0), "norme2 de (3,4,0)", echecs);

    // Test produit scalaire
    verifier(egal(scalaire(Vector3Double(1, 2, 3), Vector3Double(4, 5, 6)), 32.0), "scalaire (1,2,3)(4,5,6)", echecs);

    // Tests produits vectoriel
    verifier(egal(vectoriel(Vector3Double(1, 0, 0), Vector3Double(0, 1, 0)), Vector3Double(0, 0, 1)), "vectoriel (1,0,0)(0,1,0)", echecs);
    verifier(egal(vectoriel(Vector3Double(0, 1, 0), Vector3Double(1, 0, 0)), Vector3Double(0, 0, -1)), "vectoriel (0,1,0)(1,0,0)", echecs);


    if (echecs == 0) std::cout << "Tous les tests passent\n";
    return echecs;
}