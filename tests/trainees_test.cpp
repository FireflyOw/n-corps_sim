// Tests des traînées d'orbite dans Simulation (include/simulation.hpp)
//
// Système de test : une étoile de masse 1 et une planète en orbite circulaire
// de rayon 1, en unités internes (G = 1). La période vaut donc exactement 2π
// et la planète tourne à 1 radian par unité de temps : l'angle parcouru est
// égal au temps écoulé, ce qui permet de mesurer ce que couvre une traînée.

#include "outils_test.hpp"
#include "simulation.hpp"

#include <cmath>
#include <vector>

std::vector<Corps> creerSystemeTest() {
    return {
        Corps(1.0,   0.0, double3(0, 0, 0), double3(0, 0, 0), Couleur{1.0f, 1.0f, 0.0f}),
        Corps(1e-10, 0.0, double3(1, 0, 0), double3(0, 1, 0), Couleur{0.0f, 0.0f, 1.0f})
    };
}

// Angle parcouru entre deux positions de l'orbite (dans le sens de rotation, entre 0 et 2π)
double angleParcouru(const double3& depart, const double3& arrivee) {
    const double deuxPi = 2.0 * std::acos(-1.0);
    double angle = std::atan2(arrivee.y, arrivee.x) - std::atan2(depart.y, depart.x);
    if (angle < 0.0) angle += deuxPi;
    return angle;
}

int main() {
    int echecs = 0;

    const double PI     = std::acos(-1.0);
    const double deuxPi = 2.0 * PI;
    const Unites unites{1.495978707e11, 1.98847e30};
    const std::vector<Corps> systeme = creerSystemeTest();
    const double dt = 0.0002;   // petit pas : l'intervalle vaut ~56 pas, l'arrondi au pas reste < 1 %

    // --- Test 1 : tableaux de la bonne taille après construction -------------
    {
        Simulation simulation(unites, systeme, dt);
        verifier(simulation.trainees.size()             == 2, "construction : un tampon par corps", echecs);
        verifier(simulation.intervalleTrainees.size()   == 2, "construction : un intervalle par corps", echecs);
        verifier(simulation.dernierPointTrainees.size() == 2, "construction : un dernier point par corps", echecs);
        verifier(simulation.trainees[1].vide(),               "construction : tampon vide", echecs);
        verifier(simulation.trainees[1].capacite() == simulation.longueurTrainees,
                 "construction : capacite = longueurTrainees", echecs);
    }

    // --- Test 2 : valeur de l'intervalle -------------------------------------
    // T = 2π (orbite de rayon 1 autour d'une masse 1) ; intervalle = fraction × T / nombre de points.
    {
        Simulation simulation(unites, systeme, dt);
        double attendu = simulation.fractionOrbite * deuxPi / simulation.longueurTrainees;
        verifier(proche(simulation.intervalleTrainees[1], attendu, 1e-9), "intervalle de la planete = fraction x T / N", echecs);
        verifier(egal(simulation.intervalleTrainees[0], simulation.intervalleTrainees[1]),
                 "intervalle de l'etoile = celui de la planete (le plus grand)", echecs);
    }

    // --- Test 3 : nombre de points enregistrés après un demi-tour ------------
    // Les points sont enregistrés aux pas de physique, donc l'intervalle réel est
    // l'intervalle théorique arrondi au multiple de dt supérieur.
    {
        Simulation simulation(unites, systeme, dt);
        double intervalleReel = std::ceil(simulation.intervalleTrainees[1] / dt - 1e-9) * dt;

        while (simulation.univers.temps < PI) simulation.avancer();

        double attendu = std::floor(simulation.univers.temps / intervalleReel);
        double obtenu  = static_cast<double>(simulation.trainees[1].taille());
        verifier(std::abs(obtenu - attendu) <= 2.0, "demi-tour : nombre de points attendu", echecs);
    }

    // --- Test 4 : après plusieurs tours, le tampon est plein et couvre ~90 % de l'orbite
    {
        Simulation simulation(unites, systeme, dt);
        while (simulation.univers.temps < 3 * deuxPi) simulation.avancer();

        const TamponCirculaire<double3>& trainee = simulation.trainees[1];
        verifier(trainee.plein(), "3 tours : tampon plein", echecs);

        double angleCouvert = angleParcouru(trainee[0], trainee[trainee.taille() - 1]);
        double angleVise    = simulation.fractionOrbite * deuxPi;
        double tolerance    = angleVise * (dt / simulation.intervalleTrainees[1] + 0.01);
        verifier(proche(angleCouvert, angleVise, tolerance), "3 tours : la traînee couvre ~90 % de l'orbite", echecs);
    }

    // --- Test 5 : en pause, aucun point n'est ajouté -------------------------
    {
        Simulation simulation(unites, systeme, dt);
        for (int iImage = 0; iImage < 300; ++iImage) simulation.avancer();
        std::size_t pointsAvant = simulation.trainees[1].taille();

        simulation.simPause = true;
        for (int iImage = 0; iImage < 300; ++iImage) simulation.avancer();

        verifier(pointsAvant > 0, "pause : des points existaient avant la pause", echecs);
        verifier(simulation.trainees[1].taille() == pointsAvant, "pause : aucun point ajoute", echecs);
    }

    // --- Test 6 : reinitialiser() remet les traînées à zéro ------------------
    {
        Simulation simulation(unites, systeme, dt);
        for (int iImage = 0; iImage < 300; ++iImage) simulation.avancer();
        simulation.reinitialiser();
        simulation.reinitialiser();

        verifier(simulation.trainees.size() == 2,              "reset : toujours un tampon par corps", echecs);
        verifier(simulation.trainees[1].vide(),                "reset : tampon vide", echecs);
        verifier(egal(simulation.dernierPointTrainees[1], 0.0), "reset : dernier point remis a zero", echecs);

        // Et la simulation enregistre de nouveau normalement après le reset
        for (int iImage = 0; iImage < 300; ++iImage) simulation.avancer();
        verifier(!simulation.trainees[1].vide(), "reset : enregistrement repris", echecs);
    }

    // --- Test 7 : un seul corps (pas de planète pour fixer l'intervalle du Soleil)
    {
        const std::vector<Corps> seul = { systeme[0] };
        Simulation simulation(unites, seul, dt);
        for (int iImage = 0; iImage < 10; ++iImage) simulation.avancer();
        verifier(simulation.trainees.size() == 1, "un seul corps : ne plante pas", echecs);
    }

    if (echecs == 0) std::cout << "Trainees : tous les tests sont passes\n";
    return echecs == 0 ? 0 : 1;
}