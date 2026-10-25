#pragma once

#include <cmath>

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

    static constexpr double G_SI = 6.674e-11; // constante en SI

    // Calcul du temps de référence : T = √(L³ / (G · M))
    double temps_s() {
        return std::sqrt(longueur_m * longueur_m * longueur_m / (G_SI * masse_kg));  // en secondes
    }

    // Calcul de la vitesse de référence : v = L / T
    double vitesse_ms(double temps_ref) {
        return longueur_m / temps_ref; // en m/s
    }

    // Calcul de l'énergie de référence : E = M · L² / T²
    double energie_J(double temps_ref) {
        masse_kg * longueur_m * longueur_m / (temps_ref * temps_ref); // en joules
    }

    // ------------------------------------------------------
    // Fonctions de conversion des unités internes vers SI
    // ------------------------------------------------------
    double versMetres(double longueurInterne) const {
        return longueurInterne * longueur_m;
    }

    double versKilogrammes(double masseInterne) const {
        return masseInterne * masse_kg;
    }
    
    double versMetresSeconde(double vitesseInterne) const {
        return vitesseInterne * vitesse_ms(temps_s());
    }

    // ------------------------------------------------------
    // Fonctions de conversion des unités SI vers interne
    // ------------------------------------------------------
    double versLongueur(double longueurInterne) const {
        return longueurInterne / longueur_m;
    }
};