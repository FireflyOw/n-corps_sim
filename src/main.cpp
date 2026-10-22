#include "raylib.h"
#include "Physique.hpp"

// Convertit une position de la simulation (double, unités physiques)
// en position à l'écran (float, pixels), en vue de dessus (plan x, y)
Vector2 versEcran(const Vector3Double& positionSimulation, float pixelsParUnite, Vector2 centreEcran) {
    return Vector2 {
        centreEcran.x + static_cast<float>(positionSimulation.x) * pixelsParUnite,
        centreEcran.y - static_cast<float>(positionSimulation.y) * pixelsParUnite   // signe moins!
    };
}

int main() {
    // ----------------- Réglages de la fenêtre -----------------
    const int largeur = 800;
    const int hauteur = 600;

    const Vector2 centreEcran = { largeur / 2.0f, hauteur / 2.0f };

    InitWindow(largeur, hauteur, "Simulation N-Corps");
    SetTargetFPS(60);

    // ----------------- Réglages de l'univers -----------------

    Univers univers;

    std::vector<Corps> listeCorps = {
        Corps(1, Vector3Double(0, 0, 0), Vector3Double(0, 0, 0)),
        Corps(1e-10, Vector3Double(1, 0, 0), Vector3Double(0, 1, 0))
    };

    for (const Corps& c : listeCorps) univers.ajouter(c);

    double dt = 0.001;
    const float scale = 200; // en (pixel / unité)
    std::size_t pasImage = 10;

    calculAccelerations(univers);

    // ------------------- Boucle principale -------------------

    while(!WindowShouldClose()) {
        for (std::size_t i = 0; i < pasImage; ++i) {
            pasLeapfrog(univers, dt);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (std::size_t iCorps = 0; iCorps < univers.taille(); ++iCorps) {
            Vector2 positionEcran = versEcran(univers.positions[iCorps], scale, centreEcran);
            DrawCircleV(positionEcran, 5, WHITE);
        }

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
}