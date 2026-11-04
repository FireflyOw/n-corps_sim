#pragma once

#include <vector>
#include <cstddef>
#include <cassert>
#include <algorithm>

template <typename Type>
class TamponCirculaire {
private:
    std::size_t taille_ = 0, head_ = 0, capacite_; // variables privées pour qu'aucun code n'y touche
    std::vector<Type> buffer;

public:
    // Fonctions pour vérifier l'état du tampon
    bool vide() const { return taille_ == 0; }
    bool plein() const { return taille_ == capacite_; }

    // Fonction pour récupérer les infos du tampon
    std::size_t taille() const { return taille_; }
    std::size_t capacite() const { return capacite_; }

    // Constructeur qui crée le tampon d'une capacité donnée
    TamponCirculaire(std::size_t capaciteMax) : capacite_(std::max<std::size_t>(capaciteMax, 1)) {
        buffer.resize(capacite_);
    }

    // Fonction pour écrire dans le tampon
    void ajouter(const Type& element) {
        buffer[(head_ + taille_) % capacite_] = element;

        if (taille_ < capacite_) taille_++;
        else head_ = (head_ + 1) % capacite_;
    }

    // Fonction pour vider le tampon 
    void vider() {
        head_ = 0;
        taille_ = 0;
    }

    const Type& operator[](std::size_t i) const {
        assert(i < taille_);

        return buffer[(head_ + i) % capacite_];
    }
};