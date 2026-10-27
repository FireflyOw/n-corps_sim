#include "camera.hpp"

// Convertit une position de la simulation (double, unités physiques)
// en position à l'écran (float, pixels), en vue de dessus (plan x, y)
Vector2 versEcran(const Vector3Double& positionSimulation, float pixelsParUnite, Vector2 centreEcran) {
    return Vector2 {
        centreEcran.x + static_cast<float>(positionSimulation.x) * pixelsParUnite,
        centreEcran.y - static_cast<float>(positionSimulation.y) * pixelsParUnite   // signe moins!
    };
}

Vector2 versSimulation(const Vector3Double& positionSimulation, float pixelsParUnite, Vector2 centreEcran) {
    
}