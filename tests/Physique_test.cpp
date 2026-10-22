#include <iostream>
#include <cmath>

#include "Outils_test.hpp"
#include "Physique.hpp"

// Vrai si aucune composante n'est infinie ou NaN (résultat d'une division par zéro)
bool estFini(const Vector3Double& vecteur) {
    return std::isfinite(vecteur.x) && std::isfinite(vecteur.y) && std::isfinite(vecteur.z);
}

int main() {
    int echecs = 0;

    // ---------------------------------------------------------------------
    // Test 1 : deux corps identiques à distance 1 (G = 1, sans adoucissement)
    // Chacun doit avoir une accélération de 1, dirigée vers l'autre.
    // ---------------------------------------------------------------------
    {
        Univers univers;
        univers.ajouter(Corps(1, Vector3Double(0, 0, 0), Vector3Double()));
        univers.ajouter(Corps(1, Vector3Double(1, 0, 0), Vector3Double()));

        calculAccelerations(univers);

        verifier(egal(univers.accelerations[0], Vector3Double( 1, 0, 0)), "deux corps : corps 0 attire vers +x", echecs);
        verifier(egal(univers.accelerations[1], Vector3Double(-1, 0, 0)), "deux corps : corps 1 attire vers -x", echecs);
    }

    // ---------------------------------------------------------------------
    // Test 2 : loi en 1/r² — à distance 2, l'accélération doit valoir 1/4
    // ---------------------------------------------------------------------
    {
        Univers univers;
        univers.ajouter(Corps(1, Vector3Double(0, 0, 0), Vector3Double()));
        univers.ajouter(Corps(1, Vector3Double(0, 2, 0), Vector3Double()));

        calculAccelerations(univers);

        verifier(egal(univers.accelerations[0].norme(), 0.25), "loi en 1/r2 : distance 2 donne 1/4", echecs);
    }

    // ---------------------------------------------------------------------
    // Test 3 : masses différentes (1 et 3)
    // Chaque corps est accéléré par la masse de L'AUTRE : le corps léger
    // doit avoir une accélération 3 fois plus grande que le corps lourd.
    // ---------------------------------------------------------------------
    {
        Univers univers;
        univers.ajouter(Corps(1, Vector3Double(0, 0, 0), Vector3Double()));   // léger
        univers.ajouter(Corps(3, Vector3Double(1, 0, 0), Vector3Double()));   // lourd

        calculAccelerations(univers);

        const double accelerationLeger = univers.accelerations[0].norme();
        const double accelerationLourd = univers.accelerations[1].norme();

        verifier(egal(accelerationLeger, 3.0), "masses 1 et 3 : acceleration du corps leger = 3", echecs);
        verifier(egal(accelerationLourd, 1.0), "masses 1 et 3 : acceleration du corps lourd = 1", echecs);
    }

    // ---------------------------------------------------------------------
    // Test 4 : conservation de la quantité de mouvement (3e loi de Newton)
    // Avec des corps placés n'importe où, la somme des m × a doit être nulle.
    // C'est ce test qui valide la boucle décalée (le += / -= sur chaque paire).
    // ---------------------------------------------------------------------
    {
        Univers univers;
        univers.ajouter(Corps(1.0, Vector3Double( 0.3, -1.2,  0.5), Vector3Double()));
        univers.ajouter(Corps(2.5, Vector3Double(-0.7,  0.4,  1.1), Vector3Double()));
        univers.ajouter(Corps(0.8, Vector3Double( 1.5,  0.9, -0.6), Vector3Double()));
        univers.ajouter(Corps(4.0, Vector3Double(-1.0, -0.5, -1.3), Vector3Double()));

        calculAccelerations(univers);

        Vector3Double sommeMasseFoisAcceleration;
        for (std::size_t indiceCorps = 0; indiceCorps < univers.taille(); ++indiceCorps) {
            sommeMasseFoisAcceleration += univers.accelerations[indiceCorps] * univers.masses[indiceCorps];
        }

        verifier(egal(sommeMasseFoisAcceleration, Vector3Double()), "quantite de mouvement : somme des m*a nulle", echecs);
    }

    // ---------------------------------------------------------------------
    // Test 5 : appeler la fonction deux fois doit donner le même résultat
    // (vérifie que les accélérations sont bien remises à zéro au début)
    // ---------------------------------------------------------------------
    {
        Univers univers;
        univers.ajouter(Corps(1, Vector3Double(0, 0, 0), Vector3Double()));
        univers.ajouter(Corps(1, Vector3Double(1, 0, 0), Vector3Double()));

        calculAccelerations(univers);
        const Vector3Double premierResultat = univers.accelerations[0];

        calculAccelerations(univers);
        const Vector3Double secondResultat = univers.accelerations[0];

        verifier(egal(premierResultat, secondResultat), "deux appels successifs : meme resultat", echecs);
    }

    // ---------------------------------------------------------------------
    // Test 6 : adoucissement (epsilon)
    // a) Deux corps superposés : sans epsilon on diviserait par zéro.
    //    Avec epsilon, l'accélération doit rester finie (et nulle ici).
    // b) À distance 1 avec epsilon = 1 : a = 1 / (1² + 1²)^(3/2) = 1 / 2^(3/2)
    // ---------------------------------------------------------------------
    {
        Univers univers;
        univers.epsilon = 0.1;
        univers.ajouter(Corps(1, Vector3Double(0, 0, 0), Vector3Double()));
        univers.ajouter(Corps(1, Vector3Double(0, 0, 0), Vector3Double()));

        calculAccelerations(univers);

        verifier(estFini(univers.accelerations[0]), "adoucissement : corps superposes, pas de division par zero", echecs);
        verifier(egal(univers.accelerations[0], Vector3Double()), "adoucissement : corps superposes, acceleration nulle", echecs);
    }
    {
        Univers univers;
        univers.epsilon = 1.0;
        univers.ajouter(Corps(1, Vector3Double(0, 0, 0), Vector3Double()));
        univers.ajouter(Corps(1, Vector3Double(1, 0, 0), Vector3Double()));

        calculAccelerations(univers);

        const double valeurAttendue = 1.0 / std::pow(2.0, 1.5);
        verifier(egal(univers.accelerations[0].norme(), valeurAttendue), "adoucissement : epsilon = 1 a distance 1", echecs);
    }

    // ---------------------------------------------------------------------
    // Test 7 : un corps seul ne subit aucune accélération
    // ---------------------------------------------------------------------
    {
        Univers univers;
        univers.ajouter(Corps(5, Vector3Double(1, 2, 3), Vector3Double()));

        calculAccelerations(univers);

        verifier(egal(univers.accelerations[0], Vector3Double()), "corps seul : acceleration nulle", echecs);
    }

    if (echecs == 0) std::cout << "Tous les tests passent\n";
    return echecs;
}