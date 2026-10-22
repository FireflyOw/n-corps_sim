#include "Physique.hpp"

#include <cmath>

void calculAcceleration(Univers& univers) {
    const std::size_t nbCorps = univers.taille();

    for (Vector3Double accel : univers.accelerations) accel = Vector3Double();

    // Boucles décalées pour calculer l'accélération de chaque 
    // corps par rapport à tous les autres en évitant les doublons 
    for (int iCorps = 0; iCorps < nbCorps; ++iCorps) {
        for (int iAutre = iCorps + 1; iAutre < nbCorps; ++iAutre) {

            // Calcul du vecteur allant de Corps vers Autre et de la distance² (pour 
            // économiser une racine carrée) adoucie par un facteur epsilon entre les deux
            Vector3Double vecDistance = univers.positions[iAutre] - univers.positions[iCorps];
            double distance2 = vecDistance.norme2() + univers.epsilon * univers.epsilon;

            // Calcul de la distance inverse au cube
            double distanceInverse  = 1 / std::sqrt(distance2);
            double distanceInverse3 = distanceInverse * distanceInverse * distanceInverse;

            // Application de la formule de Newton pour le calcul des accélérations (égales et opposées)
            univers.accelerations[iCorps] += vecDistance * univers.G * univers.masses[iAutre] * distanceInverse3;
            univers.accelerations[iAutre] += vecDistance * univers.G * univers.masses[iCorps] * distanceInverse3;
        }
    }
}