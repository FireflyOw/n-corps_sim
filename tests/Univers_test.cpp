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
    for (const Corps c : listeCorps) univers.ajouter(c);

    verifier(egal(univers.taille(), univers.masses.size()), "taille de l'univers", echecs);

    if (echecs == 0) std::cout << "Tous les tests passent\n";
    return echecs;
}