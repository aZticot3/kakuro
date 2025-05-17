#include "../include/kakuroGame.h"
#include "../include/grid.h"
#include "../include/grid_factory.h"
#include "../include/kakuroSolver.h"
#include "../include/emptyCell.h"
#include "../include/filledCell.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <limits>
#include <nlohmann/json.hpp>

// Constructeur
KakuroGame::KakuroGame() : grid(nullptr), solutionGrid(nullptr), isSolved(false) {
    // Scan des fichiers de grilles disponibles au démarrage
    GridFactory::initialize();
    scanGridFiles("grilles");
}

// Destructeur
KakuroGame::~KakuroGame() {
    delete grid;
    delete solutionGrid;
}

// Méthode pour charger une grille à partir d'un fichier
bool KakuroGame::loadGrid(const std::string& filename, const std::string& type) {
    // Extraction des dimensions de la grille depuis le fichier
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier " << filename << std::endl;
        return false;
    }
    
    int height, width;
    
    // Pour JSON, nous devons parser le fichier différemment
    if (type == "json" || type == "Json" || type == "JSON") {
        try {
            // Utilisation de nlohmann/json pour lire les dimensions
            nlohmann::json jsonData;
            file >> jsonData;
            
            if (!jsonData.contains("height") || !jsonData.contains("width")) {
                std::cerr << "Erreur: Dimensions manquantes dans le fichier JSON" << std::endl;
                file.close();
                return false;
            }
            
            height = jsonData["height"];
            width = jsonData["width"];
        } catch (const std::exception& e) {
            std::cerr << "Erreur lors du parsing JSON: " << e.what() << std::endl;
            file.close();
            return false;
        }
    } else {
        // Format texte standard
        file >> height >> width;
    }
    
    file.close();
    
    // Libération de la grille précédente si elle existe
    if (grid != nullptr) {
        delete grid;
        grid = nullptr;
    }
    
    if (solutionGrid != nullptr) {
        delete solutionGrid;
        solutionGrid = nullptr;
    }
    
    // Création d'une nouvelle grille du type spécifié
    grid = GridFactory::creerGrid(type, height, width);
    
    // Création d'une copie pour la solution
    solutionGrid = GridFactory::creerGrid(type, height, width);
    
    if (grid == nullptr || solutionGrid == nullptr) {
        std::cerr << "Erreur: Type de grille non reconnu: " << type << std::endl;
        return false;
    }
    
    // Charger la grille principale
    bool success = grid->loadFromFile(filename);
    
    // Charger également la grille de solution
    if (success) {
        success = solutionGrid->loadFromFile(filename);
        
        // Résoudre la grille de solution
        if (success) {
            KakuroSolver solver(solutionGrid);
            success = solver.solve();
            if (!success) {
                std::cerr << "Erreur: Impossible de résoudre la grille pour la solution" << std::endl;
            }
        }
    }
    
    return success;
}

