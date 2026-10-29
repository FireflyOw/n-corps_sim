#pragma once

#include "unites.hpp"
#include "univers.hpp"

struct Simulation {
    Univers univers;
    Unites unites;

    std::vector<Corps> condInitiales;

    const double dt;
    std::size_t pasImage = 10;

    bool simPause = false;

    // Initialisation des valeurs d'énergie
    double energieDepart;
    double eTotale;

    Simulation(const Unites& unites, const std::vector<Corps>& condInitiales, double dt);

    void avancer();
    void reinitialiser();

    double calculTempsSim() const;
    double calculEnergieReel() const;
    double calculErreurEnergie() const;
};