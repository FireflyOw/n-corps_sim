#pragma once

#include "Univers.hpp"
#include "Vector3Double.hpp"

void calculAccelerations(Univers& univers);

void pasEuler(Univers& univers, double dt);
void pasEulerSymplectique(Univers& univers, double dt);
void pasLeapfrog(Univers& univers, double dt);

using Integrateur = void (*)(Univers&, double);
void simuler(Univers& univers, Integrateur integrateur, double dt);

double energieTotale(const Univers& univers);