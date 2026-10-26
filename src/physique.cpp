#include "physique.hpp"

#include <cmath>

// ---------------------------------------------------------------------
// Fonctions de calcul de l'accélération des coprs en fonction 
// de la force qu'ils appliquent les uns sur les autres
// ---------------------------------------------------------------------

void calculAccelerations(Univers& univers) {
    const std::size_t nbCorps = univers.taille();

    // On remet toutes les accélérations à 0 avant de calculer les nouvelles
    for (Vector3Double& accel : univers.accelerations) accel = Vector3Double();

    // Boucles décalées pour éviter les doublons 
    for (std::size_t iCorps = 0; iCorps < nbCorps; ++iCorps) {
        for (std::size_t iAutre = iCorps + 1; iAutre < nbCorps; ++iAutre) {

            // Calcul du vecteur allant de Corps vers Autre et de la distance² (pour 
            // économiser une racine carrée) adoucie par un facteur epsilon entre les deux
            Vector3Double vecDistance = univers.positions[iAutre] - univers.positions[iCorps];
            double distance2 = vecDistance.norme2() + univers.epsilon * univers.epsilon;

            // Calcul de la distance inverse au cube
            double distanceInverse  = 1 / std::sqrt(distance2);
            double distanceInverse3 = distanceInverse * distanceInverse * distanceInverse;

            // Application de la formule de Newton pour le calcul des 
            // accélérations (opposées et dépendantes de la masse de l'autre corps)
            univers.accelerations[iCorps] += vecDistance * univers.G * univers.masses[iAutre] * distanceInverse3;
            univers.accelerations[iAutre] -= vecDistance * univers.G * univers.masses[iCorps] * distanceInverse3;
        }
    }
}

// ---------------------------------------------------------------------
// Fonctions d'intégration : Euler (basique et symplectique) et Leapfrog
// ---------------------------------------------------------------------

void pasEuler(Univers& univers, double dt) {
    calculAccelerations(univers);

    for (std::size_t iCorps = 0; iCorps < univers.taille(); ++iCorps) {
        univers.positions[iCorps] += univers.vitesses[iCorps] * dt;
        univers.vitesses[iCorps]  += univers.accelerations[iCorps] * dt;
    }   

    // On avance le temps de simulation
    univers.temps += dt;
}

void pasEulerSymplectique(Univers& univers, double dt) {
    calculAccelerations(univers);

    for (std::size_t iCorps = 0; iCorps < univers.taille(); ++iCorps) {
        univers.vitesses[iCorps]  += univers.accelerations[iCorps] * dt;
        univers.positions[iCorps] += univers.vitesses[iCorps] * dt;
    } 

    // On avance le temps de simulation
    univers.temps += dt;  
}

void pasLeapfrog(Univers& univers, double dt) {
    // Premier pas de l'intégrateur
    for (std::size_t iCorps = 0; iCorps < univers.taille(); ++iCorps) {
        univers.vitesses[iCorps]  += univers.accelerations[iCorps] * (dt / 2); // kick
        univers.positions[iCorps] += univers.vitesses[iCorps] * dt; // drift
    }

    // Calcul des nouvelles accélérations aux nouvelles positions
    calculAccelerations(univers);

    // Deuxième pas de l'intégrateur
    for (std::size_t iCorps = 0; iCorps < univers.taille(); ++iCorps) {
        univers.vitesses[iCorps] += univers.accelerations[iCorps] * (dt / 2); // kick
    }

    // On avance le temps de simulation
    univers.temps += dt;
}

// ---------------------------------------------------------------------
// Fonctions annexes : calcul d'énergie totale et initialisation de l'univers
// ---------------------------------------------------------------------

// Fonctions de calcul de l'énergie totale du système (cinétique + potentielle)
double energieTotale(const Univers& univers) {
    const std::size_t nbCorps = univers.taille();

    double energieCinetique = 0.0;
    double energiePotentielle = 0.0;

    // Même principe de boucles décalées pour éviter les doublons
    for (std::size_t iCorps = 0; iCorps < nbCorps; ++iCorps) {
        for (std::size_t iAutre = iCorps + 1; iAutre < nbCorps; ++iAutre) {
            
            Vector3Double vecDistance = univers.positions[iAutre] - univers.positions[iCorps];
            double distance = std::sqrt(vecDistance.norme2() + univers.epsilon * univers.epsilon);
            
            // Calcul de l'énergie potentielle en (G * m1 * m2 / d)
            energiePotentielle -= univers.G * univers.masses[iCorps] * univers.masses[iAutre] / distance;
        }

        // Calcul de l'énergie cinétique en (1/2 * mv²)
        energieCinetique += 0.5 * univers.masses[iCorps] * univers.vitesses[iCorps].norme2();
    }

    // Énergie totale = potentielle + cinétique
    return energieCinetique + energiePotentielle;
}

// Fonction pour ajouter tous les corps d'une liste dans un univers
void initUnivers(Univers& univers, std::vector<Corps>& conditionsInitiales) {
    univers.positions.clear();
    univers.vitesses.clear();
    univers.masses.clear();
    univers.rayons.clear();

    univers.temps = 0;

    for (const Corps& c : conditionsInitiales) univers.ajouter(c); 

    calculAccelerations(univers);
}