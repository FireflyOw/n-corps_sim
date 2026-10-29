#include "physique.hpp"
#include "simulation.hpp"

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
        }

        // Calcul de l'énergie totale du système
        eTotale = energieTotale(univers);
    }
}

// Réinitialiser la simulation aux conditions initiales
void Simulation::reinitialiser() {
    initUnivers(univers, condInitiales);

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