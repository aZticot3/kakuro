#ifndef FILLED_CELL_H
#define FILLED_CELL_H

#include "cell.h"
#include <string>

class FilledCell : public Cell {
private:
    int value; // Valeur numérique dans la cellule

public:
    FilledCell(int row, int col, int value);
    virtual ~FilledCell();
    
    // Obtenir la valeur de la cellule
    int getValue() const;
    
    // Implémentation de la méthode abstraite de la classe Cell
    std::string display() const override;
};

#endif // FILLED_CELL_H