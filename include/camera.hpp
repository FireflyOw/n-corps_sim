#pragma once

#include "raylib.h"
#include "vecteurs.hpp"

struct CameraSim {
    Vector2 centreEcran;
    double3 positionVisee;
    
    double zoom = 200;
    double facteurZoom = 1.2;

    std::size_t iCorpsSuivi = 0;
    bool suiviActif = false;
    bool enMouvement = false;

    CameraSim(Vector2 centreEcran) : centreEcran(centreEcran) {}

    // Convertit une position de la simulation (double, unités physiques)
    // en position à l'écran (float, pixels), en vue de dessus (plan x, y)
    Vector2 versEcran(const double3& positionSimulation) const {
        return Vector2 {
            centreEcran.x + static_cast<float>((positionSimulation.x - positionVisee.x) * zoom),
            centreEcran.y - static_cast<float>((positionSimulation.y - positionVisee.y) * zoom)   // signe moins!
        };
    }

    void deplacement(const Vector2& deplacementSouris) {
        positionVisee.x -= (deplacementSouris.x / zoom);
        positionVisee.y += (deplacementSouris.y / zoom);
    }
};