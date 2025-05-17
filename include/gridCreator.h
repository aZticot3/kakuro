#ifndef GRID_CREATOR_H
#define GRID_CREATOR_H

#include <string>
#include <memory>

class Grid;

class GridCreator {
public:
    virtual ~GridCreator() = default;
    virtual Grid* createGrid(int height, int width) const = 0;
    virtual bool canHandleType(const std::string& type) const = 0;
};

#endif // GRID_CREATOR_H