#include "../include/kakuroSolver.h"
#include "../include/emptyCell.h"
#include "../include/clueCell.h"
#include "../include/filledCell.h"
#include <vector>
#include <utility>
#include <iostream>

// Constructeur
KakuroSolver::KakuroSolver(Grid* grid) : grid(grid) {
    // grid est une référence, ne pas le supprimer dans le destructeur
}

// Destructeur
KakuroSolver::~KakuroSolver() {
    // grid n'est pas la propriété de KakuroSolver, ne pas le supprimer ici
}

// Méthode principale de résolution
bool KakuroSolver::solve() {
    if (grid == nullptr) {
        return false;
    }
    
    return backtrack();
}

// Méthode récursive de backtracking pour résoudre la grille
bool KakuroSolver::backtrack() {
    // Récupérer toutes les cellules vides
    std::vector<std::pair<int, int>> emptyCells = getEmptyCells();
    
    // Si plus de cellules vides, la grille est résolue
    if (emptyCells.empty()) {
        return true;
    }
    
    // Prendre la première cellule vide
    int row = emptyCells[0].first;
    int col = emptyCells[0].second;
    
    // Récupérer la cellule
    EmptyCell* cell = dynamic_cast<EmptyCell*>(grid->getCell(row, col));
    if (cell == nullptr) {
        return false;  // Erreur de cast
    }
    
    // Essayer chaque valeur possible
    for (int value = 1; value <= 9; ++value) {
        if (isValueValid(row, col, value)) {
            // Placer la valeur
            cell->setValue(value);
            
            // Continuer avec la prochaine cellule
            if (backtrack()) {
                return true;
            }
            
            // Si pas de solution, revenir en arrière
            cell->setValue(0);
        }
    }
    
    // Aucune solution trouvée
    return false;
}

