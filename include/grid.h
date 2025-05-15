#ifndef GRID_H
#define GRID_H

#include <vector>
#include "cell.h"
#include <string>

class Grid {
private:
    int height;
    int width;
    std::vector<std::vector<Cell*>> cells;  

public:
    Grid(int height, int width);
    virtual ~Grid(); 
    
    int getHeight() const;
    int getWidth() const;
    Cell* getCell(int row, int col) const;
    void setCell(int row, int col, Cell* cell);
    void display() const;
    
    // Add a virtual method for loading files
    virtual bool loadFromFile(const std::string& filename) = 0;
};

#endif // GRID_H