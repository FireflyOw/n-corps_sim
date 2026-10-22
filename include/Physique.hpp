#pragma once

#include "Univers.hpp"
#include "Vector3Double.hpp"

void calculAccelerations(Univers& univers);

void Euler(Univers& univers, double dt);
void EulerSymplectique(Univers& univers, double dt);
void Leapfrog(Univers& univers, double dt);

double energieTotale(const Univers& univers);