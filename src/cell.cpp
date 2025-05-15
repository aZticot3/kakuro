#include "../include/cell.h"

// Constructeur
Cell::Cell(int row, int col) : row(row), col(col) {}

// Destructeur virtuel (important pour une classe abstraite)
Cell::~Cell() {}

// Implémentation des méthodes getRow et getCol
int Cell::getRow() const {
    return row;
}

int Cell::getCol() const {
    return col;
}

std::string Cell::displayColored(bool isCorrect) const {
    // Par défaut, on utilise la méthode standard display()
    // et on y ajoute la couleur appropriée
    if (isCorrect) {
        return COLOR_GREEN + display() + COLOR_RESET;
    } else {
        return COLOR_RED + display() + COLOR_RESET;
    }
}