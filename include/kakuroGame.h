#ifndef KAKURO_GAME_H
#define KAKURO_GAME_H

#include "grid.h"
#include "grid_factory.h"
#include <string>
#include <vector>

class KakuroGame {
private:
    Grid* grid;
    Grid* solutionGrid;  // Nouvelle grille pour stocker la solution correcte
    bool isSolved;
    std::vector<std::string> gridFiles;
    
    // Méthodes privées pour l'interface utilisateur
    void displayMenu() const;
    void displayGridFiles() const;
    bool processCommand(const std::string& command);
    void scanGridFiles(const std::string& directory);
    bool playManually();
    
    // Nouvelle méthode pour vérifier la grille du joueur
    void checkPlayerGrid() const;
    
public:
    KakuroGame();
    ~KakuroGame();
    
    // Méthodes principales
    bool loadGrid(const std::string& filename, const std::string& type);
    bool solve();
    void display() const;
    void run();
};

#endif // KAKURO_GAME_H