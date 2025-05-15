#include "../include/grid.h"
#include <iostream>
#include <iomanip>

// Constructeur corrigé
Grid::Grid(int height, int width) : height(height), width(width) {
    // Initialiser le vecteur avec des pointeurs null
    cells.resize(height, std::vector<Cell*>(width, nullptr));
}

// Destructeur corrigé
Grid::~Grid() {
    // Libérer la mémoire pour toutes les cellules
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            delete cells[i][j];  // Supprime chaque pointeur Cell*
        }
    }
    // Pas besoin de libérer cells lui-même car c'est un std::vector
}

// Le reste du code reste inchangé
int Grid::getHeight() const {
    return height;
}

int Grid::getWidth() const {
    return width;
}

Cell* Grid::getCell(int row, int col) const {
    // Vérifier que les indices sont valides
    if (row >= 0 && row < height && col >= 0 && col < width) {
        return cells[row][col];
    }
    return nullptr;
}

void Grid::setCell(int row, int col, Cell* cell) {
    // Vérifier que les indices sont valides
    if (row >= 0 && row < height && col >= 0 && col < width) {
        // Libérer l'ancienne cellule si elle existe
        if (cells[row][col] != nullptr) {
            delete cells[row][col];
        }
        cells[row][col] = cell;
    }
}

void Grid::display() const {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (cells[i][j] != nullptr) {
                std::cout << std::setw(4) << cells[i][j]->display();
            } else {
                std::cout << std::setw(4) << "?"; // Case non définie
            }
        }
        std::cout << std::endl;
    }
}

