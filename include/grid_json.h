#ifndef GRID_JSON_H
#define GRID_JSON_H

#include "grid.h"
#include <string>

class Grid_Json : public Grid {
public:
    Grid_Json(int height, int width);
    virtual ~Grid_Json();
    
    // Méthodes spécifiques pour le format JSON
    bool loadFromFile(const std::string& filename) override;
    bool validerDonnee(int valeur); // Validation pour format JSON
};

#endif // GRID_JSON_H