#ifndef GRID_FACTORY_H
#define GRID_FACTORY_H

#include "grid.h"
#include "grid_default.h"
#include "grid_json.h"
#include <string>

class GridFactory {
public:
    // Énumération des types de grilles disponibles
    enum class GridType {
        DEFAULT,
        JSON
    };
    
    // Méthode statique pour créer une grille du type demandé
    static Grid* creerGrid(const std::string& type, int hauteur, int largeur);
    
    // Surcharge de la méthode avec l'énumération
    static Grid* creerGrid(GridType type, int hauteur, int largeur);
};

#endif // GRID_FACTORY_H