// JsonGridCreator.h
#ifndef JSON_GRID_CREATOR_H
#define JSON_GRID_CREATOR_H

#include "gridCreator.h"

class JsonGridCreator : public GridCreator {
public:
    Grid* createGrid(int height, int width) const override;
    bool canHandleType(const std::string& type) const override;
};

#endif // JSON_GRID_CREATOR_H