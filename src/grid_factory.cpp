#include "../include/grid_factory.h"

Grid* GridFactory::creerGrid(const std::string& type, int hauteur, int largeur) {
    if (type == "default" || type == "Default") {
        return new Grid_Default(hauteur, largeur);
    } else if (type == "json" || type == "Json" || type == "JSON") {
        return new Grid_Json(hauteur, largeur);
    } else {
        // Type non reconnu, retourner nullptr ou une valeur par défaut
        return nullptr;
    }
}

Grid* GridFactory::creerGrid(GridType type, int hauteur, int largeur) {
    switch (type) {
        case GridType::DEFAULT:
            return new Grid_Default(hauteur, largeur);
        case GridType::JSON:
            return new Grid_Json(hauteur, largeur);
        default:
            return nullptr;
    }
}