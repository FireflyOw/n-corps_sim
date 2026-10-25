#pragma once

#include <vector>
#include "vector3double.hpp"

struct Corps { 
    double masse, rayon;    
    Vector3Double position, vitesse;

    Corps(double masse, double rayon, Vector3Double position, Vector3Double vitesse) :
        masse(masse), rayon(rayon), position(position), vitesse(vitesse) {}
};

struct Univers {
    // On crée un tableau pour chaque info pour que chaque fonction puisse 
    // prendre ce dont elle a besoin sans avoir à copier les corps en entier
    std::vector<Vector3Double> positions, vitesses, accelerations;
    std::vector<double> masses, rayons;

    double G = 1;
    double temps = 0.0;
    double epsilon = 0.0;

    // Fonction pour ajouter les infos d'un corps dans les tableaux
    void ajouter(const Corps& corps) {
        positions.push_back(corps.position);
        vitesses.push_back(corps.vitesse);
        masses.push_back(corps.masse);
        rayons.push_back(corps.rayon);

        accelerations.push_back(Vector3Double()); // vecteur nul au départ pour l'accélération
    }

    // Fonction pour connaître le nombre de corps dans l'univers
    std::size_t taille() const {
        return masses.size();
    }
};