// --------------------------
// Lancement : ./build/nbody
// --------------------------

#include "raylib.h"
#include "physique.hpp"

#include <chrono>

// Convertit une position de la simulation (double, unités physiques)
// en position à l'écran (float, pixels), en vue de dessus (plan x, y)
Vector2 versEcran(const Vector3Double& positionSimulation, float pixelsParUnite, Vector2 centreEcran) {
    return Vector2 {
        centreEcran.x + static_cast<float>(positionSimulation.x) * pixelsParUnite,
        centreEcran.y - static_cast<float>(positionSimulation.y) * pixelsParUnite   // signe moins!
    };
}

int main() {
    // ---------------------------------------------------------------------
    //                     Réglages de la fenêtre raylib 
    // ---------------------------------------------------------------------
    const int largeur = 800;
    const int hauteur = 600;

    const Vector2 centreEcran = { largeur / 2.0f, hauteur / 2.0f };

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(largeur, hauteur, "Simulation N-Corps");
    SetTargetFPS(60);

    const float fontSize = 21.0f;
    Font googleSans = LoadFontEx("resources/fonts/Google_Sans/GoogleSans-Regular.ttf", fontSize, 0, 0);
    SetTextureFilter(googleSans.texture, TEXTURE_FILTER_POINT);

    // ---------------------------------------------------------------------
    //                         Réglages de l'univers
    // ---------------------------------------------------------------------
    Univers univers;

    std::vector<Corps> listeCorps = {
        Corps(1,     Vector3Double(0, 0, 0), Vector3Double(0, 0, 0)),
        Corps(1e-10, Vector3Double(1, 0, 0), Vector3Double(0, 1, 0))
    };

    for (const Corps& c : listeCorps) univers.ajouter(c);

    const double dt = 0.001;
    const float scale = 200; // en (pixel / unité)
    std::size_t pasImage = 10;

    calculAccelerations(univers);
    double energieDepart = energieTotale(univers);

    // ---------------------------------------------------------------------
    //                          Boucle principale
    // ---------------------------------------------------------------------

    while(!WindowShouldClose()) {
        auto frameStart = std::chrono::steady_clock::now();

        // -------------------------- Bloc physique --------------------------
        for (std::size_t i = 0; i < pasImage; ++i) {
            pasLeapfrog(univers, dt);
        }

        double eTotale = energieTotale(univers); // valeur d'énergie totale du système

        // Calcul de l'erreur relative d'énergie du système pa rapport au départ
        double erreurEnergie = (energieDepart - eTotale) / energieDepart;

        // -------------------------- Bloc affichage --------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        // Dessin d'un cercle blanc pour chaque corps de la liste
        for (std::size_t iCorps = 0; iCorps < univers.taille(); ++iCorps) {
            Vector2 positionEcran = versEcran(univers.positions[iCorps], scale, centreEcran);
            DrawCircleV(positionEcran, 5, WHITE);
        }

        // Affichages d'infos (temps de sim., énergie totale, etc...)
        DrawTextEx(googleSans, TextFormat("temps = %.2f", univers.temps),  {10, 50},                fontSize, 2, WHITE);
        DrawTextEx(googleSans, TextFormat("e_totale = %.2e J", eTotale),   {10, 50 + fontSize},     fontSize, 2, WHITE);
        DrawTextEx(googleSans, TextFormat("Erreur = %.2e", erreurEnergie), {10, 50 + 2 * fontSize}, fontSize, 2, WHITE);

        DrawFPS(10, 10);

        auto frameEnd  = std::chrono::steady_clock::now();
        auto frametime = std::chrono::duration<float, std::milli>(frameEnd - frameStart);

        DrawTextEx(googleSans, TextFormat("Frametime = %.2f ms", frametime.count()), {10, 50 + 3 * fontSize}, fontSize, 2, WHITE);

        EndDrawing();
    }

    CloseWindow();
    UnloadFont(googleSans);
}