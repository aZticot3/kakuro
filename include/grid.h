#ifndef GRID_H
#define GRID_H

#include <vector>
#include "cell.h"

class Grid {
private:
    int height;
    int width;
    std::vector<std::vector<Cell*>> cells;  // Notez que c'est un vector de pointeurs Cell*

public:
    Grid(int height, int width);
    ~Grid();
    
    int getHeight() const;
    int getWidth() const;
    Cell* getCell(int row, int col) const;
    void setCell(int row, int col, Cell* cell);
    void display() const;
};

#endif // GRID_H