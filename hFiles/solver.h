#ifndef SOLVER_H
#define SOLVER_H

#include <stdbool.h>

void insertRandomValues(int **grid, int **frozenGrid, int blockSize, int boardN);
void generateNeighbor(int **grid, int **frozenGrid, int blockSize);
int calculateCost(int **board, int blockSize, int boardN);
void solve_knapsack_sa(int **board, int **solution, int blockSize, int boardN,
                      double T_start, double T_end, double alpha, int max_iterations);
void FindSolutions(int* boardN, int* k, int* level);

#endif
