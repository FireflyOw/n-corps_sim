#pragma once

#include "raylib.h"
#include "vector3double.hpp"

struct CameraSim {
    Vector2 centreEcran;
    
    double zoom = 200;
    double facteurZoom = 1.2;
    Vector3Double positionVisee;

    CameraSim(Vector2 centreEcran) : centreEcran(centreEcran) {}

    // Convertit une position de la simulation (double, unités physiques)
    // en position à l'écran (float, pixels), en vue de dessus (plan x, y)
    Vector2 versEcran(const Vector3Double& positionSimulation) const {
        return Vector2 {
            centreEcran.x + static_cast<float>((positionSimulation.x - positionVisee.x) * zoom),
            centreEcran.y - static_cast<float>((positionSimulation.y - positionVisee.y) * zoom)   // signe moins!
        };
    }

//    Vector2 versSimulation(const Vector3Double& positionEcran, float pixelsParUnite) const {
//        return Vector2 {0, 0};
//    }
};