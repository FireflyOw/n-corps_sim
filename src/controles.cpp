#include "raylib.h"
#include "controles.hpp"

#include <algorithm>

void gestionControles(Simulation& sim, CameraSim& camera) {
    // Gestion de la vitesse de simulation avec les flèches
    if (IsKeyPressed(KEY_UP)) sim.pasImage *= 2;
    else if (IsKeyPressed(KEY_DOWN)) sim.pasImage /= 2;

    sim.pasImage = std::clamp<std::size_t>(sim.pasImage, 1, 100);

    // Gestion du zoom avec la molette
    camera.zoom *= std::pow(camera.facteurZoom, GetMouseWheelMove());
    camera.zoom = std::clamp<double>(camera.zoom, 10, 1e7);

    // Gestion de la pause avec Espace
    if (IsKeyPressed(KEY_SPACE)) sim.simPause = !sim.simPause;

    // Gestion du déplacement à la souris
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        camera.enMouvement = true;
        camera.deplacement(GetMouseDelta());
    }
    else camera.enMouvement = false;

    // Gestion du suivi actif d'un corps
    if (IsKeyPressed(KEY_F)) camera.suiviActif = !camera.suiviActif;

    if (camera.suiviActif) {
        if (IsKeyPressed(KEY_RIGHT)) {
            ++camera.iCorpsSuivi;
            if (camera.iCorpsSuivi > sim.univers.taille() - 1) camera.iCorpsSuivi = 0;
        }
        else if (IsKeyPressed(KEY_LEFT)) {
            if (camera.iCorpsSuivi == 0) camera.iCorpsSuivi = sim.univers.taille();
            --camera.iCorpsSuivi;
        }
        
        if (camera.enMouvement || IsKeyPressed(KEY_C)) camera.suiviActif = false;
    }

    // Réinitialisation de la vue
    if (IsKeyPressed(KEY_C)) {
        camera.positionVisee = double3();
        camera.zoom = 200;
    }

    // Réinitialisation de la simulation avec R
    if (IsKeyPressed(KEY_R)) {
        sim.reinitialiser();
    }
}