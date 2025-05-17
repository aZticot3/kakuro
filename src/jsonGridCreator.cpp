#include "../include/jsonGridCreator.h"
#include "../include/grid_json.h"

Grid* JsonGridCreator::createGrid(int height, int width) const {
    return new Grid_Json(height, width);
}

bool JsonGridCreator::canHandleType(const std::string& type) const {
    return (type == "json" || type == "Json" || type == "JSON");
}