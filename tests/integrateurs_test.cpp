#include <iostream>
#include <cmath>

#include "outils_test.hpp"
#include "physique.hpp"

// Type "pointeur de fonction" : une variable qui contient une fonction.
// Ça permet d'écrire un seul test et de le lancer avec chacun des intégrateurs.
using FonctionPas = void (*)(Univers&, double);

const double PI = 3.14159265358979323846;

// Une étoile de masse 1 au centre et une planète quasi sans masse à distance 1.
// Avec G = 1 :
//   - vitesseInitiale = 1   → orbite circulaire, de période 2π
//   - vitesseInitiale = 0.8 → orbite elliptique (plus exigeante pour les intégrateurs)
Univers creerSystemeEtoilePlanete(double vitesseInitiale) {
    Univers univers;
    univers.ajouter(Corps(1.0,   Vector3Double(0, 0, 0), Vector3Double(0, 0, 0)));                // étoile
    univers.ajouter(Corps(1e-10, Vector3Double(1, 0, 0), Vector3Double(0, vitesseInitiale, 0)));  // planète
    return univers;
}

// Trois corps de masses comparables, positions et vitesses quelconques
Univers creerSystemeTroisCorps() {
    Univers univers;
    univers.ajouter(Corps(1.0, Vector3Double( 0.0, 0.0, 0.0), Vector3Double( 0.0,  0.1, 0.0)));
    univers.ajouter(Corps(0.5, Vector3Double( 1.0, 0.0, 0.0), Vector3Double( 0.0,  0.8, 0.1)));
    univers.ajouter(Corps(0.3, Vector3Double(-0.5, 1.0, 0.2), Vector3Double( 0.3, -0.2, 0.0)));
    return univers;
}

// Fait avancer l'univers de nombrePas avec l'intégrateur donné.
// calculAccelerations est appelé une fois avant : c'est la condition du leapfrog
// (inutile mais sans danger pour les deux Euler).
void pasSimulation(Univers& univers, FonctionPas pas, double dt, int nombrePas) {
    calculAccelerations(univers);
    for (int numeroPas = 0; numeroPas < nombrePas; ++numeroPas) {
        pas(univers, dt);
    }
}

// Plus grand écart relatif d'énergie observé pendant la simulation : |E - E0| / |E0|
double deriveEnergieMaximale(FonctionPas pas, double dt, int nombrePas) {
    Univers univers = creerSystemeEtoilePlanete(0.8);
    calculAccelerations(univers);

    const double energieInitiale = energieTotale(univers);
    double ecartMaximal = 0.0;

    for (int numeroPas = 0; numeroPas < nombrePas; ++numeroPas) {
        pas(univers, dt);
        const double ecartRelatif = std::abs((energieTotale(univers) - energieInitiale) / energieInitiale);
        if (ecartRelatif > ecartMaximal) ecartMaximal = ecartRelatif;
    }
    return ecartMaximal;
}

// Position de la planète après une durée donnée
Vector3Double positionFinalePlanete(FonctionPas pas, double dt, double duree) {
    Univers univers = creerSystemeEtoilePlanete(0.8);
    const int nombrePas = static_cast<int>(std::lround(duree / dt));
    pasSimulation(univers, pas, dt, nombrePas);
    return univers.positions[1];
}

