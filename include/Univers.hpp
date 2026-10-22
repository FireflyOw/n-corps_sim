#pragma once

#include <vector>
#include "Vector3Double.hpp"

struct Corps { 
    double masse; 
    
    Vector3Double position, vitesse;

    Corps(double masse, Vector3Double position, Vector3Double vitesse) :
        masse(masse), position(position), vitesse(vitesse) {}
};

struct Univers {
    // On crée un tableau pour chaque info pour que chaque fonction puisse 
    // prendre ce dont elle a besoin sans avoir à copier les corps en entier
    std::vector<Vector3Double> positions, vitesses, accelerations;
    std::vector<double> masses;

    double G = 1;
    double temps = 0.0;
    double epsilon = 0.0;

    // Fonction pour ajouter les infos d'un corps dans les tableaux
    void ajouter(const Corps& corps) {
        positions.push_back(corps.position);
        vitesses.push_back(corps.vitesse);
        masses.push_back(corps.masse);

        accelerations.push_back(Vector3Double()); // vecteur nul au départ pour l'accélération
    }

    // Fonction pour connaître le nombre de corps dans l'univers
    std::size_t taille() const {
        return masses.size();
    }
};