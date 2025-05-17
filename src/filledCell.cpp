#include "../include/filledCell.h"

// Constructeur
FilledCell::FilledCell(int row, int col, int value) : Cell(row, col), value(value) {
    // value est initialisé dans la liste d'initialisation
}

// Destructeur
FilledCell::~FilledCell() {
    // Rien de spécial à nettoyer
}

// Méthode pour obtenir la valeur de la cellule
int FilledCell::getValue() const {
    return value;
}

// Implémentation de la méthode abstraite display()
std::string FilledCell::display() const {
    // Convertit la valeur numérique en string
    return std::to_string(value);
}