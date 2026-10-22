#pragma once

#include "univers.hpp"
#include "Vector3Double.hpp"

void calculAccelerations(Univers& univers);

void pasEuler(Univers& univers, double dt);
void pasEulerSymplectique(Univers& univers, double dt);
void pasLeapfrog(Univers& univers, double dt);

double energieTotale(const Univers& univers);