// Tests du tampon circulaire (TamponCirculaire dans utility/ringBuffer.hpp)
//
// Convention à vérifier partout : tampon[0] = valeur la PLUS ANCIENNE encore
// stockée, tampon[taille() - 1] = la plus récente.
//
// Les cas qui comptent vraiment sont ceux où l'indice de tête (head_) n'est
// plus à 0 : c'est là que les erreurs de modulo se cachent.

#include "outils_test.hpp"
#include "utility/ringBuffer.hpp"

#include <vector>

// Vérifie que le contenu du tampon est exactement celui de la liste attendue,
// de la plus ancienne valeur à la plus récente.
bool contenuEgal(const TamponCirculaire<int>& tampon, const std::vector<int>& attendu) {
    if (tampon.taille() != attendu.size()) return false;
    for (std::size_t i = 0; i < attendu.size(); ++i) {
        if (tampon[i] != attendu[i]) return false;
    }
    return true;
}

int main() {
    int echecs = 0;

    // --- Test 1 : tampon neuf ------------------------------------------------
    {
        TamponCirculaire<int> tampon(4);
        verifier(tampon.vide(),            "neuf : vide", echecs);
        verifier(!tampon.plein(),          "neuf : pas plein", echecs);
        verifier(tampon.taille() == 0,     "neuf : taille 0", echecs);
        verifier(tampon.capacite() == 4,   "neuf : capacite 4", echecs);
    }

    // --- Test 2 : remplissage partiel ----------------------------------------
    {
        TamponCirculaire<int> tampon(4);
        tampon.ajouter(1);
        tampon.ajouter(2);
        tampon.ajouter(3);
        verifier(contenuEgal(tampon, {1, 2, 3}), "partiel : contenu 1 2 3", echecs);
        verifier(!tampon.vide(),                 "partiel : pas vide", echecs);
        verifier(!tampon.plein(),                "partiel : pas plein", echecs);
    }

    // --- Test 3 : exactement plein, sans débordement -------------------------
    {
        TamponCirculaire<int> tampon(4);
        for (int valeur = 1; valeur <= 4; ++valeur) tampon.ajouter(valeur);
        verifier(contenuEgal(tampon, {1, 2, 3, 4}), "plein : contenu 1 2 3 4", echecs);
        verifier(tampon.plein(),                    "plein : plein()", echecs);
        verifier(tampon.taille() == 4,              "plein : taille 4", echecs);
    }

    // --- Test 4 : un débordement (cas détaillé dans nos échanges) ------------
    // 1..6 dans une capacité 4 : le vector interne contient [5, 6, 3, 4]
    // avec head_ = 2, mais vu de l'extérieur on doit lire 3 4 5 6.
    {
        TamponCirculaire<int> tampon(4);
        for (int valeur = 1; valeur <= 6; ++valeur) tampon.ajouter(valeur);
        verifier(contenuEgal(tampon, {3, 4, 5, 6}), "debordement : contenu 3 4 5 6", echecs);
        verifier(tampon.plein(),                    "debordement : toujours plein", echecs);
        verifier(tampon.taille() == 4,              "debordement : taille reste 4", echecs);
    }

    // --- Test 5 : plusieurs tours complets -----------------------------------
    {
        TamponCirculaire<int> tampon(4);
        for (int valeur = 1; valeur <= 11; ++valeur) tampon.ajouter(valeur);
        verifier(contenuEgal(tampon, {8, 9, 10, 11}), "plusieurs tours : contenu 8 9 10 11", echecs);
    }

    // --- Test 6 : vider() puis réutilisation ---------------------------------
    // On vide quand head_ n'est PAS à 0, puis on remplit jusqu'à re-déborder.
    {
        TamponCirculaire<int> tampon(4);
        for (int valeur = 1; valeur <= 6; ++valeur) tampon.ajouter(valeur);
        tampon.vider();
        verifier(tampon.vide(),        "vider : vide() vrai", echecs);
        verifier(tampon.taille() == 0, "vider : taille 0", echecs);
        verifier(!tampon.plein(),      "vider : plus plein", echecs);
        verifier(tampon.capacite() == 4, "vider : capacite inchangee", echecs);

        tampon.ajouter(100);
        tampon.ajouter(101);
        verifier(contenuEgal(tampon, {100, 101}), "apres vider : contenu 100 101", echecs);

        for (int valeur = 102; valeur <= 106; ++valeur) tampon.ajouter(valeur);
        verifier(contenuEgal(tampon, {103, 104, 105, 106}), "apres vider : re-debordement", echecs);
    }

    // --- Test 7 : capacité 1 -------------------------------------------------
    {
        TamponCirculaire<int> tampon(1);
        tampon.ajouter(7);
        verifier(contenuEgal(tampon, {7}), "capacite 1 : contenu 7", echecs);
        tampon.ajouter(8);
        verifier(contenuEgal(tampon, {8}), "capacite 1 : 8 remplace 7", echecs);
        verifier(tampon.plein(),           "capacite 1 : plein", echecs);
    }

    // --- Test 8 : capacité 0 ramenée à 1 -------------------------------------
    // Sans cette protection, ajouter() ferait un modulo par zéro.
    {
        TamponCirculaire<int> tampon(0);
        verifier(tampon.capacite() == 1, "capacite 0 : ramenee a 1", echecs);
        tampon.ajouter(5);
        verifier(contenuEgal(tampon, {5}), "capacite 0 : utilisable", echecs);
    }

    // --- Test 9 : comparaison avec un modèle naïf ----------------------------
    // Pour plusieurs capacités et plusieurs nombres d'ajouts, on compare à
    // chaque étape avec "les capacite dernières valeurs d'un std::vector".
    // Ça couvre des positions de head_ qu'on n'aurait pas pensé à écrire à la main.
    {
        bool toutBon = true;
        for (std::size_t capacite = 1; capacite <= 7; ++capacite) {
            TamponCirculaire<int> tampon(capacite);
            std::vector<int> historique;
            for (int valeur = 1; valeur <= 40; ++valeur) {
                tampon.ajouter(valeur);
                historique.push_back(valeur);

                std::size_t debut = historique.size() > capacite ? historique.size() - capacite : 0;
                std::vector<int> attendu(historique.begin() + debut, historique.end());
                if (!contenuEgal(tampon, attendu)) toutBon = false;
            }
        }
        verifier(toutBon, "modele naif : capacites 1 a 7, 40 ajouts", echecs);
    }

    // --- Test 10 : le tampon fonctionne avec double3 (notre vrai usage) -----
    {
        TamponCirculaire<double3> tampon(3);
        for (int k = 1; k <= 5; ++k) tampon.ajouter(double3(k, 10 * k, 0));
        verifier(tampon.taille() == 3,                     "double3 : taille 3", echecs);
        verifier(egal(tampon[0], double3(3, 30, 0)),       "double3 : plus ancien = 3", echecs);
        verifier(egal(tampon[2], double3(5, 50, 0)),       "double3 : plus recent = 5", echecs);
    }

    if (echecs == 0) std::cout << "Tampon circulaire : tous les tests sont passes\n";
    return echecs == 0 ? 0 : 1;
}