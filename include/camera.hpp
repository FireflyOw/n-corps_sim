#pragma once

#include "raylib.h"
#include "vector3double.hpp"

// Convertit une position de la simulation (double, unités physiques)
// en position à l'écran (float, pixels), en vue de dessus (plan x, y)
Vector2 versEcran(const Vector3Double& positionSimulation, float pixelsParUnite, Vector2 centreEcran);

Vector2 versSimulation(const Vector3Double& positionSimulation, float pixelsParUnite, Vector2 centreEcran);