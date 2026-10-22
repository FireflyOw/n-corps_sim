#include "Physique.hpp"

#include <cmath>

void calculAccelerations(Univers& univers) {
    const std::size_t nbCorps = univers.taille();

    for (Vector3Double& accel : univers.accelerations) accel = Vector3Double();

    // Boucles décalées pour calculer l'accélération de chaque 
    // corps par rapport à tous les autres en évitant les doublons 
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

double energieTotale(const Univers& univers) {
    const std::size_t nbCorps = univers.taille();

    // Calcul de l'énergie cinétique en (1/2 * mv²)
    double energieCinetique = 0.0;
    for (std::size_t iCorps = 0; iCorps < nbCorps; ++iCorps) {
        energieCinetique += 0.5 * univers.masses[iCorps] * univers.vitesses[iCorps].norme2();
    }

    // Calcul de l'énergie potentielle en (G * m1 * m2 / d)
    double energiePotentielle = 0.0;
    for (std::size_t iCorps = 0; iCorps < nbCorps; ++iCorps) {
        for (std::size_t iAutre = iCorps + 1; iAutre < nbCorps; ++iAutre) {
            
            Vector3Double vecDistance = univers.positions[iAutre] - univers.positions[iCorps];
            double distance = std::sqrt(vecDistance.norme2() + univers.epsilon * univers.epsilon);
            
            energiePotentielle -= univers.G * univers.masses[iCorps] * univers.masses[iAutre] / distance;
        }
    }

    return energieCinetique + energiePotentielle;
}