// Implémentation de la méthode de vérification
void KakuroGame::checkPlayerGrid() const {
    if (grid == nullptr || solutionGrid == nullptr) {
        std::cout << "Aucune grille chargée." << std::endl;
        return;
    }
    
    std::cout << "\nVérification de votre solution:" << std::endl;
    grid->displayWithValidation(*solutionGrid);
    
    // Compter les erreurs
    int errorCount = 0;
    int filledCount = 0;
    
    for (int i = 0; i < grid->getHeight(); ++i) {
        for (int j = 0; j < grid->getWidth(); ++j) {
            Cell* playerCell = grid->getCell(i, j);
            Cell* solutionCell = solutionGrid->getCell(i, j);
            
            EmptyCell* playerEmptyCell = dynamic_cast<EmptyCell*>(playerCell);
            if (playerEmptyCell && playerEmptyCell->getValue() > 0) {
                filledCount++;
                
                // Vérifier la solution
                int solutionValue = 0;
                EmptyCell* solutionEmptyCell = dynamic_cast<EmptyCell*>(solutionCell);
                FilledCell* solutionFilledCell = dynamic_cast<FilledCell*>(solutionCell);
                
                if (solutionEmptyCell) {
                    solutionValue = solutionEmptyCell->getValue();
                } else if (solutionFilledCell) {
                    solutionValue = solutionFilledCell->getValue();
                }
                
                if (playerEmptyCell->getValue() != solutionValue) {
                    errorCount++;
                }
            }
        }
    }
    
    // Afficher le résultat
    if (errorCount == 0 && filledCount > 0) {
        std::cout << "\nFélicitations! Votre solution est correcte." << std::endl;
    } else {
        std::cout << "\nVous avez " << errorCount << " erreur(s) sur " << filledCount << " case(s) remplie(s)." << std::endl;
    }
}
// Méthode pour résoudre la grille automatiquement
bool KakuroGame::solve() {
    if (grid == nullptr) {
        std::cerr << "Erreur: Aucune grille chargée" << std::endl;
        return false;
    }
    
    KakuroSolver solver(grid);
    bool solved = solver.solve();
    isSolved = solved;
    
    if (solved) {
        std::cout << "La grille a été résolue avec succès!" << std::endl;
    } else {
        std::cout << "Impossible de résoudre la grille." << std::endl;
    }
    
    return solved;
}

// Méthode pour afficher la grille
void KakuroGame::display() const {
    if (grid == nullptr) {
        std::cout << "Aucune grille chargée." << std::endl;
        return;
    }
    
    std::cout << "\nGrille actuelle:" << std::endl;
    grid->display();
}

// Méthode principale qui gère l'interface utilisateur
void KakuroGame::run() {
    std::string command;
    bool running = true;
    
    std::cout << "=== Bienvenue dans le jeu de Kakuro ===" << std::endl;
    
    while (running) {
        displayMenu();
        std::cout << "Votre choix: ";
        std::getline(std::cin, command);
        
        running = processCommand(command);
    }
    
    std::cout << "Merci d'avoir joué au Kakuro!" << std::endl;
}

// Affichage du menu principal
void KakuroGame::displayMenu() const {
    std::cout << "\n=== Menu Principal ===" << std::endl;
    std::cout << "1. Charger une grille" << std::endl;
    std::cout << "2. Afficher la grille actuelle" << std::endl;
    std::cout << "3. Jouer manuellement" << std::endl;
    std::cout << "4. Résoudre automatiquement" << std::endl;
    std::cout << "5. Vérifier ma solution" << std::endl;
    std::cout << "6. Quitter" << std::endl;
}

// Affichage des fichiers de grilles disponibles
void KakuroGame::displayGridFiles() const {
    std::cout << "\nFichiers de grilles disponibles:" << std::endl;
    
    for (size_t i = 0; i < gridFiles.size(); ++i) {
        std::cout << i + 1 << ". " << gridFiles[i] << std::endl;
    }
}

