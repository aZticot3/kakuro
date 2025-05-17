#include "../include/blackCell.h"

// Constructeur
BlackCell::BlackCell(int row, int col) : Cell(row, col) {
    // Pas de données supplémentaires à initialiser
}

// Destructeur
BlackCell::~BlackCell() {
    // Rien de spécial à nettoyer
}

// Implémentation de la méthode abstraite display()
std::string BlackCell::display() const {
    return "#"; // Représentation d'une case noire
}