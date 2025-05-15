#ifndef CLUE_CELL_H
#define CLUE_CELL_H

#include "cell.h"
#include <string>
#include <vector>

class ClueCell : public Cell {
private:
    int rightSum;  // Somme attendue pour les cellules à droite
    int downSum;   // Somme attendue pour les cellules en dessous
public:
    ClueCell(int row, int col, int rightSum, int downSum);
    virtual ~ClueCell();
    
    // Obtenir les sommes
    int getRightSum() const;
    int getDownSum() const;
      
    // Implémentation de la méthode abstraite de la classe Cell
    std::string display() const override;
};

#endif // CLUE_CELL_H