#include "../include/grid_json.h"
#include "../include/blackCell.h"
#include "../include/clueCell.h"
#include "../include/emptyCell.h"
#include "../include/filledCell.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

// Utilisation du namespace pour simplifier le code
using json = nlohmann::json;

Grid_Json::Grid_Json(int height, int width) : Grid(height, width) {
    // Le constructeur de Grid est appelé
}

Grid_Json::~Grid_Json() {
    // Le destructeur de Grid s'occupe de libérer la mémoire des cellules
}

bool Grid_Json::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier " << filename << std::endl;
        return false;
    }
    
    try {
        // Parsing du JSON avec nlohmann/json
        json jsonData;
        file >> jsonData;
        file.close();
        
        // Vérifier les dimensions
        if (!jsonData.contains("height") || !jsonData.contains("width") || !jsonData.contains("cells")) {
            std::cerr << "Erreur: Format JSON invalide - dimensions ou cellules manquantes" << std::endl;
            return false;
        }
        
        int fileHeight = jsonData["height"];
        int fileWidth = jsonData["width"];
        
        if (fileHeight != getHeight() || fileWidth != getWidth()) {
            std::cerr << "Erreur: Les dimensions dans le fichier (" << fileHeight << "x" << fileWidth 
                      << ") ne correspondent pas à celles de la grille (" << getHeight() << "x" << getWidth() << ")" << std::endl;
            return false;
        }
        
        // Traitement des cellules
        for (const auto& cell : jsonData["cells"]) {
            if (!cell.contains("row") || !cell.contains("col") || !cell.contains("type")) {
                std::cerr << "Erreur: Cellule mal formatée dans le JSON" << std::endl;
                continue;
            }
            
            int row = cell["row"];
            int col = cell["col"];
            std::string type = cell["type"];
            
            if (row < 0 || row >= getHeight() || col < 0 || col >= getWidth()) {
                std::cerr << "Erreur: Indices hors limites pour la cellule: (" << row << ", " << col << ")" << std::endl;
                continue;
            }
            
            if (type == "black") {
                setCell(row, col, new BlackCell(row, col));
            } else if (type == "empty") {
                setCell(row, col, new EmptyCell(row, col));
            } else if (type == "clue") {
                int downSum = 0;
                int rightSum = 0;
                
                if (cell.contains("downSum")) {
                    downSum = cell["downSum"];
                }
                
                if (cell.contains("rightSum")) {
                    rightSum = cell["rightSum"];
                }
                
                setCell(row, col, new ClueCell(row, col, rightSum, downSum));
            } else if (type == "filled") {
                int value = 0;
                
                if (cell.contains("value")) {
                    value = cell["value"];
                    if (value < 1 || value > 9) {
                        std::cerr << "Erreur: Valeur invalide pour une cellule remplie: " << value << std::endl;
                        continue;
                    }
                } else {
                    std::cerr << "Erreur: Valeur manquante pour une cellule remplie" << std::endl;
                    continue;
                }
                
                setCell(row, col, new FilledCell(row, col, value));
            } else {
                std::cerr << "Erreur: Type de cellule inconnu: " << type << std::endl;
            }
        }
        
        return true;
    } catch (const json::exception& e) {
        std::cerr << "Erreur JSON: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du parsing: " << e.what() << std::endl;
        return false;
    }
}

bool Grid_Json::validerDonnee(int valeur) {
    // Une valeur valide est entre 1 et 9 pour le Kakuro
    return valeur >= 1 && valeur <= 9;
}