#ifndef GRID_FACTORY_H
#define GRID_FACTORY_H

#include <string>
#include <vector>
#include <memory>

class Grid;
class GridCreator;

class GridFactory {
public:
    enum class GridType {
        DEFAULT,
        JSON
    };
    
    // Méthodes pour enregistrer des créateurs de grilles
    static void registerCreator(std::unique_ptr<GridCreator> creator);
    
    // Méthodes de création qui utilisent les créateurs enregistrés
    static Grid* creerGrid(const std::string& type, int hauteur, int largeur);
    static Grid* creerGrid(GridType type, int hauteur, int largeur);
    
    // Initialisation des créateurs par défaut
    static void initialize();

private:
    static std::vector<std::unique_ptr<GridCreator>> creators;
    static bool isInitialized;
};

#endif // GRID_FACTORY_H