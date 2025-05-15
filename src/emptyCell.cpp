#include "../include/emptyCell.h"
#include <string>
#include <algorithm>

// Constructeur
EmptyCell::EmptyCell(int row, int col) 
    : Cell(row, col), value(0) {
    // Initialisation des valeurs possibles (1-9)
    for (int i = 1; i <= 9; ++i) {
        possibleValues.insert(i);
    }
}

// Destructeur
EmptyCell::~EmptyCell() {
    // Rien de spécial à nettoyer
}

// Définir la valeur de la cellule
void EmptyCell::setValue(int val) {
    value = val;
    // Si une valeur est définie, on vide l'ensemble des valeurs possibles
    if (val > 0) {
        possibleValues.clear();
    }
}

// Obtenir la valeur actuelle de la cellule
int EmptyCell::getValue() const {
    return value;
}

// Obtenir l'ensemble des valeurs possibles
const std::set<int>& EmptyCell::getPossibleValues() const {
    return possibleValues;
}

// Supprimer une valeur de l'ensemble des valeurs possibles
void EmptyCell::removePossibleValue(int val) {
    possibleValues.erase(val);
}

// Remettre toutes les valeurs possibles (1-9)
void EmptyCell::clearPossibleValues() {
    possibleValues.clear();
}

// Implémentation de la méthode abstraite display()
std::string EmptyCell::display() const {
    if (value > 0) {
        // Si la cellule a une valeur, on l'affiche
        return std::to_string(value);
    } else {
        // Sinon on affiche un caractère indiquant une cellule vide
        return "_";
    }
}