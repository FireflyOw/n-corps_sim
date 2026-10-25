cmake_minimum_required(VERSION 3.16)
project(physique_n_corps LANGUAGES CXX)

# --- Langage -----------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fichier utilisé par les éditeurs (clangd, VS Code...) pour l'autocomplétion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Mode de compilation par défaut -------------------------------------
# Sans choix explicite, on compile en Release (optimisations -O3).
# En Debug, rien n'est optimisé : la simulation serait très lente.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Type de build" FORCE)
endif()

# --- Affichage : raylib ---------------------------------------------------
# SIM_AFFICHAGE=OFF permet de ne compiler que la simulation et les tests
# (par exemple sur une machine sans écran ni pilotes graphiques) :
#     cmake -B build -DSIM_AFFICHAGE=OFF
# Ce bloc est placé AVANT add_compile_options pour que nos avertissements
# (-Wall -Wextra...) ne s'appliquent pas au code de raylib lui-même.
option(SIM_AFFICHAGE "Compiler les programmes d'affichage (telecharge raylib)" ON)

if(SIM_AFFICHAGE)
    include(FetchContent)

    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

    # Sous Linux, raylib utilise X11 par défaut (donc XWayland sur une session
    # Wayland). Si les paquets de développement Wayland sont présents, on active
    # aussi le support natif : GLFW choisit ensuite le bon à l'exécution.
    if(UNIX AND NOT APPLE)
        find_package(PkgConfig QUIET)
        if(PkgConfig_FOUND)
            pkg_check_modules(WAYLAND_DEPS QUIET wayland-client xkbcommon wayland-protocols)
        endif()
        if(WAYLAND_DEPS_FOUND)
            message(STATUS "Wayland detecte : raylib sera compile avec le support Wayland")
            set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE)
        endif()
    endif()

    # CMake télécharge et compile raylib lors de la première configuration
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        6.2
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(raylib)
endif()

# --- Avertissements du compilateur --------------------------------------
# À placer AVANT la création des cibles : add_compile_options ne s'applique
# qu'aux cibles créées après lui.
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# --- Bibliothèque de simulation -----------------------------------------
# STATIC : les .cpp de src/ sont compilés une seule fois en une bibliothèque
# (libsim.a), ensuite intégrée à chaque exécutable qui la lie.
# PUBLIC : le dossier include/ sert à compiler la bibliothèque elle-même
# ET à tous ceux qui la lient (tests, futur programme).
add_library(sim STATIC
    src/physique.cpp
)
target_include_directories(sim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(sim PUBLIC cxx_std_17)

# --- Tests (lancement : ctest --test-dir build) ---------------------------------------------------------------
enable_testing()

# Petite fonction CMake pour éviter de répéter les mêmes 4 lignes par test.
# Utilisation : ajouter_test(<nom du test> <fichier .cpp>)
function(ajouter_test nom fichier)
    add_executable(${nom} ${fichier})
    target_include_directories(${nom} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    target_link_libraries(${nom} PRIVATE sim)
    add_test(NAME ${nom} COMMAND ${nom})
endfunction()

ajouter_test(vec3D_test        tests/vec3D_test.cpp)
ajouter_test(unites_test       tests/unites_test.cpp)
ajouter_test(univers_test      tests/univers_test.cpp)
ajouter_test(physique_test     tests/physique_test.cpp)
ajouter_test(integrateurs_test tests/integrateurs_test.cpp)

# --- Programmes d'exemple ------------------------------------------------
add_executable(comp_integrateurs exemples/comp_integrateurs.cpp)
target_link_libraries(comp_integrateurs PRIVATE sim)

# --- Affichage -----------------------------------------------------------
if(SIM_AFFICHAGE)
    # Petit programme qui ouvre une fenêtre : sert à vérifier que raylib fonctionne
    add_executable(fenetre_Raylib exemples/fenetre_Raylib.cpp)
    target_link_libraries(fenetre_Raylib PRIVATE raylib)

    # Programme principal avec physique et affichage
    add_executable(nbody src/main.cpp)
    target_link_libraries(nbody PRIVATE sim raylib)
endif()