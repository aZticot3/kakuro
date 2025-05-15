#ifndef BLACK_CELL_H
#define BLACK_CELL_H

#include "cell.h"
#include <string>

class BlackCell : public Cell {
public:
    BlackCell(int row, int col);
    virtual ~BlackCell();
    
    // Implémentation de la méthode abstraite de la classe Cell
    std::string display() const override;
};

#endif // BLACK_CELL_H