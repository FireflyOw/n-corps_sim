#pragma once

#include <cmath>

struct Unites {
    // Références choisies
    double longueur_m; // en mètres
    double masse_kg;   // en kilogrammes

    double G = 6.674e-11; // constante en SI

    // Références calculées
    double temps   = std::sqrt(longueur_m * longueur_m * longueur_m / (G * masse_kg));  // en secondes
    double energie = masse_kg * longueur_m * longueur_m / (temps * temps);              // en joules
    double vitesse = longueur_m / temps;                                                // en m/s
};