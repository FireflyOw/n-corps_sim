// Compare Euler, Euler symplectique et leapfrog sur une orbite elliptique.
// Pour chaque orbite, affiche la plus grande erreur relative sur l'énergie
// observée pendant cette orbite, et la distance planète-étoile à la fin de
// l'orbite (elle devrait revenir à 1). Toutes les méthodes utilisent le même dt.
//
// Lancement : ./build/comp_integrateurs

#include <cstdio>
#include <cmath>

#include "physique.hpp"

using FonctionPas = void (*)(Univers&, double);

Univers creerSystemeEtoilePlanete() {
    Univers univers;
    univers.ajouter(Corps(1.0,   Vector3Double(0, 0, 0), Vector3Double(0, 0,   0)));  // étoile
    univers.ajouter(Corps(1e-10, Vector3Double(1, 0, 0), Vector3Double(0, 0.8, 0)));  // planète
    return univers;
}

int main() {
    // Orbite elliptique : demi-grand axe a = 1 / (2 - v²), période T = 2π·a^(3/2)
    const double vitesseInitiale = 0.8;
    const double demiGrandAxe = 1.0 / (2.0 - vitesseInitiale * vitesseInitiale);
    const double periode = 2.0 * 3.14159265358979323846 * std::pow(demiGrandAxe, 1.5);

    const int pasParOrbite = 500;
    const double dt = periode / pasParOrbite;
    const int nombreOrbites = 20;

    const char* nomsMethodes[] = { "Euler", "Euler sympl.", "Leapfrog" };
    const FonctionPas methodes[] = { pasEuler, pasEulerSymplectique, pasLeapfrog };

    // Un univers par méthode, tous partant du même état
    Univers univers[3];
    double energiesInitiales[3];
    for (int indiceMethode = 0; indiceMethode < 3; ++indiceMethode) {
        univers[indiceMethode] = creerSystemeEtoilePlanete();
        calculAccelerations(univers[indiceMethode]);   // condition de départ du leapfrog
        energiesInitiales[indiceMethode] = energieTotale(univers[indiceMethode]);
    }

    std::printf("Orbite elliptique, periode = %.4f, %d pas par orbite (dt = %.5f)\n\n", periode, pasParOrbite, dt);
    std::printf("%-7s |", "Orbite");
    for (const char* nom : nomsMethodes) std::printf(" %-12s erreur E max  distance |", nom);
    std::printf("\n");

    for (int numeroOrbite = 1; numeroOrbite <= nombreOrbites; ++numeroOrbite) {
        std::printf("%-7d |", numeroOrbite);

        for (int indiceMethode = 0; indiceMethode < 3; ++indiceMethode) {
            Univers& systeme = univers[indiceMethode];   // référence : on modifie l'univers du tableau
            double erreurEnergieMax = 0.0;

            for (int numeroPas = 0; numeroPas < pasParOrbite; ++numeroPas) {
                methodes[indiceMethode](systeme, dt);

                const double erreurEnergie = std::abs((energieTotale(systeme) - energiesInitiales[indiceMethode])
                                                      / energiesInitiales[indiceMethode]);
                if (erreurEnergie > erreurEnergieMax) erreurEnergieMax = erreurEnergie;
            }

            const double distance = (systeme.positions[1] - systeme.positions[0]).norme();
            std::printf(" %24.3e  %8.4f |", erreurEnergieMax, distance);
        }
        std::printf("\n");
    }

    // Après un nombre entier d'orbites, la planète devrait être revenue en (1, 0, 0)
    std::printf("\nPosition finale de la planete (attendu : 1, 0, 0)\n");
    for (int indiceMethode = 0; indiceMethode < 3; ++indiceMethode) {
        const Vector3Double& position = univers[indiceMethode].positions[1];
        std::printf("  %-12s (%8.4f, %8.4f, %8.4f)\n", nomsMethodes[indiceMethode], position.x, position.y, position.z);
    }
    return 0;
}