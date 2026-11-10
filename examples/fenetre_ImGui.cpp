// Premier test de l'interface : une fenetre raylib avec un menu Dear ImGui.
//
// Lancement : ./build/fenetre_ImGui
//
// Principe d'ImGui ("immediate mode") : il n'y a pas d'objets bouton ou curseur
// a creer et a stocker. A CHAQUE IMAGE, on decrit l'interface avec des appels
// (ImGui::Begin, ImGui::SliderInt...), et ImGui la dessine. Un widget renvoie
// true quand l'utilisateur vient de le modifier, et il ecrit directement dans
// la variable qu'on lui passe par pointeur.

#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"

int main() {
    const int largeur = 800;
    const int hauteur = 600;

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(largeur, hauteur, "Test ImGui");
    SetTargetFPS(60);

    rlImGuiSetup(true);   // true = theme sombre

    int  pasParImage = 10;
    bool pause       = false;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Le dessin de la simulation (raylib) viendrait ici, AVANT l'interface

        rlImGuiBegin();   // debut de l'image ImGui

        ImGui::Begin("Simulation");
        ImGui::Text("FPS : %d", GetFPS());
        ImGui::Checkbox("Pause", &pause);
        ImGui::SliderInt("Pas par image", &pasParImage, 1, 100);
        ImGui::End();

        rlImGuiEnd();     // fin de l'image ImGui : c'est ici que tout est dessine

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
}