#include <stdbool.h>
#include <stdlib.h>
#include "hFiles/sudokuGenerator.h"

bool unUsedInBox(int** grid, int rowStart, int colStart, int num, int blockSize) {
    for (int i = 0; i < blockSize; i++)
        for (int j = 0; j < blockSize; j++)
            if (grid[rowStart + i][colStart + j] == num)
                return false;
    return true;
}

void fillBox(int** grid, int row, int col, int blockSize, int boardN) {
    for (int i = 0; i < blockSize; i++) {
        for (int j = 0; j < blockSize; j++) {
            while (true) {
                int num = rand() % boardN + 1;
                if (unUsedInBox(grid, row, col, num, blockSize)) {
                    grid[row + i][col + j] = num;
                    break;
                }
            }
        }
    }
}

// filling the boxes for example 4x4 -> 1,1 and 2,2
void fillDiagonal(int **grid, int blockSize, int boardN) {
    for (int i = 0; i < boardN; i += blockSize) {
        fillBox(grid, i, i, blockSize, boardN);
    }
}

bool unUsedInRow(int** grid, int i, int num, int boardN) {
    for (int j = 0; j < boardN; j++)
        if (grid[i][j] == num)
            return false;
    return true;
}

bool unUsedInCol(int** grid, int j, int num, int boardN) {
    for (int i = 0; i < boardN; i++)
        if (grid[i][j] == num)
            return false;
    return true;
}

bool checkIfSafe(int** grid, int i, int j, int num, int blockSize, int boardN) {
    return unUsedInRow(grid, i, num,boardN) &&
           unUsedInCol(grid, j, num,boardN) &&
           unUsedInBox(grid, i - i % blockSize, j - j % blockSize, num, blockSize);
}

bool fillRemaining(int** grid, int i, int j, int blockSize, int boardN) {
    if (i == boardN) return true;
    if (j == boardN) return fillRemaining(grid, i + 1, 0, blockSize, boardN);
    if (grid[i][j] != 0) return fillRemaining(grid, i, j + 1, blockSize, boardN);

    for (int num = 1; num <= boardN; num++) {
        if (checkIfSafe(grid, i, j, num, blockSize, boardN)) {
            grid[i][j] = num;
            if (fillRemaining(grid, i, j + 1, blockSize, boardN))
                return true;
            grid[i][j] = 0;
        }
    }
    return false;
}



void removeNDigits(int** grid, int N, int boardN) {
    while (N > 0) {
        int cellId = rand() % (boardN * boardN);
        int i = cellId / boardN;
        int j = cellId % boardN;

        if (grid[i][j] != 0) {
            grid[i][j] = 0;
            N--;
        }
    }
}

void copyGrid(int** source, int** destination, int boardN) {
    for (int i = 0; i < boardN; i++) {
        for (int j = 0; j < boardN; j++) {
            destination[i][j] = source[i][j];
        }
    }
}



// Generate a Sudoku grid with K empty cells
void sudokuGenerator(int **grid, int **solution, int k, int blockSize, int boardN)
{
    bool isComplete = false;
    do
    {
        // clear the grid
        for (int i = 0; i < boardN; i++) {
            for (int j = 0; j < boardN; j++) {
                grid[i][j] = 0;
            }
        }

        //generate a sudoku grid using backtracking
        fillDiagonal(grid, blockSize, boardN);
        isComplete = fillRemaining(grid, 0, 0, blockSize, boardN);


        //copy the filled grid to solution if grid is correct
        if (isComplete)
        {
            copyGrid(grid, solution, boardN);
            removeNDigits(grid, boardN*boardN - k, boardN);
        }
    } while (!isComplete);
}
