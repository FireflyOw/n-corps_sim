// Tests de la structure Simulation (include/simulation.hpp)
//
// Simulation ne dépend pas de raylib : on peut vérifier son comportement
// sans ouvrir de fenêtre. On teste surtout ce qui a déjà cassé une fois :
// la réinitialisation (tableaux qui gonflent, temps pas remis à zéro...).

#include "outils_test.hpp"
#include "simulation.hpp"

#include <vector>

// Petit système étoile + planète en orbite circulaire, en unités internes
// (G = 1, masse de l'étoile = 1, rayon de l'orbite = 1, vitesse = 1).
std::vector<Corps> creerSystemeTest() {
    return {
        Corps(1.0,   0.0, double3(0, 0, 0), double3(0, 0, 0), Couleur{1.0f, 1.0f, 0.0f}),
        Corps(1e-10, 0.0, double3(1, 0, 0), double3(0, 1, 0), Couleur{0.0f, 0.0f, 1.0f})
    };
}

// Vérifie que tous les tableaux de l'univers ont la même taille que le nombre de corps
bool tableauxCoherents(const Univers& univers, std::size_t nombreAttendu) {
    return univers.taille()               == nombreAttendu
        && univers.positions.size()       == nombreAttendu
        && univers.vitesses.size()        == nombreAttendu
        && univers.accelerations.size()   == nombreAttendu
        && univers.masses.size()          == nombreAttendu
        && univers.rayons.size()          == nombreAttendu
        && univers.couleurs.size()        == nombreAttendu;
}

int main() {
    int echecs = 0;

    // Unités du système solaire (seules les conversions d'affichage en dépendent)
    const Unites unites{1.495978707e11, 1.98847e30};
    const std::vector<Corps> systeme = creerSystemeTest();
    const double dt = 0.001;

    // --- Test 1 : état juste après la construction ---------------------------
    // Le constructeur appelle reinitialiser() : la simulation doit être prête.
    {
        Simulation simulation(unites, systeme, dt);

        verifier(tableauxCoherents(simulation.univers, 2), "construction : tableaux coherents", echecs);
        verifier(egal(simulation.univers.temps, 0.0), "construction : temps a zero", echecs);
        verifier(egal(simulation.calculErreurEnergie(), 0.0), "construction : erreur d'energie nulle", echecs);
        // Énergie d'une orbite circulaire en unités internes : -G·M·m / (2r) = -0.5e-10
        verifier(proche(simulation.energieDepart, -0.5e-10, 1e-20), "construction : energie de depart correcte", echecs);
    }

    // --- Test 2 : avancer() fait avancer le temps de pasImage × dt ------------
    {
        Simulation simulation(unites, systeme, dt);
        simulation.pasImage = 10;
        simulation.avancer();

        verifier(proche(simulation.univers.temps, 10 * dt, 1e-12), "avancer : temps = pasImage * dt", echecs);
        verifier(!egal(simulation.univers.positions[1].y, 0.0), "avancer : la planete a bouge", echecs);
    }

    // --- Test 3 : en pause, avancer() ne change rien --------------------------
    {
        Simulation simulation(unites, systeme, dt);
        simulation.simPause = true;
        double3 positionAvant = simulation.univers.positions[1];
        simulation.avancer();

        verifier(egal(simulation.univers.temps, 0.0), "pause : le temps ne bouge pas", echecs);
        verifier(egal(simulation.univers.positions[1], positionAvant), "pause : la planete ne bouge pas", echecs);
    }

    // --- Test 4 : reinitialiser() remet tout à l'état initial -----------------
    // On avance, puis on réinitialise PLUSIEURS fois : c'est ce qui révélait
    // le bug des accélérations (le tableau grossissait à chaque appel).
    {
        Simulation simulation(unites, systeme, dt);
        for (int iReinit = 0; iReinit < 3; ++iReinit) {
            for (int iImage = 0; iImage < 50; ++iImage) simulation.avancer();
            simulation.reinitialiser();
        }

        verifier(tableauxCoherents(simulation.univers, 2), "reinitialiser : tableaux coherents apres 3 appels", echecs);
        verifier(egal(simulation.univers.temps, 0.0), "reinitialiser : temps a zero", echecs);
        verifier(egal(simulation.univers.positions[1], double3(1, 0, 0)), "reinitialiser : position initiale", echecs);
        verifier(egal(simulation.univers.vitesses[1],  double3(0, 1, 0)), "reinitialiser : vitesse initiale", echecs);
        verifier(egal(simulation.calculErreurEnergie(), 0.0), "reinitialiser : erreur d'energie nulle", echecs);
    }

    // --- Test 5 : réinitialiser EN PAUSE remet aussi le temps à zéro ----------
    // C'est le cas qui affichait un temps périmé quand le temps était stocké
    // dans une variable au lieu d'être calculé par une méthode.
    {
        Simulation simulation(unites, systeme, dt);
        for (int iImage = 0; iImage < 50; ++iImage) simulation.avancer();
        simulation.simPause = true;
        simulation.reinitialiser();

        verifier(egal(simulation.calculTempsSim(), 0.0), "reinitialiser en pause : temps affiche a zero", echecs);
    }

    // --- Test 6 : l'énergie reste conservée après plusieurs images ------------
    // 100 images de 10 pas : l'erreur relative doit rester très petite avec leapfrog.
    {
        Simulation simulation(unites, systeme, dt);
        for (int iImage = 0; iImage < 100; ++iImage) simulation.avancer();

        verifier(std::abs(simulation.calculErreurEnergie()) < 1e-8, "energie conservee sur 1000 pas", echecs);
    }

    if (echecs == 0) std::cout << "Simulation : tous les tests sont passes\n";
    return echecs == 0 ? 0 : 1;
}