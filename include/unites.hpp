#pragma once

#include <cmath>

namespace constantes {
    constexpr double uniteAstronomique_m = 1.495978707e11;
    constexpr double masseSolaire_kg     = 1.98847e30;
    constexpr double kiloparsec_m        = 3.0856775814913673e19;
    constexpr double secondesParJour     = 86400.0;
    constexpr double secondesParAn       = 365.25 * 86400.0;
}

// ---------------------------------------------------------------------------------------------------
// Références système (equivalent en unités SI d'une unité interne)
// ex : 1 unité de longueur = x mètres | 1 unité de temps = y secondes
//
// On part de la dimension de G = [L³ / (M · T²)] dont on choisi de fixer la longueur et 
// la masse, ce qui permet de calculer le temps. De ces 3 valeurs on peut déterminer toutes 
// les autres références et donc faire des conversion entre nos unités internes arbitraires et le SI
// ---------------------------------------------------------------------------------------------------

struct Unites {
    // ------------------------------------------------------
    // Calcul des références à partir de G = [L³ / (M · T²)]
    // ------------------------------------------------------
    double longueur_m; // en mètres
    double masse_kg;   // en kilogrammes

    static constexpr double G_SI = 6.67430e-11; // constante en SI

    // Calcul du temps de référence : T = √(L³ / (G · M))
    double temps_s() const {
        return std::sqrt(longueur_m * longueur_m * longueur_m / (G_SI * masse_kg));  // en secondes
    }

    // Calcul de la vitesse de référence : v = L / T
    double vitesse_ms() const {
        return longueur_m / temps_s(); // en m/s
    }

    // Calcul de l'énergie de référence : E = M · L² / T²
    double energie_J() const {
        return masse_kg * longueur_m * longueur_m / (temps_s() * temps_s()); // en joules
    }

    // ------------------------------------------------------
    // Fonctions de conversion des unités internes vers SI
    // ------------------------------------------------------

    // Conversion de l'unité interne de longueur vers des mètres
    double versMetres(double longueurInterne) const {
        return longueurInterne * longueur_m;
    }

    // Conversion de l'unité interne de masse vers des kilogrammes
    double versKilogrammes(double masseInterne) const {
        return masseInterne * masse_kg;
    }
    
    // Conversion de l'unité interne de temps vers des secondes
    double versSecondes(double tempsInterne) const {
        return tempsInterne * temps_s();
    }

    // ------------------------------------------------------
    // Fonctions de conversion des unités SI vers interne
    // ------------------------------------------------------

    // Conversion de mètres vers l'unité interne de longueur
    double versLongueur(double longueurSI) const {
        return longueurSI / longueur_m;
    }

    // Conversion de kilogrammes vers l'unité interne de masse
    double versMasse(double masseSI) const {
        return masseSI / masse_kg;
    }

    // Conversion de secondes vers l'unité interne de temps
    double versTemps(double tempsSI) const {
        return tempsSI / temps_s();
    }
};