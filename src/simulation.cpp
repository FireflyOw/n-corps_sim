#include "physique.hpp"
#include "simulation.hpp"

#include <cmath>
#include <algorithm>

// Constructeur de la structure
Simulation::Simulation(const Unites& unites, const std::vector<Corps>& condInitiales, double dt) :
    unites(unites), condInitiales(condInitiales), dt(dt) {

    reinitialiser();
}

// Avancer la simulation en fonction du pas de temps et du nombre de pas par image
void Simulation::avancer() {
    if (!simPause) {    
        for (std::size_t i = 0; i < pasImage; ++i) {
            pasLeapfrog(univers, dt);

            // On ajoute la dernière position au tampon de chaque corps pour dessiner leur trajectoire
            for (std::size_t iCorps = 0; iCorps < univers.taille(); ++iCorps) {
                if (univers.temps - dernierPointTrainees[iCorps] >= intervalleTrainees[iCorps]) {
                    trainees[iCorps].ajouter(univers.positions[iCorps]);

                    dernierPointTrainees[iCorps] += intervalleTrainees[iCorps];
                }
            }
        }

        // Calcul de l'énergie totale du système
        eTotale = energieTotale(univers);
    }
}

// Réinitialiser la simulation aux conditions initiales
void Simulation::reinitialiser() {
    initUnivers(univers, condInitiales);

    // Création des tableaux de trainées à la bonne taille
    intervalleTrainees.assign(univers.taille(), 0.0);
    dernierPointTrainees.assign(univers.taille(), 0.0);
    trainees.assign(univers.taille(), TamponCirculaire<double3> (longueurTrainees));

    // On parcours les corps pour calculer l'intervalle de dessin des trainées (en sautant le soleil)
    for (std::size_t iCorps = 1; iCorps < univers.taille(); ++iCorps) {
        double distanceSoleil = (univers.positions[iCorps] - univers.positions[0]).norme();
        double distance3 = distanceSoleil * distanceSoleil * distanceSoleil;
        double periode = 2 * M_PI * std::sqrt(distance3 / univers.masses[0]);

        intervalleTrainees[iCorps] = fractionOrbite * periode / longueurTrainees;
    }

    // Pour le soleil, on prend l'intervalle le plus grand de la liste (pas très important tant qu'on zoom pas dessus)
    if (univers.taille() > 1) intervalleTrainees[0] = *std::max_element(intervalleTrainees.begin() + 1, intervalleTrainees.end());
        
    // Calculs des énergies de départ
    energieDepart = energieTotale(univers);
    eTotale       = energieDepart;
}

double Simulation::calculTempsSim() const {
    return unites.versSecondes(univers.temps) / constantes::secondesParJour;
}

double Simulation::calculEnergieReel() const {
    return eTotale * unites.energie_J();
}

double Simulation::calculErreurEnergie() const {
    return (energieDepart - eTotale) / energieDepart;
}