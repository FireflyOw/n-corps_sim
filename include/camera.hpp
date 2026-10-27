#pragma once

#include "raylib.h"
#include "camera.hpp"
#include "vector3double.hpp"

struct CameraSim {
    const int largeur = 800;
    const int hauteur = 600;

    const Vector2 centreEcran = { largeur / 2.0f, hauteur / 2.0f };
    
    double zoom;
    Vector3Double positionEcran;

    // Convertit une position de la simulation (double, unités physiques)
    // en position à l'écran (float, pixels), en vue de dessus (plan x, y)
    Vector2 versEcran(const Vector3Double& positionSimulation, float pixelsParUnite, Vector2 centreEcran) const {
        return Vector2 {
            centreEcran.x + static_cast<float>(positionSimulation.x) * pixelsParUnite,
            centreEcran.y - static_cast<float>(positionSimulation.y) * pixelsParUnite   // signe moins!
        };
    }

    Vector2 versSimulation(const Vector3Double& positionEcran, float pixelsParUnite, Vector2 centreEcran) const {

    }
};