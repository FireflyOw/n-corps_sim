// --------------------------
// Lancement : ./build/nbody
// --------------------------

#include "physique.hpp"
#include "controles.hpp"
#include "simulation.hpp"
#include "interface.hpp"

#include <chrono>
#include <algorithm>

int main() {
    // ---------------------------------------------------------------------
    //                     Réglages de la fenêtre raylib 
    // ---------------------------------------------------------------------
    const int largeur = 1280;
    const int hauteur = 720;

    bool quitter = false;

    Vector2 centreEcran = { largeur / 2.0f, hauteur / 2.0f };
    CameraSim camera(centreEcran);

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(largeur, hauteur, "Simulation N-Corps");
    SetTargetFPS(60);

    // ---------------------------------------------------------------------
    //               Réglages de l'interface ImGui et police
    // ---------------------------------------------------------------------
    rlImGuiSetup(true);

    ImGuiStyle& style = ImGui::GetStyle();
    style.FontScaleMain = 0.9f;      // taille du texte
    style.ScaleAllSizes(1.0f);       // marges, curseurs, cases

    // On prend en compte l'échelle de l'affichage (120% dans mon cas, ça renvoie 1.2)
    Vector2 facteurEchelle = GetWindowScaleDPI();
    
    const float fontSize = 21.0f / facteurEchelle.x;
    int echelleTexte = static_cast<int>(std::round(fontSize * facteurEchelle.x));

    // Chargement de la police d'écriture personnalisée à l'échelle avec un filtre pour la netteté
    Font googleSans = LoadFontEx("resources/fonts/Google_Sans/GoogleSans-Regular.ttf", echelleTexte, 0, 0);
    SetTextureFilter(googleSans.texture, TEXTURE_FILTER_POINT);

    // ---------------------------------------------------------------------
    //                       Réglages de la simulation
    // ---------------------------------------------------------------------
    Unites unites = {constantes::uniteAstronomique_m, constantes::masseSolaire_kg};

    bool rayonTest = false;
    const double dt = unites.versTemps(3600.0);

    std::vector<Corps> condInitiales = {
        corpsSI(unites, constantes::masseSolaire_kg, 6.957e8,  double3(0, 0, 0),         double3(0, 0, 0),     Couleur{1.0f, 1.0f, 0.0f}),           // Soleil
        corpsSI(unites, 3.301e23,                    2.4397e6, double3(5.791e10, 0, 0),  double3(0, 47872, 0), Couleur{1.0f, 0.5f, 0.5f}),           // Mercure
        corpsSI(unites, 4.867e24,                    6.0518e6, double3(1.0821e11, 0, 0), double3(0, 35021, 0), Couleur{0.5725f, 0.5216f, 0.5647f}),  // Venus
        corpsSI(unites, 5.972e24,                    6.371e6,  double3(1.496e11, 0, 0),  double3(0, 29785, 0), Couleur{0.0f, 1.0f, 1.0f}),           // Terre
        corpsSI(unites, 6.417e23,                    3.3895e6, double3(2.2794e11, 0, 0), double3(0, 24130, 0), Couleur{1.0f, 0.0f, 0.0f}),           // Mars

        corpsSI(unites, 1.898e27,                    6.9911e7, double3(7.7857e11, 0, 0), double3(0, 13056, 0), Couleur{0.6627f, 0.3255f, 0.0941f})   // Jupiter
    };

    Simulation sim(unites, condInitiales, dt);

    // ---------------------------------------------------------------------
    //                          Boucle principale
    // ---------------------------------------------------------------------

    auto debutSim = std::chrono::steady_clock::now();

    while(!WindowShouldClose() && !quitter) {
        auto frameStart = std::chrono::steady_clock::now();

        // -------------------- Bloc physique / contrôles --------------------
        sim.avancer();

        gestionControles(sim, camera);
        if (camera.suiviActif) camera.positionVisee = sim.univers.positions[camera.iCorpsSuivi];

        // -------------------------- Bloc affichage --------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        Vector2 largeurPAUSED = MeasureTextEx(googleSans, "PAUSED", fontSize, 2);
        if (sim.simPause) DrawTextEx(googleSans, "PAUSED", {(largeur / 2) - (largeurPAUSED.x / 2), hauteur - 30}, fontSize, 2, WHITE);

        // Dessin d'un cercle coloré pour chaque corps de la liste
        for (std::size_t iCorps = 0; iCorps < sim.univers.taille(); ++iCorps) {
            Vector2 positionEcran = camera.versEcran(sim.univers.positions[iCorps]);

            // Conversion de la couleur du corps dans le format utilisé par raylib
            Vector4 couleurBase = {sim.univers.couleurs[iCorps].r, sim.univers.couleurs[iCorps].g, sim.univers.couleurs[iCorps].b, 1};
            Color couleur = ColorFromNormalized(couleurBase); // fonction de conversion donnée

            for (std::size_t j = 0; j + 1 < sim.trainees[iCorps].taille(); ++j) {
                DrawLineV(camera.versEcran(sim.trainees[iCorps][j]), camera.versEcran(sim.trainees[iCorps][j + 1]), couleur);
            }

            double rayonAffiche = std::max(sim.univers.rayons[iCorps] * camera.zoom, 1.0);

            if (rayonTest) DrawCircleV(positionEcran, 5, couleur);
            else DrawCircleV(positionEcran, rayonAffiche, couleur);
        }

        // Calcul du temps depuis le lancement de la simulation (secondes réelles)
        auto tempsActuel = std::chrono::steady_clock::now();
        float tempsTotal  = std::chrono::duration<float, std::milli>(tempsActuel - debutSim).count();

        // Calcul et affichage du frametime (temps pour créer une image)
        auto frameEnd  = std::chrono::steady_clock::now();
        auto frametime = std::chrono::duration<float, std::milli>(frameEnd - frameStart);

        quitter = dessinerInterface(sim, camera, unites, tempsTotal);
        EndDrawing();
    }

    UnloadFont(googleSans);
    rlImGuiShutdown();
    CloseWindow();
}