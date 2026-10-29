#include <iostream>
#include <cmath>

#include "outils_test.hpp"
#include "vecteurs.hpp"

static int echecs = 0;

int main() {
    double3 v(3, 4, 0);

    // Tests opérateurs
    const double3 vecteurA(1, 2, 3);
    const double3 vecteurB(4, -1, 2);

    verifier(egal(vecteurA + vecteurB, double3(5, 1, 5)),   "addition (1,2,3)+(4,-1,2)", echecs);
    verifier(egal(vecteurA - vecteurB, double3(-3, 3, 1)),  "soustraction (1,2,3)-(4,-1,2)", echecs);
    verifier(egal(vecteurA * 2.0,      double3(2, 4, 6)),   "multiplication (1,2,3)*2", echecs);
    verifier(egal(vecteurA / 2.0,      double3(0.5, 1, 1.5)), "division (1,2,3)/2", echecs);

    // Aller-retour : += puis -= doit redonner le vecteur de départ
    // (c'est ce test qui aurait attrapé l'inversion de += et -=)
    double3 vecteurModifie = vecteurA;
    vecteurModifie += vecteurB;
    verifier(egal(vecteurModifie, double3(5, 1, 5)), "operateur +=", echecs);
    vecteurModifie -= vecteurB;
    verifier(egal(vecteurModifie, vecteurA), "aller-retour += puis -=", echecs);

    // Test normes
    verifier(egal(v.norme(), 5.0), "norme de (3,4,0)", echecs);
    verifier(egal(v.norme2(), 25.0), "norme2 de (3,4,0)", echecs);

    // Test produit scalaire
    verifier(egal(scalaire(double3(1, 2, 3), double3(4, 5, 6)), 32.0), "scalaire (1,2,3)(4,5,6)", echecs);

    // Tests produits vectoriel
    verifier(egal(vectoriel(double3(1, 0, 0), double3(0, 1, 0)), double3(0, 0, 1)), "vectoriel (1,0,0)(0,1,0)", echecs);
    verifier(egal(vectoriel(double3(0, 1, 0), double3(1, 0, 0)), double3(0, 0, -1)), "vectoriel (0,1,0)(1,0,0)", echecs);

    if (echecs == 0) std::cout << "Tous les tests passent\n";
    return echecs;
}