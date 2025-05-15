#ifndef KAKURO_SOLVER_H
#define KAKURO_SOLVER_H

#include "grid.h"
#include "clueCell.h"
#include <vector>

class KakuroSolver {
private:
    Grid* grid;
    
    // Méthodes privées pour l'algorithme de résolution
    bool backtrack();
    bool isSumValid(ClueCell* clue);
    bool isValueValid(int row, int col, int value);
    std::vector<std::pair<int, int>> getEmptyCells();
    
public:
    KakuroSolver(Grid* grid);
    ~KakuroSolver();
    
    bool solve();
};

#endif // KAKURO_SOLVER_H