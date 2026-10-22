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
 
# --- Bibliothèque de simulation -----------------------------------------
# "INTERFACE" = bibliothèque 100 % headers (rien à compiler pour l'instant).
# Tout exécutable qui fait target_link_libraries(... sim) récupère
# automatiquement le dossier src/ et le standard C++17.
add_library(sim INTERFACE)
target_include_directories(sim INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_features(sim INTERFACE cxx_std_17)
 
# Avertissements du compilateur : ils attrapent beaucoup d'erreurs
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# --- Tests ---------------------------------------------------------------
enable_testing()

target_include_directories(Vec3D_test PRIVATE tests)
add_executable(Vec3D_test tests/Vec3D_test.cpp)
target_link_libraries(Vec3D_test PRIVATE sim)
add_test(NAME vec3 COMMAND Vec3D_test)

arget_include_directories(Univers_test PRIVATE tests)
add_executable(Univers_test tests/Univers_test.cpp)
target_link_libraries(Univers_test PRIVATE sim)
add_test(NAME vec3 COMMAND Univers_test)
 
# --- Plus tard : l'exécutable de la simulation ---------------------------
# add_executable(nbody src/main.cpp)
# target_link_libraries(nbody PRIVATE sim)
