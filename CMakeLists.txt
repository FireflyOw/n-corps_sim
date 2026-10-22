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
    src/Physique.cpp
)
target_include_directories(sim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(sim PUBLIC cxx_std_17)

# --- Tests ---------------------------------------------------------------
enable_testing()

# Petite fonction CMake pour éviter de répéter les mêmes 4 lignes par test.
# Utilisation : ajouter_test(<nom du test> <fichier .cpp>)
function(ajouter_test nom fichier)
    add_executable(${nom} ${fichier})
    target_include_directories(${nom} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    target_link_libraries(${nom} PRIVATE sim)
    add_test(NAME ${nom} COMMAND ${nom})
endfunction()

ajouter_test(Vec3D_test    tests/Vec3D_test.cpp)
ajouter_test(Univers_test  tests/Univers_test.cpp)
ajouter_test(Physique_test tests/Physique_test.cpp)

# --- Plus tard : l'exécutable de la simulation ---------------------------
# add_executable(nbody src/main.cpp)
# target_link_libraries(nbody PRIVATE sim)