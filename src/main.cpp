// --------------------------
// Lancement : ./build/nbody
// --------------------------

#include "camera.hpp"
#include "unites.hpp"
#include "physique.hpp"

#include <chrono>
#include <algorithm>

int main() {
    // ---------------------------------------------------------------------
    //                     Réglages de la fenêtre raylib 
    // ---------------------------------------------------------------------
    const int largeur = 800;
    const int hauteur = 600;

    Vector2 centreEcran = { largeur / 2.0f, hauteur / 2.0f };
    CameraSim camera(centreEcran);

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(largeur, hauteur, "Simulation N-Corps");
    SetTargetFPS(60);

    // On prend en compte l'échelle de l'affichage (120% dans mon cas, ça renvoie 1.2)
    Vector2 facteurEchelle = GetWindowScaleDPI();
    
    const float fontSize = 21.0f / facteurEchelle.x;
    int echelleTexte = static_cast<int>(std::round(fontSize * facteurEchelle.x));

    // Chargement de la police d'écriture personnalisée à l'échelle avec un filtre pour la netteté
    Font googleSans = LoadFontEx("resources/fonts/Google_Sans/GoogleSans-Regular.ttf", echelleTexte, 0, 0);
    SetTextureFilter(googleSans.texture, TEXTURE_FILTER_POINT);

    // ---------------------------------------------------------------------
    //                         Réglages de l'univers
    // ---------------------------------------------------------------------
    Univers univers;

    Unites unites = {constantes::uniteAstronomique_m, constantes::masseSolaire_kg};

    // Conditions initiales du système solaire
    std::vector<Corps> condInitiales = {
        corpsSI(unites, constantes::masseSolaire_kg, 6.957e8,  double3(0, 0, 0),         double3(0, 0, 0),     Couleur{1.0f, 1.0f, 0.0f}),           // Soleil
        corpsSI(unites, 3.301e23,                    2.4397e6, double3(5.791e10, 0, 0),  double3(0, 47872, 0), Couleur{1.0f, 0.5f, 0.5f}),           // Mercure
        corpsSI(unites, 4.867e24,                    6.0518e6, double3(1.0821e11, 0, 0), double3(0, 35021, 0), Couleur{0.5725f, 0.5216f, 0.5647f}),  // Venus
        corpsSI(unites, 5.972e24,                    6.371e6,  double3(1.496e11, 0, 0),  double3(0, 29785, 0), Couleur{0.0f, 1.0f, 1.0f}),           // Terre
        corpsSI(unites, 6.417e23,                    3.3895e6, double3(2.2794e11, 0, 0), double3(0, 24130, 0), Couleur{1.0f, 0.0f, 0.0f}),           // Mars

        corpsSI(unites, 1.898e27,                    6.9911e7, double3(7.7857e11, 0, 0), double3(0, 13056, 0), Couleur{0.6627f, 0.3255f, 0.0941f})   // Jupiter
    };

    const double dt = unites.versTemps(3600.0); // en secondes
    std::size_t pasImage = 10;

    // ---------------------------------------------------------------------
    //                       Réglages de la simulation
    // ---------------------------------------------------------------------
    bool rayonTest = false;
    bool simPause = false;

    initUnivers(univers, condInitiales);
    
    // Initialisation des valeurs d'énergie
    double energieDepart = energieTotale(univers);
    double eTotale       = energieDepart;
    double erreurEnergie = (energieDepart - eTotale) / energieDepart;

    double energieReel = eTotale * unites.energie_J();
    double tempsSim    = unites.versSecondes(univers.temps) / constantes::secondesParJour;

    // ---------------------------------------------------------------------
    //                          Boucle principale
    // ---------------------------------------------------------------------

    auto debutSim = std::chrono::steady_clock::now();

    while(!WindowShouldClose()) {
        auto frameStart = std::chrono::steady_clock::now();

        // -------------------------- Bloc contrôles --------------------------

        // Gestion de la vitesse de simulation avec les flèches
        if (IsKeyPressed(KEY_UP)) pasImage *= 2;
        else if (IsKeyPressed(KEY_DOWN)) pasImage /= 2;

        pasImage = std::clamp<std::size_t>(pasImage, 1, 100);

        // Gestion du zoom avec la molette
        camera.zoom *= std::pow(camera.facteurZoom, GetMouseWheelMove());
        camera.zoom = std::clamp<double>(camera.zoom, 10, 1e7);

        // Gestion de la pause avec Espace
        if (IsKeyPressed(KEY_SPACE)) simPause = !simPause;

        // Gestion du déplacement à la souris
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) camera.deplacement(GetMouseDelta());

        // Réinitialisation de la vue
        if (IsKeyPressed(KEY_C)) {
            camera.positionVisee = double3();
            camera.zoom = 200;
        }

        // Réinitialisation de la simulation avec R
        if (IsKeyPressed(KEY_R)) {
            initUnivers(univers, condInitiales);

            energieDepart = energieTotale(univers);
            eTotale       = energieDepart;
            erreurEnergie = (energieDepart - eTotale) / energieDepart;   
        }

        // -------------------------- Bloc physique --------------------------
        if (!simPause) {    
            for (std::size_t i = 0; i < pasImage; ++i) {
                pasLeapfrog(univers, dt);
            }

            // Calcul de l'erreur relative d'énergie du système par rapport au départ
            eTotale = energieTotale(univers); // valeur d'énergie totale du système
            erreurEnergie = (energieDepart - eTotale) / energieDepart;

            energieReel = eTotale * unites.energie_J();
            tempsSim    = unites.versSecondes(univers.temps) / constantes::secondesParJour;
        }

        // -------------------------- Bloc affichage --------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        Vector2 largeurPAUSED = MeasureTextEx(googleSans, "PAUSED", fontSize, 2);
        if (simPause) DrawTextEx(googleSans, "PAUSED", {(largeur / 2) - (largeurPAUSED.x / 2), 10}, fontSize, 2, WHITE);

        // Dessin d'un cercle coloré pour chaque corps de la liste
        for (std::size_t iCorps = 0; iCorps < univers.taille(); ++iCorps) {
            Vector2 positionEcran = camera.versEcran(univers.positions[iCorps]);

            Vector4 couleurBase = {univers.couleurs[iCorps].r, univers.couleurs[iCorps].g, univers.couleurs[iCorps].b, 1};
            Color couleur = ColorFromNormalized(couleurBase);
            double rayonAffiche = std::max(univers.rayons[iCorps] * camera.zoom, 1.0);

            if (rayonTest) DrawCircleV(positionEcran, 5, couleur);
            else DrawCircleV(positionEcran, rayonAffiche, couleur);
        }

        auto tempsActuel = std::chrono::steady_clock::now();
        auto tempsTotal  = std::chrono::duration<float, std::milli>(tempsActuel - debutSim);

        // Affichages d'infos (temps de sim., énergie totale, etc...)
        DrawTextEx(googleSans, TextFormat("temps = %.2f jours", tempsSim),    {10, 50},                fontSize, 2, WHITE);
        DrawTextEx(googleSans, TextFormat("e_totale = %.2e J", energieReel),  {10, 50 + fontSize},     fontSize, 2, WHITE);
        DrawTextEx(googleSans, TextFormat("Erreur = %.2e", erreurEnergie),    {10, 50 + 2 * fontSize}, fontSize, 2, WHITE);

        Vector2 largeurTempsTotal = MeasureTextEx(googleSans, TextFormat("%.2f s", tempsTotal.count() / 1000), fontSize, 2);
        DrawTextEx(googleSans, TextFormat("%.2f s", tempsTotal.count() / 1000), {largeur - (largeurTempsTotal.x + 10), 10}, fontSize, 2, WHITE);

        DrawFPS(10, 10);

        // Calcul et affichage du frametime (temps pour créer une image)
        auto frameEnd  = std::chrono::steady_clock::now();
        auto frametime = std::chrono::duration<float, std::milli>(frameEnd - frameStart);

        DrawTextEx(googleSans, TextFormat("Frametime = %.2f ms", frametime.count()), {10, 50 + 3 * fontSize}, fontSize, 2, WHITE);

        EndDrawing();
    }

    UnloadFont(googleSans);
    CloseWindow();
}