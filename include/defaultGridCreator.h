// DefaultGridCreator.h
#ifndef DEFAULT_GRID_CREATOR_H
#define DEFAULT_GRID_CREATOR_H

#include "gridCreator.h"

class DefaultGridCreator : public GridCreator {
public:
    Grid* createGrid(int height, int width) const override;
    bool canHandleType(const std::string& type) const override;
};

#endif // DEFAULT_GRID_CREATOR_H