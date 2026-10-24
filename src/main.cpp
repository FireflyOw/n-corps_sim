// Lancement : ./build/nbody

#include "raylib.h"
#include "physique.hpp"

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

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(largeur, hauteur, "Simulation N-Corps");
    SetTargetFPS(60);

    // ----------------- Réglages de l'univers -----------------

    Univers univers;

    std::vector<Corps> listeCorps = {
        Corps(1, Vector3Double(0, 0, 0), Vector3Double(0, 0, 0)),
        Corps(1e-10, Vector3Double(1, 0, 0), Vector3Double(0, 1, 0))
    };

    for (const Corps& c : listeCorps) univers.ajouter(c);

    const double dt = 0.001;
    const float scale = 200; // en (pixel / unité)
    std::size_t pasImage = 10;

    double energieDepart = energieTotale(univers);

    // ------------------- Boucle principale -------------------

    while(!WindowShouldClose()) {
        for (std::size_t i = 0; i < pasImage; ++i) {
            pasEuler(univers, dt);
        }

        double eTotale = energieTotale(univers); // valeur d'énergie totale du système

        // Calcul de l'erreur relative d'énergie du système pa rapport au départ
        double erreurEnergie = (energieDepart - eTotale) / energieDepart;

        BeginDrawing();
        ClearBackground(BLACK);

        // Dessin d'un cercle blanc pour chaque corps de la liste
        for (std::size_t iCorps = 0; iCorps < univers.taille(); ++iCorps) {
            Vector2 positionEcran = versEcran(univers.positions[iCorps], scale, centreEcran);
            DrawCircleV(positionEcran, 5, WHITE);
        }

        DrawText(TextFormat("temps = %.2f", univers.temps),  10, 50, 15, WHITE);
        DrawText(TextFormat("e_totale = %.2f J", eTotale),   10, 65, 15, WHITE);
        DrawText(TextFormat("Erreur = %.3f", erreurEnergie), 10, 80, 15, WHITE);

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
}