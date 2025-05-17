#include "../include/grid_factory.h"
#include "../include/GridCreator.h"
#include "../include/DefaultGridCreator.h"
#include "../include/JsonGridCreator.h"
#include "../include/grid.h"

// Initialisation des membres statiques
std::vector<std::unique_ptr<GridCreator>> GridFactory::creators;
bool GridFactory::isInitialized = false;

void GridFactory::initialize() {
    if (!isInitialized) {
        // Enregistrement des créateurs par défaut
        registerCreator(std::make_unique<DefaultGridCreator>());
        registerCreator(std::make_unique<JsonGridCreator>());
        isInitialized = true;
    }
}

void GridFactory::registerCreator(std::unique_ptr<GridCreator> creator) {
    creators.push_back(std::move(creator));
}

Grid* GridFactory::creerGrid(const std::string& type, int hauteur, int largeur) {
    // S'assurer que les créateurs par défaut sont enregistrés
    initialize();
    
    // Chercher un créateur qui peut gérer ce type
    for (const auto& creator : creators) {
        if (creator->canHandleType(type)) {
            return creator->createGrid(hauteur, largeur);
        }
    }
    
    // Aucun créateur trouvé pour ce type
    return nullptr;
}

Grid* GridFactory::creerGrid(GridType type, int hauteur, int largeur) {
    // Convertir l'énumération en string pour réutiliser l'autre méthode
    switch (type) {
        case GridType::DEFAULT:
            return creerGrid("default", hauteur, largeur);
        case GridType::JSON:
            return creerGrid("json", hauteur, largeur);
        default:
            return nullptr;
    }
}