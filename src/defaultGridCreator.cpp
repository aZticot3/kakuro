// DefaultGridCreator.cpp
#include "../include/defaultGridCreator.h"
#include "../include/grid_default.h"

Grid* DefaultGridCreator::createGrid(int height, int width) const {
    return new Grid_Default(height, width);
}

bool DefaultGridCreator::canHandleType(const std::string& type) const {
    return (type == "default" || type == "Default");
}