// Vérifier si une valeur est valide à une position donnée
bool KakuroSolver::isValueValid(int row, int col, int value) {
    // Vérifier que la valeur est entre 1 et 9
    if (value < 1 || value > 9) {
        return false;
    }
    
    // Trouver les indices de début et de fin pour la ligne
    int startCol = col;
    int endCol = col;
    
    // Chercher le début de la séquence de la ligne
    while (startCol > 0) {
        Cell* cell = grid->getCell(row, startCol - 1);
        if (dynamic_cast<EmptyCell*>(cell) || dynamic_cast<FilledCell*>(cell)) {
            startCol--;
        } else {
            break;
        }
    }
    
    // Chercher la fin de la séquence de la ligne
    while (endCol < grid->getWidth() - 1) {
        Cell* cell = grid->getCell(row, endCol + 1);
        if (dynamic_cast<EmptyCell*>(cell) || dynamic_cast<FilledCell*>(cell)) {
            endCol++;
        } else {
            break;
        }
    }
    
    // Vérifier que la valeur n'apparaît pas déjà dans la séquence de la ligne
    for (int j = startCol; j <= endCol; ++j) {
        if (j != col) {
            Cell* cell = grid->getCell(row, j);
            EmptyCell* emptyCell = dynamic_cast<EmptyCell*>(cell);
            FilledCell* filledCell = dynamic_cast<FilledCell*>(cell);
            
            if (emptyCell && emptyCell->getValue() == value) {
                return false;
            } else if (filledCell && filledCell->getValue() == value) {
                return false;
            }
        }
    }
    
    // Trouver les indices de début et de fin pour la colonne
    int startRow = row;
    int endRow = row;
    
    // Chercher le début de la séquence de la colonne
    while (startRow > 0) {
        Cell* cell = grid->getCell(startRow - 1, col);
        if (dynamic_cast<EmptyCell*>(cell) || dynamic_cast<FilledCell*>(cell)) {
            startRow--;
        } else {
            break;
        }
    }
    
    // Chercher la fin de la séquence de la colonne
    while (endRow < grid->getHeight() - 1) {
        Cell* cell = grid->getCell(endRow + 1, col);
        if (dynamic_cast<EmptyCell*>(cell) || dynamic_cast<FilledCell*>(cell)) {
            endRow++;
        } else {
            break;
        }
    }
    
    // Vérifier que la valeur n'apparaît pas déjà dans la séquence de la colonne
    for (int i = startRow; i <= endRow; ++i) {
        if (i != row) {
            Cell* cell = grid->getCell(i, col);
            EmptyCell* emptyCell = dynamic_cast<EmptyCell*>(cell);
            FilledCell* filledCell = dynamic_cast<FilledCell*>(cell);
            
            if (emptyCell && emptyCell->getValue() == value) {
                return false;
            } else if (filledCell && filledCell->getValue() == value) {
                return false;
            }
        }
    }
    
    // Trouver la cellule d'indice pour la ligne (si elle existe)
    ClueCell* rowClue = nullptr;
    if (startCol > 0) {
        Cell* cell = grid->getCell(row, startCol - 1);
        rowClue = dynamic_cast<ClueCell*>(cell);
    }
    
    // Trouver la cellule d'indice pour la colonne (si elle existe)
    ClueCell* colClue = nullptr;
    if (startRow > 0) {
        Cell* cell = grid->getCell(startRow - 1, col);
        colClue = dynamic_cast<ClueCell*>(cell);
    }
    
    // Calculer la somme actuelle pour la ligne
    int rowSum = 0;
    int rowCellCount = 0;
    bool rowComplete = true;
    
    for (int j = startCol; j <= endCol; ++j) {
        Cell* cell = grid->getCell(row, j);
        EmptyCell* emptyCell = dynamic_cast<EmptyCell*>(cell);
        FilledCell* filledCell = dynamic_cast<FilledCell*>(cell);
        
        if (emptyCell) {
            if (j == col) {
                rowSum += value;
            } else if (emptyCell->getValue() > 0) {
                rowSum += emptyCell->getValue();
            } else {
                rowComplete = false;
            }
            rowCellCount++;
        } else if (filledCell) {
            rowSum += filledCell->getValue();
            rowCellCount++;
        }
    }
    
    // Vérifier la contrainte de somme pour la ligne si elle est complète
    if (rowClue && rowClue->getRightSum() > 0) {
        // Si la ligne est complète, vérifier que la somme est correcte
        if (rowComplete && rowSum != rowClue->getRightSum()) {
            return false;
        }
        
        // Si la somme dépasse déjà l'indice, c'est invalide
        if (rowSum > rowClue->getRightSum()) {
            return false;
        }
    }
    
    // Calculer la somme actuelle pour la colonne
    int colSum = 0;
    int colCellCount = 0;
    bool colComplete = true;
    
    for (int i = startRow; i <= endRow; ++i) {
        Cell* cell = grid->getCell(i, col);
        EmptyCell* emptyCell = dynamic_cast<EmptyCell*>(cell);
        FilledCell* filledCell = dynamic_cast<FilledCell*>(cell);
        
        if (emptyCell) {
            if (i == row) {
                colSum += value;
            } else if (emptyCell->getValue() > 0) {
                colSum += emptyCell->getValue();
            } else {
                colComplete = false;
            }
            colCellCount++;
        } else if (filledCell) {
            colSum += filledCell->getValue();
            colCellCount++;
        }
    }
    
    // Vérifier la contrainte de somme pour la colonne si elle est complète
    if (colClue && colClue->getDownSum() > 0) {
        // Si la colonne est complète, vérifier que la somme est correcte
        if (colComplete && colSum != colClue->getDownSum()) {
            return false;
        }
        
        // Si la somme dépasse déjà l'indice, c'est invalide
        if (colSum > colClue->getDownSum()) {
            return false;
        }
    }
    
    // Toutes les vérifications ont réussi
    return true;
}