// Traitement des commandes utilisateur
bool KakuroGame::processCommand(const std::string& command) {
    if (command == "1") {
        // Charger une grille
        if (gridFiles.empty()) {
            std::cout << "Aucun fichier de grille trouvé dans le répertoire 'grilles'." << std::endl;
            return true;
        }
        
        displayGridFiles();
        
        std::cout << "Sélectionnez un fichier (1-" << gridFiles.size() << "): ";
        std::string fileChoice;
        std::getline(std::cin, fileChoice);
        
        try {
            int choice = std::stoi(fileChoice);
            if (choice >= 1 && choice <= static_cast<int>(gridFiles.size())) {
                std::string filename = "grilles/" + gridFiles[choice - 1];
                
                // Déterminer le type de fichier
                std::string type;
                if (filename.substr(filename.length() - 5) == ".json") {
                    type = "json";
                } else {
                    type = "default";
                }
                
                if (loadGrid(filename, type)) {
                    std::cout << "Grille chargée avec succès!" << std::endl;
                    display();
                } else {
                    std::cout << "Échec du chargement de la grille." << std::endl;
                }
            } else {
                std::cout << "Choix invalide." << std::endl;
            }
        } catch (const std::invalid_argument&) {
            std::cout << "Entrée invalide. Veuillez entrer un nombre." << std::endl;
        }
    } else if (command == "2") {
        // Afficher la grille actuelle
        display();
    } else if (command == "3") {
        // Jouer manuellement
        if (grid == nullptr) {
            std::cout << "Vous devez d'abord charger une grille." << std::endl;
            return true;
        }
        
        bool gameCompleted = playManually();
        if (gameCompleted) {
            std::cout << "Félicitations! Vous avez résolu la grille!" << std::endl;
        }
    } else if (command == "4") {
        // Résoudre automatiquement
        if (grid == nullptr) {
            std::cout << "Vous devez d'abord charger une grille." << std::endl;
            return true;
        }
        
        solve();
        display();
    } else if (command == "5") {
        // Vérifier la grille
        if (grid == nullptr) {
            std::cout << "Vous devez d'abord charger une grille." << std::endl;
            return true;
        }
        
        checkPlayerGrid();
    } else if (command == "6") {
        // Quitter
        return false;
    } else {
        std::cout << "Commande non reconnue. Veuillez réessayer." << std::endl;
    }
    
    return true;
}

// Scan des fichiers de grilles dans un répertoire
void KakuroGame::scanGridFiles(const std::string& directory) {
    gridFiles.clear();
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                // Filtrer pour ne garder que les fichiers .txt et .json
                if (filename.substr(filename.length() - 4) == ".txt" || 
                    filename.substr(filename.length() - 5) == ".json") {
                    gridFiles.push_back(filename);
                }
            }
        }
        
        // Trier les fichiers par ordre alphabétique
        std::sort(gridFiles.begin(), gridFiles.end());
    } catch (const std::filesystem::filesystem_error&) {
        std::cerr << "Erreur: Impossible d'accéder au répertoire '" << directory << "'" << std::endl;
    }
}

