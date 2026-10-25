// Tests de la structure Unites (include/unites.hpp)
//
// On vérifie les conversions avec des valeurs astronomiques CONNUES,
// trouvées indépendamment de notre code (tables, ordres de grandeur réels).
// Si un test échoue, soit la formule est fausse, soit une constante est mal recopiée.

#include "outils_test.hpp"
#include "unites.hpp"

#include <cmath>

// Comparaison RELATIVE : |a - b| / |b| < tolerance.
// Indispensable ici : nos valeurs vont de 1e-6 à 1e39, une tolérance
// absolue (comme egal() avec 1e-12) n'aurait aucun sens à ces échelles.
// Exemple : tolerance = 1e-4 veut dire "identiques à 0,01 % près".
bool procheRelatif(double valeurObtenue, double valeurAttendue, double tolerance) {
    return std::abs(valeurObtenue - valeurAttendue) / std::abs(valeurAttendue) < tolerance;
}

int main() {
    int echecs = 0;

    // Constantes de référence (valeurs officielles, en SI)
    const double uniteAstronomique_m = 1.495978707e11;       // 1 UA
    const double masseSoleil_kg      = 1.98847e30;           // 1 M☉
    const double kiloparsec_m        = 3.0856775814913673e19; // 1 kpc
    const double secondesParJour     = 86400.0;
    const double PI                  = std::acos(-1.0);

    // "const" ici n'est pas décoratif : il vérifie à la COMPILATION que toutes
    // les méthodes utilisées plus bas sont bien marquées const.
    // (Enlève un const dans unites.hpp : ce fichier ne compilera plus.)
    const Unites systemeSolaire{uniteAstronomique_m, masseSoleil_kg};

    // --- Test 1 : temps de référence ≈ 58,13 jours ---------------------------
    // T = √(L³ / (G·M)) avec L = 1 UA et M = 1 M☉
    // Tolérance serrée (1e-6) : elle détecte une valeur de G tronquée
    // (6.674e-11 au lieu de 6.67430e-11 décale déjà le 5e chiffre).
    {
        double tempsReference_jours = systemeSolaire.temps_s() / secondesParJour;
        verifier(procheRelatif(tempsReference_jours, 58.131562, 1e-6),
                 "temps de reference du systeme solaire = 58,13 jours", echecs);
    }

    // --- Test 2 : une orbite de 1 UA dure une année --------------------------
    // En unités internes (G = 1, M = 1, r = 1), la période d'une orbite
    // circulaire vaut 2π. Convertie en jours, on doit retrouver l'année
    // sidérale réelle : 365,256 jours (à ~0,01 % près, car on néglige
    // la masse de la Terre).
    {
        double periodeOrbite_jours = systemeSolaire.versSecondes(2.0 * PI) / secondesParJour;
        verifier(procheRelatif(periodeOrbite_jours, 365.256, 1e-4),
                 "orbite de 1 UA = 1 an", echecs);
    }

    // --- Test 3 : vitesse orbitale de la Terre ≈ 29,78 km/s ------------------
    // En unités internes, la vitesse d'une orbite circulaire de rayon 1 vaut 1.
    // Convertie en m/s, elle doit donner la vitesse moyenne réelle de la Terre.
    {
        double vitesseTerre_ms = 1.0 * systemeSolaire.vitesse_ms();
        verifier(procheRelatif(vitesseTerre_ms, 29780.0, 1e-3),
                 "vitesse orbitale terrestre = 29,78 km/s", echecs);
    }

    // --- Test 4 : énergie de référence = G·M²/L ------------------------------
    // Deux formules différentes doivent donner la même énergie :
    // M·L²/T² (ta méthode) et G·M²/L (obtenue en remplaçant T par sa formule).
    {
        double energieAttendue_J = Unites::G_SI * masseSoleil_kg * masseSoleil_kg / uniteAstronomique_m;
        verifier(procheRelatif(systemeSolaire.energie_J(), energieAttendue_J, 1e-12),
                 "energie de reference = G.M^2/L", echecs);
    }

    // --- Test 5 : conversions aller-retour -----------------------------------
    // Convertir vers le SI puis revenir doit redonner la valeur de départ
    // (à l'arrondi machine près). On teste des valeurs très différentes.
    {
        const double valeursInternes[] = {1.0, 3.0e-6, 2.5, 1234.5};
        for (double valeurInterne : valeursInternes) {
            double longueurRetour = systemeSolaire.versLongueur(systemeSolaire.versMetres(valeurInterne));
            double masseRetour    = systemeSolaire.versMasse(systemeSolaire.versKilogrammes(valeurInterne));
            double tempsRetour    = systemeSolaire.versTemps(systemeSolaire.versSecondes(valeurInterne));

            verifier(procheRelatif(longueurRetour, valeurInterne, 1e-14), "aller-retour longueur", echecs);
            verifier(procheRelatif(masseRetour,    valeurInterne, 1e-14), "aller-retour masse",    echecs);
            verifier(procheRelatif(tempsRetour,    valeurInterne, 1e-14), "aller-retour temps",    echecs);
        }
    }

    // --- Test 6 : la masse de la Terre en unités internes --------------------
    // 5,972e24 kg / 1,988e30 kg ≈ 3,0e-6 : la Terre pèse 3 millionièmes de Soleil.
    {
        double masseTerreInterne = systemeSolaire.versMasse(5.972e24);
        verifier(procheRelatif(masseTerreInterne, 3.003e-6, 1e-3),
                 "masse de la Terre = 3e-6 masse solaire", echecs);
    }

    // --- Test 7 : changement d'échelle : une galaxie -------------------------
    // Même structure, autres références : 1 kpc et 10^10 M☉.
    // La vitesse de référence vaut alors ≈ 207 km/s : c'est l'ordre de
    // grandeur de la vitesse de rotation des étoiles dans une galaxie spirale
    // (le Soleil tourne à ~220 km/s autour du centre de la Voie lactée).
    // Valeur de comparaison calculée avec G = 4,30091e-6 kpc·(km/s)²/M☉,
    // une forme de G utilisée en astrophysique, indépendante de notre code.
    {
        const Unites galaxie{kiloparsec_m, 1.0e10 * masseSoleil_kg};
        double vitesseAttendue_ms = std::sqrt(4.30091e-6 * 1.0e10) * 1000.0;
        verifier(procheRelatif(galaxie.vitesse_ms(), vitesseAttendue_ms, 1e-4),
                 "vitesse de reference galactique = 207 km/s", echecs);
    }

    if (echecs == 0) {
        std::cout << "Unites : tous les tests sont passes\n";
    }
    return echecs == 0 ? 0 : 1;
}