// Récupérer toutes les cellules vides de la grille
std::vector<std::pair<int, int>> KakuroSolver::getEmptyCells() {
    std::vector<std::pair<int, int>> emptyCells;
    
    for (int i = 0; i < grid->getHeight(); ++i) {
        for (int j = 0; j < grid->getWidth(); ++j) {
            Cell* cell = grid->getCell(i, j);
            EmptyCell* emptyCell = dynamic_cast<EmptyCell*>(cell);
            
            if (emptyCell && emptyCell->getValue() == 0) {
                emptyCells.push_back(std::make_pair(i, j));
            }
        }
    }
    
    return emptyCells;
}

// Vérifier si les sommes d'une cellule d'indice sont valides
bool KakuroSolver::isSumValid(ClueCell* clue) {
    if (clue == nullptr) {
        return false;
    }
    
    int row = clue->getRow();
    int col = clue->getCol();
    
    // Vérifier la somme vers la droite
    if (clue->getRightSum() > 0) {
        int sum = 0;
        std::vector<int> values;
        int j = col + 1;
        
        while (j < grid->getWidth()) {
            Cell* cell = grid->getCell(row, j);
            EmptyCell* emptyCell = dynamic_cast<EmptyCell*>(cell);
            FilledCell* filledCell = dynamic_cast<FilledCell*>(cell);
            
            if (emptyCell) {
                if (emptyCell->getValue() == 0) {
                    // Cellule vide, on ne peut pas vérifier complètement
                    break;
                }
                sum += emptyCell->getValue();
                values.push_back(emptyCell->getValue());
            } else if (filledCell) {
                sum += filledCell->getValue();
                values.push_back(filledCell->getValue());
            } else {
                // Fin de la séquence
                break;
            }
            
            j++;
        }
        
        // Vérifier si des valeurs sont dupliquées
        for (size_t i = 0; i < values.size(); ++i) {
            for (size_t k = i + 1; k < values.size(); ++k) {
                if (values[i] == values[k]) {
                    return false;
                }
            }
        }
        
        // Si on a parcouru toute la séquence, vérifier la somme
        if (j == grid->getWidth() || (grid->getCell(row, j) && !dynamic_cast<EmptyCell*>(grid->getCell(row, j)) && !dynamic_cast<FilledCell*>(grid->getCell(row, j)))) {
            if (sum != clue->getRightSum()) {
                return false;
            }
        }
    }
    
    // Vérifier la somme vers le bas
    if (clue->getDownSum() > 0) {
        int sum = 0;
        std::vector<int> values;
        int i = row + 1;
        
        while (i < grid->getHeight()) {
            Cell* cell = grid->getCell(i, col);
            EmptyCell* emptyCell = dynamic_cast<EmptyCell*>(cell);
            FilledCell* filledCell = dynamic_cast<FilledCell*>(cell);
            
            if (emptyCell) {
                if (emptyCell->getValue() == 0) {
                    // Cellule vide, on ne peut pas vérifier complètement
                    break;
                }
                sum += emptyCell->getValue();
                values.push_back(emptyCell->getValue());
            } else if (filledCell) {
                sum += filledCell->getValue();
                values.push_back(filledCell->getValue());
            } else {
                // Fin de la séquence
                break;
            }
            
            i++;
        }
        
        // Vérifier si des valeurs sont dupliquées
        for (size_t i = 0; i < values.size(); ++i) {
            for (size_t k = i + 1; k < values.size(); ++k) {
                if (values[i] == values[k]) {
                    return false;
                }
            }
        }
        
        // Si on a parcouru toute la séquence, vérifier la somme
        if (i == grid->getHeight() || (grid->getCell(i, col) && !dynamic_cast<EmptyCell*>(grid->getCell(i, col)) && !dynamic_cast<FilledCell*>(grid->getCell(i, col)))) {
            if (sum != clue->getDownSum()) {
                return false;
            }
        }
    }
    
    return true;
}