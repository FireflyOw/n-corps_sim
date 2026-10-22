#include <iostream>
#include <cmath>

#include "outils_test.hpp"
#include "Vector3Double.hpp"

static int echecs = 0;

int main() {
    Vector3Double v(3, 4, 0);

    // Tests opérateurs
    const Vector3Double vecteurA(1, 2, 3);
    const Vector3Double vecteurB(4, -1, 2);

    verifier(egal(vecteurA + vecteurB, Vector3Double(5, 1, 5)),   "addition (1,2,3)+(4,-1,2)", echecs);
    verifier(egal(vecteurA - vecteurB, Vector3Double(-3, 3, 1)),  "soustraction (1,2,3)-(4,-1,2)", echecs);
    verifier(egal(vecteurA * 2.0,      Vector3Double(2, 4, 6)),   "multiplication (1,2,3)*2", echecs);
    verifier(egal(vecteurA / 2.0,      Vector3Double(0.5, 1, 1.5)), "division (1,2,3)/2", echecs);

    // Aller-retour : += puis -= doit redonner le vecteur de départ
    // (c'est ce test qui aurait attrapé l'inversion de += et -=)
    Vector3Double vecteurModifie = vecteurA;
    vecteurModifie += vecteurB;
    verifier(egal(vecteurModifie, Vector3Double(5, 1, 5)), "operateur +=", echecs);
    vecteurModifie -= vecteurB;
    verifier(egal(vecteurModifie, vecteurA), "aller-retour += puis -=", echecs);

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