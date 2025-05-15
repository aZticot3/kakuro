#include "../include/grid_default.h"
#include "../include/blackCell.h"
#include "../include/clueCell.h"
#include "../include/emptyCell.h"
#include "../include/filledCell.h"
#include <fstream>
#include <sstream>
#include <iostream>

Grid_Default::Grid_Default(int height, int width) : Grid(height, width) {
    // Le constructeur de Grid est appelé
}

Grid_Default::~Grid_Default() {
    // Le destructeur de Grid s'occupe de libérer la mémoire des cellules
}

bool Grid_Default::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier " << filename << std::endl;
        return false;
    }
    
    int height, width;
    file >> height >> width;
    
    // Vérifier que les dimensions correspondent à celles de la grille
    if (height != getHeight() || width != getWidth()) {
        std::cerr << "Erreur: Les dimensions du fichier ne correspondent pas à celles de la grille" << std::endl;
        file.close();
        return false;
    }
    
    // Consommer la fin de ligne
    std::string dummy;
    std::getline(file, dummy);
    
    for (int i = 0; i < height; ++i) {
        std::string line;
        std::getline(file, line);
        std::istringstream iss(line);
        
        for (int j = 0; j < width; ++j) {
            std::string token;
            iss >> token;
            
            // Analyser le token pour déterminer le type de cellule
            if (token == "#") {
                // Case noire
                setCell(i, j, new BlackCell(i, j));
            } else if (token == "_") {
                // Case vide à remplir
                setCell(i, j, new EmptyCell(i, j));
            } else if (token.find('/') != std::string::npos) {
                // Case avec indices de somme
                int rightSum = 0, downSum = 0;
                
                // Format: downSum/rightSum ou /rightSum ou downSum/
                size_t slashPos = token.find('/');
                
                // Traiter la partie avant le slash (downSum)
                if (slashPos > 0 && token[0] != '/') {
                    std::string downPart = token.substr(0, slashPos);
                    if (downPart != "") {
                        downSum = std::stoi(downPart);
                    }
                }
                
                // Traiter la partie après le slash (rightSum)
                if (slashPos < token.length() - 1) {
                    std::string rightPart = token.substr(slashPos + 1);
                    if (rightPart != "") {
                        rightSum = std::stoi(rightPart);
                    }
                }
                
                setCell(i, j, new ClueCell(i, j, rightSum, downSum));
            } else {
                // Essayer de convertir en nombre
                try {
                    int value = std::stoi(token);
                    setCell(i, j, new FilledCell(i, j, value));
                } catch (const std::invalid_argument&) {
                    std::cerr << "Erreur: Token non reconnu: " << token << std::endl;
                    file.close();
                    return false;
                }
            }
        }
    }
    
    file.close();
    return true;
}


bool Grid_Default::validerDonnee(int valeur) {
    // Une valeur valide est entre 1 et 9 pour le Kakuro
    return valeur >= 1 && valeur <= 9;
}