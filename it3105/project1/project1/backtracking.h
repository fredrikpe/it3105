#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include "board.h"

class BackTracking
{
public:
    BackTracking(Board *board);

    void solve(std::vector<int> queens);


    Board *board;
    std::vector<std::vector<int>> solutions;

    std::vector<std::vector<int>> states;
    std::vector<int> path;
    int currentStep = 0;
};

#endif // BACKTRACKING_H