int main() {
    int echecs = 0;

    // ---------------------------------------------------------------------
    // Test 1 : le temps avance d'exactement dt par pas,
    // quel que soit le nombre de corps (ici 3) et l'intégrateur
    // ---------------------------------------------------------------------
    {
        const FonctionPas integrateurs[] = { pasEuler, pasEulerSymplectique, pasLeapfrog };
        for (FonctionPas pas : integrateurs) {
            Univers univers = creerSystemeTroisCorps();
            pasSimulation(univers, pas, 0.01, 10);
            verifier(egal(univers.temps, 0.1), "temps : 10 pas de 0.01 donnent 0.1", echecs);
        }
    }

    // ---------------------------------------------------------------------
    // Test 2 : énergie totale de cas calculables à la main
    // a) Deux masses 1 au repos à distance 1 : Ec = 0, Ep = -G·1·1/1 = -1
    // b) Même chose, mais un corps a une vitesse 2 : Ec = ½·1·2² = 2, donc E = 1
    // c) Orbite circulaire : énergie négative (système lié)
    // ---------------------------------------------------------------------
    {
        Univers univers;
        univers.ajouter(Corps(1, Vector3Double(0, 0, 0), Vector3Double()));
        univers.ajouter(Corps(1, Vector3Double(1, 0, 0), Vector3Double()));
        verifier(egal(energieTotale(univers), -1.0), "energie : deux masses au repos = -1", echecs);

        univers.vitesses[1] = Vector3Double(0, 2, 0);
        verifier(egal(energieTotale(univers), 1.0), "energie : avec vitesse 2, E = 2 - 1 = 1", echecs);

        Univers orbite = creerSystemeEtoilePlanete(1.0);
        verifier(energieTotale(orbite) < 0.0, "energie : une orbite fermee a une energie negative", echecs);
    }

    // ---------------------------------------------------------------------
    // Test 3 : orbite de Kepler circulaire avec le leapfrog
    // a) Après une période (2π), la planète revient à son point de départ
    // b) Pendant 10 orbites, la distance à l'étoile reste égale à 1
    //    (à ~dt² près : le leapfrog oscille légèrement autour du cercle, sans dériver)
    // ---------------------------------------------------------------------
    {
        const int pasParOrbite = 1000;
        const double dt = 2.0 * PI / pasParOrbite;

        Univers univers = creerSystemeEtoilePlanete(1.0);
        pasSimulation(univers, pasLeapfrog, dt, pasParOrbite);
        verifier(proche(univers.positions[1], Vector3Double(1, 0, 0), 1e-3), "kepler : retour au depart apres une periode", echecs);

        bool rayonConstant = true;
        for (int numeroPas = 0; numeroPas < 10 * pasParOrbite; ++numeroPas) {
            pasLeapfrog(univers, dt);
            const double distanceEtoile = (univers.positions[1] - univers.positions[0]).norme();
            if (!proche(distanceEtoile, 1.0, 1e-4)) rayonConstant = false;
        }
        verifier(rayonConstant, "kepler : rayon constant sur 10 orbites", echecs);
    }

    // ---------------------------------------------------------------------
    // Test 4 : conservation de l'énergie sur une orbite elliptique (20 000 pas)
    // Le leapfrog doit garder une erreur minuscule et bornée ;
    // Euler doit dériver beaucoup plus (c'est le cœur de l'expérience).
    // ---------------------------------------------------------------------
    {
        const double deriveEuler    = deriveEnergieMaximale(pasEuler,    0.001, 20000);
        const double deriveLeapfrog = deriveEnergieMaximale(pasLeapfrog, 0.001, 20000);

        verifier(deriveLeapfrog < 1e-5, "energie : le leapfrog conserve l'energie (< 1e-5)", echecs);
        verifier(deriveEuler > 1000.0 * deriveLeapfrog, "energie : Euler derive au moins 1000 fois plus", echecs);
    }

    // ---------------------------------------------------------------------
    // Test 5 : ordre de précision
    // On compare la position finale à une référence calculée avec un pas minuscule.
    // Diviser dt par 2 doit diviser l'erreur par 2 pour Euler (ordre 1)
    // et par 4 pour le leapfrog (ordre 2).
    // ---------------------------------------------------------------------
    {
        const double duree = 2.0;
        const Vector3Double reference = positionFinalePlanete(pasLeapfrog, 1e-5, duree);

        const double erreurEulerDt     = (positionFinalePlanete(pasEuler, 0.01,  duree) - reference).norme();
        const double erreurEulerDemiDt = (positionFinalePlanete(pasEuler, 0.005, duree) - reference).norme();
        const double rapportEuler = erreurEulerDt / erreurEulerDemiDt;

        const double erreurLeapfrogDt     = (positionFinalePlanete(pasLeapfrog, 0.01,  duree) - reference).norme();
        const double erreurLeapfrogDemiDt = (positionFinalePlanete(pasLeapfrog, 0.005, duree) - reference).norme();
        const double rapportLeapfrog = erreurLeapfrogDt / erreurLeapfrogDemiDt;

        verifier(proche(rapportEuler,    2.0, 0.5), "ordre : Euler est d'ordre 1 (erreur / 2)", echecs);
        verifier(proche(rapportLeapfrog, 4.0, 0.5), "ordre : leapfrog est d'ordre 2 (erreur / 4)", echecs);
    }

    // ---------------------------------------------------------------------
    // Test 6 : réversibilité du leapfrog
    // 500 pas en avant puis 500 pas avec -dt : on doit revenir exactement
    // au point de départ (aux erreurs d'arrondi près).
    // ---------------------------------------------------------------------
    {
        Univers univers = creerSystemeTroisCorps();
        const Univers etatInitial = univers;   // copie complète de l'univers

        pasSimulation(univers, pasLeapfrog,  0.001, 500);
        pasSimulation(univers, pasLeapfrog, -0.001, 500);

        bool retourAuDepart = true;
        for (std::size_t indiceCorps = 0; indiceCorps < univers.taille(); ++indiceCorps) {
            if (!proche(univers.positions[indiceCorps], etatInitial.positions[indiceCorps], 1e-12)) retourAuDepart = false;
            if (!proche(univers.vitesses[indiceCorps],  etatInitial.vitesses[indiceCorps],  1e-12)) retourAuDepart = false;
        }
        verifier(retourAuDepart, "reversibilite : le leapfrog revient au depart", echecs);
        verifier(proche(univers.temps, 0.0, 1e-12), "reversibilite : le temps revient a 0", echecs);
    }

    // ---------------------------------------------------------------------
    // Test 7 : conservation de la quantité de mouvement totale (Σ m·v)
    // Grâce au += / -= de la boucle décalée, elle doit rester constante
    // pendant toute la simulation, avec n'importe quel intégrateur.
    // ---------------------------------------------------------------------
    {
        Univers univers = creerSystemeTroisCorps();

        auto quantiteDeMouvementTotale = [](const Univers& systeme) {
            Vector3Double somme;
            for (std::size_t indiceCorps = 0; indiceCorps < systeme.taille(); ++indiceCorps) {
                somme += systeme.vitesses[indiceCorps] * systeme.masses[indiceCorps];
            }
            return somme;
        };

        const Vector3Double quantiteInitiale = quantiteDeMouvementTotale(univers);
        pasSimulation(univers, pasLeapfrog, 0.001, 1000);

        verifier(proche(quantiteDeMouvementTotale(univers), quantiteInitiale, 1e-12), "quantite de mouvement conservee", echecs);
    }

    if (echecs == 0) std::cout << "Tous les tests passent\n";
    return echecs;
}