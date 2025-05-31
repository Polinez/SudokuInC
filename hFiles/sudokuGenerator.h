#ifndef SUDOKU_GENERATOR_H
#define SUDOKU_GENERATOR_H

#include <stdbool.h>

void sudokuGenerator(int **grid, int **solution, int k, int blockSize, int boardN);


bool unUsedInBox(int **grid, int rowStart, int colStart, int num, int blockSize);
void fillBox(int **grid, int row, int col, int blockSize, int boardN);
void fillDiagonal(int **grid, int blockSize, int boardN);
bool unUsedInRow(int **grid, int i, int num, int boardN);
bool unUsedInCol(int **grid, int j, int num, int boardN);
bool checkIfSafe(int **grid, int i, int j, int num, int blockSize, int boardN);
bool fillRemaining(int **grid, int i, int j, int blockSize, int boardN);
void removeNDigits(int **grid, int N, int boardN);
void copyGrid(int **source, int **destination, int boardN);

#endif
