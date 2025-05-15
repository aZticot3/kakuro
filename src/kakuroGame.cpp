#include "../include/kakuroGame.h"
#include "../include/kakuroSolver.h"
#include "../include/emptyCell.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <limits>
#include <nlohmann/json.hpp>

// Constructeur
KakuroGame::KakuroGame() : grid(nullptr), isSolved(false) {
    // Scan des fichiers de grilles disponibles au démarrage
    scanGridFiles("grilles");
}

// Destructeur
KakuroGame::~KakuroGame() {
    delete grid;
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
    
    // Création d'une nouvelle grille du type spécifié
    grid = GridFactory::creerGrid(type, height, width);
    if (grid == nullptr) {
        std::cerr << "Erreur: Type de grille non reconnu: " << type << std::endl;
        return false;
    }
    
    // Utilisation du polymorphisme pour charger le fichier avec la méthode appropriée
    return grid->loadFromFile(filename);
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
    std::cout << "5. Quitter" << std::endl;
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
    if (grid == nullptr) {
        return false;
    }
    
    bool playing = true;
    
    while (playing) {
        display();
        
        std::cout << "\n=== Jouer manuellement ===" << std::endl;
        std::cout << "Entrez 'ligne colonne valeur' pour jouer (ex: 2 3 5)" << std::endl;
        std::cout << "Ou 'q' pour revenir au menu principal: ";
        
        std::string input;
        std::getline(std::cin, input);
        
        if (input == "q" || input == "Q") {
            return false;
        }
        
        // Parser l'entrée
        std::istringstream iss(input);
        int row, col, value;
        
        if (iss >> row >> col >> value) {
            // Indices commencent à 0 dans le programme, mais l'utilisateur entre 1-based
            row--;
            col--;
            
            // Vérifier que les indices sont valides
            if (row >= 0 && row < grid->getHeight() && col >= 0 && col < grid->getWidth()) {
                Cell* cell = grid->getCell(row, col);
                
                // Vérifier que c'est une cellule vide
                EmptyCell* emptyCell = dynamic_cast<EmptyCell*>(cell);
                if (emptyCell != nullptr) {
                    emptyCell->setValue(value);
                    std::cout << "Valeur " << value << " placée à la position (" << row + 1 << ", " << col + 1 << ")" << std::endl;
                    
                    // Vérifier si la grille est complète (très basique, à améliorer)
                    bool complete = true;
                    for (int i = 0; i < grid->getHeight() && complete; ++i) {
                        for (int j = 0; j < grid->getWidth() && complete; ++j) {
                            EmptyCell* ec = dynamic_cast<EmptyCell*>(grid->getCell(i, j));
                            if (ec != nullptr && ec->getValue() == 0) {
                                complete = false;
                            }
                        }
                    }
                    
                    if (complete) {
                        std::cout << "Grille complétée!" << std::endl;
                        return true;
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
        
        std::cout << "Appuyez sur Entrée pour continuer...";
        std::cin.get();
    }
    
    return false;
}