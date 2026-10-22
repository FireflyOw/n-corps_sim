#include <iostream>
#include <cmath>

#include "Univers.hpp"
#include "Outils_test.hpp"
#include "Vector3Double.hpp"

Univers univers;

static int echecs = 0;

std::vector<Corps> listeCorps = {
    Corps(1, Vector3Double(2,3,4), Vector3Double(5,6,7)),
    Corps(2, Vector3Double(3,4,5), Vector3Double(6,7,8))
};

int main() {
    for (const Corps& c : listeCorps) univers.ajouter(c);

    verifier(egal(univers.taille(), 2), "taille de l'univers", echecs);

    verifier(egal(univers.masses.size(), 2), "taille tableau masses", echecs);
    verifier(egal(univers.positions.size(), 2), "taille tableau positions", echecs);
    verifier(egal(univers.vitesses.size(), 2), "taille tableau vitesses", echecs);
    verifier(egal(univers.accelerations.size(), 2), "taille tableau accelerations", echecs);

    verifier(egal(univers.masses[0], 1), "valeur masse 1", echecs);
    verifier(egal(univers.positions[0], Vector3Double(2,3,4)), "valeur position 1", echecs);
    verifier(egal(univers.vitesses[0], Vector3Double(5,6,7)), "valeur vitesse 1", echecs);

    verifier(egal(univers.masses[1], 2), "valeur masse 2", echecs);
    verifier(egal(univers.positions[1], Vector3Double(3,4,5)), "valeur position 2", echecs);
    verifier(egal(univers.vitesses[1], Vector3Double(6,7,8)), "valeur vitesse 2", echecs);

    if (echecs == 0) std::cout << "Tous les tests passent\n";
    return echecs;
}