#include "backtracking.h"

#include <iostream>

BackTracking::BackTracking(Board *board) :
    board(board)
{
}

void BackTracking::solve(std::vector<int> queens)
{
    if (!board->validState(queens))
    {
        path.push_back(-1);
        return;
    }
    if (queens.size() == board->boardSize)  // Solution check
    {
        path.push_back(-1);
        return;
    }
    for (int i=0; i<board->boardSize; i++)
    {
        std::vector<int> new_queens = queens;
        new_queens.push_back(i);
        this->path.push_back(i);
        this->solve(new_queens);
    }
    this->path.push_back(-1);
}
