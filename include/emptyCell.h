#ifndef EMPTY_CELL_H
#define EMPTY_CELL_H

#include "cell.h"
#include <string>
#include <set>

class EmptyCell : public Cell {
private:
    int value;  // Valeur actuelle de la cellule (0 si non définie)
    std::set<int> possibleValues;  // Ensemble des valeurs possibles (1-9)

public:
    EmptyCell(int row, int col);
    virtual ~EmptyCell();
    
    // Manipuler la valeur
    void setValue(int val);
    int getValue() const;
    
    // Manipuler les valeurs possibles
    const std::set<int>& getPossibleValues() const;
    void removePossibleValue(int val);
    void clearPossibleValues();
    
    // Implémentation de la méthode abstraite de la classe Cell
    std::string display() const override;

    std::string displayColored(bool isCorrect) const override;
};

#endif // EMPTY_CELL_H