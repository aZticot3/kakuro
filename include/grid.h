#ifndef GRID_H
#define GRID_H

#include <vector>
#include "cell.h"
#include <string>

class Grid {
private:
    int height;
    int width;
    std::vector<std::vector<Cell*>> cells;  // Notez que c'est un vector de pointeurs Cell*

public:
    Grid(int height, int width);
    virtual ~Grid();
    
    int getHeight() const;
    int getWidth() const;
    Cell* getCell(int row, int col) const;
    void setCell(int row, int col, Cell* cell);
    void display() const;
    
    // Méthode virtuelle pour charger une grille depuis un fichier
    virtual bool loadFromFile(const std::string& filename) = 0;
};

#endif // GRID_H