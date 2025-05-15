#ifndef GRID_DEFAULT_H
#define GRID_DEFAULT_H

#include "grid.h"
#include <string>

class Grid_Default : public Grid {
public:
    Grid_Default(int height, int width);
    virtual ~Grid_Default();
    
    // Méthodes spécifiques pour le format par défaut
    bool loadFromFile(const std::string& filename) override;

    bool validerDonnee(int valeur); // Validation pour format par défaut
};

#endif // GRID_DEFAULT_H