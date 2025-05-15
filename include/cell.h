#ifndef CELL_H
#define CELL_H

#include <string>
 
// Codes ANSI pour les couleurs
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RESET "\033[0m"

class Cell {
protected:
    int row;
    int col;

public:
    Cell(int row, int col);
    virtual ~Cell();
    
    int getRow() const;
    int getCol() const;
    
    // Méthode abstraite qui doit être implémentée par les classes dérivées
    virtual std::string display() const = 0;
    
    // Nouvelle méthode pour afficher avec couleur
    virtual std::string displayColored(bool isCorrect) const;
};

#endif // CELL_H