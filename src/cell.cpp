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

