#ifndef CELL_H
#define CELL_H

#include <string>
 

class Cell {
protected:
    int row;
    int col;

public:
    Cell(int row, int col);
    virtual ~Cell();
    
    int getRow() const;
    int getCol() const;
    
    // Méthode abstraite qui doit être implémentée par les classes dérivées
    virtual std::string display() const = 0;
};

#endif // CELL_H