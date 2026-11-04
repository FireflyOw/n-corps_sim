// Lancement : ./build/Raylib_test

#include "raylib.h"

int main() {
    const int largeur = 800;
    const int hauteur = 600;

    InitWindow(largeur, hauteur, "Test raylib");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawCircle(largeur / 2, hauteur / 2, 12, YELLOW);        // étoile
        DrawCircle(largeur / 2 + 200, hauteur / 2, 5, SKYBLUE);  // planète

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}