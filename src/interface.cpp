#include "interface.hpp"

#include <chrono>
#include <cstdio>

bool dessinerInterface(Simulation& sim, CameraSim& camera, Unites& unites, float tempsTotal) {
    auto frameStart = std::chrono::steady_clock::now();
    bool quitter = false;
    
    rlImGuiBegin();
    //ImGui::ShowDemoWindow();

    ImGui::Begin("Univers");
    ImGui::Text("Temps : %.1f jours", sim.calculTempsSim());
    ImGui::Text("Vitesse : %.0f h/frame", ((unites.versSecondes(sim.dt) / 3600) * sim.pasImage));
    ImGui::Text("Zoom : %.0f", camera.zoom);
    ImGui::Checkbox("Pause", &sim.simPause);

    if (ImGui::TreeNode("Énergie Système")) {
        ImGui::Text("totale : %.2e J", sim.calculEnergieReel());
        ImGui::Text("erreur : %.2e %%", sim.calculErreurEnergie());
        ImGui::TreePop();
    }

    auto frameEnd  = std::chrono::steady_clock::now();
    float frametime = std::chrono::duration<float, std::milli>(frameEnd - frameStart).count();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Simulation")) {
            if (ImGui::MenuItem("Réinitialiser", "R")) sim.reinitialiser();
            ImGui::Separator();
            if (ImGui::MenuItem("Quitter")) quitter = true;
            ImGui::EndMenu();
        }

        char infos[128];
        std::snprintf(infos, sizeof(infos), "FPS : %d (%.2f ms) | %.2f s ", 
                                            GetFPS(), sim.frametime + frametime, tempsTotal / 1000);

        float largeurInfos = ImGui::CalcTextSize(infos).x;
        ImGui::SameLine(ImGui::GetWindowWidth() - largeurInfos - ImGui::GetStyle().ItemSpacing.x);
        ImGui::TextUnformatted(infos);

        ImGui::EndMainMenuBar();
    }

    ImGui::End();
    rlImGuiEnd();

    return quitter;
}