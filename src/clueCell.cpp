#include "../include/clueCell.h"
#include <sstream>

// Constructeur
ClueCell::ClueCell(int row, int col, int rightSum, int downSum) 
    : Cell(row, col), rightSum(rightSum), downSum(downSum) {
    // Initialisation des sommes dans la liste d'initialisation
    // Les vecteurs rightCells et downCells sont initialisés vides
}

// Destructeur
ClueCell::~ClueCell() {
    // C'est juste une référence
}

// Obtenir la somme pour les cellules à droite
int ClueCell::getRightSum() const {
    return rightSum;
}

// Obtenir la somme pour les cellules en dessous
int ClueCell::getDownSum() const {
    return downSum;
}


// Implémentation de la méthode abstraite display()
std::string ClueCell::display() const {
    std::ostringstream oss;
    
    // Format d'affichage: downSum\rightSum
    // Si l'une des sommes est 0, elle n'est pas affichée
    if (downSum > 0) {
        oss << downSum;
    }
    
    oss << "\\";
    
    if (rightSum > 0) {
        oss << rightSum;
    }
    
    return oss.str();
}