// Méthode pour permettre à l'utilisateur de jouer manuellement
bool KakuroGame::playManually() {
    if (grid == nullptr || solutionGrid == nullptr) {
        std::cout << "Erreur: Grille non chargée." << std::endl;
        return false;
    }
    
    bool playing = true;
    
    while (playing) {
        display();
        
        std::cout << "\n=== Jouer manuellement ===" << std::endl;
        std::cout << "'ligne col valeur' pour jouer | 'v' vérifier | 'h' indice | 'ligne col c' effacer | 'q' quitter" << std::endl;
        std::cout << "Action: ";
        
        std::string input;
        std::getline(std::cin, input);
        
        // Commandes spéciales
        if (input == "q") return false;
        else if (input == "v") {
            checkPlayerGrid();
            std::cout << "\nAppuyez sur Entrée pour continuer...";
            std::cin.get();
            continue;
        } else if (input == "h") {
            // Trouver toutes les cellules vides
            std::vector<std::pair<int, int>> emptyCells;
            for (int i = 0; i < grid->getHeight(); ++i) {
                for (int j = 0; j < grid->getWidth(); ++j) {
                    EmptyCell* ec = dynamic_cast<EmptyCell*>(grid->getCell(i, j));
                    if (ec && ec->getValue() == 0) emptyCells.push_back({i, j});
                }
            }
            
            if (emptyCells.empty()) {
                std::cout << "Pas d'indice disponible - toutes les cases sont remplies!" << std::endl;
            } else {
                // Choisir une cellule vide au hasard
                int index = rand() % emptyCells.size();
                int row = emptyCells[index].first;
                int col = emptyCells[index].second;
                
                // Obtenir la valeur correcte
                int correctValue = 0;
                Cell* sc = solutionGrid->getCell(row, col);
                EmptyCell* sec = dynamic_cast<EmptyCell*>(sc);
                FilledCell* sfc = dynamic_cast<FilledCell*>(sc);
                
                if (sec) correctValue = sec->getValue();
                else if (sfc) correctValue = sfc->getValue();
                
                // Placer la valeur
                if (correctValue > 0) {
                    EmptyCell* ec = dynamic_cast<EmptyCell*>(grid->getCell(row, col));
                    ec->setValue(correctValue);
                    std::cout << "Indice: " << correctValue << " à (" << row + 1 << ", " << col + 1 << ")" << std::endl;
                }
            }
            std::cout << "\nAppuyez sur Entrée pour continuer...";
            std::cin.get();
            continue;
        }
        
        // Entrée de jeu standard
        std::istringstream iss(input);
        int row, col;
        std::string valOrCmd;
        
        if (iss >> row >> col >> valOrCmd) {
            row--; col--; // Conversion 1-based à 0-based
            
            // Vérifier indices valides
            if (row >= 0 && row < grid->getHeight() && col >= 0 && col < grid->getWidth()) {
                EmptyCell* ec = dynamic_cast<EmptyCell*>(grid->getCell(row, col));
                if (ec) {
                    // Effacement
                    if (valOrCmd == "c") {
                        ec->setValue(0);
                        std::cout << "Case effacée." << std::endl;
                    } 
                    // Valeur numérique
                    else {
                        try {
                            int val = std::stoi(valOrCmd);
                            if (val >= 1 && val <= 9) {
                                ec->setValue(val);
                                
                                // Feedback immédiat
                                Cell* sc = solutionGrid->getCell(row, col);
                                int correctVal = 0;
                                EmptyCell* sec = dynamic_cast<EmptyCell*>(sc);
                                FilledCell* sfc = dynamic_cast<FilledCell*>(sc);
                                
                                if (sec) correctVal = sec->getValue();
                                else if (sfc) correctVal = sfc->getValue();
                                
                                
                                // Vérifier si complet
                                bool complete = true;
                                bool allCorrect = true;
                                
                                for (int i = 0; i < grid->getHeight() && complete; ++i) {
                                    for (int j = 0; j < grid->getWidth() && complete; ++j) {
                                        EmptyCell* cell = dynamic_cast<EmptyCell*>(grid->getCell(i, j));
                                        if (cell) {
                                            if (cell->getValue() == 0) {
                                                complete = false;
                                            } else {
                                                // Vérifier exactitude
                                                Cell* solCell = solutionGrid->getCell(i, j);
                                                int solVal = 0;
                                                EmptyCell* solEC = dynamic_cast<EmptyCell*>(solCell);
                                                FilledCell* solFC = dynamic_cast<FilledCell*>(solCell);
                                                
                                                if (solEC) solVal = solEC->getValue();
                                                else if (solFC) solVal = solFC->getValue();
                                                
                                                if (cell->getValue() != solVal) allCorrect = false;
                                            }
                                        }
                                    }
                                }
                                
                                if (complete) {
                                    if (allCorrect) {
                                        std::cout << COLOR_GREEN << "\nFélicitations! Grille résolue!" << COLOR_RESET << std::endl;
                                        display();
                                        std::cout << "\nAppuyez sur Entrée pour continuer...";
                                        std::cin.get();
                                        return true;
                                    } else {
                                        std::cout << "\nGrille complète mais avec des erreurs." << std::endl;
                                        checkPlayerGrid();
                                    }
                                }
                            } else {
                                std::cout << "Erreur: Valeur doit être entre 1 et 9." << std::endl;
                            }
                        } catch (...) {
                            std::cout << "Commande non reconnue." << std::endl;
                        }
                    }
                } else {
                    std::cout << "Cette cellule ne peut pas être modifiée." << std::endl;
                }
            } else {
                std::cout << "Indices hors limites." << std::endl;
            }
        } else {
            std::cout << "Format invalide. Utilisez 'ligne colonne valeur'." << std::endl;
        }
        
        std::cout << "\nAppuyez sur Entrée pour continuer...";
        std::cin.get();
    }
    
    return false;
}