#pragma once

#include "univers.hpp"
#include "vector3double.hpp"

void calculAccelerations(Univers& univers);

void pasEuler(Univers& univers, double dt);
void pasEulerSymplectique(Univers& univers, double dt);
void pasLeapfrog(Univers& univers, double dt);

double energieTotale(const Univers& univers);