#include "solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "settings.h"
#include "sudokuGenerator.h"
#include "board.h"
#include "game.h"



void insertRandomValues(int **grid, int **frozenGrid, int blockSize, int boardN) {
    // place num from 1 do 9 in every block
    for (int blockRow = 0; blockRow < blockSize; blockRow++) {
        for (int blockCol = 0; blockCol < blockSize; blockCol++) {
            // num 1-9
            int available[boardN];
            for (int i = 0; i < boardN; i++) {
                available[i] = i + 1;
            }

            // check if the number is already in the block if yes set them to 0
            for (int i = 0; i < blockSize; i++) {
                for (int j = 0; j < blockSize; j++) {
                    int row = blockRow * blockSize + i;
                    int col = blockCol * blockSize + j;
                    if (grid[row][col] != 0) {
                        available[grid[row][col] - 1] = 0;
                    }
                }
            }


            // make a list of available numbers
            int missing[boardN];
            int missingCount = 0;
            for (int i = 0; i < boardN; i++) {
                if (available[i] != 0) {
                    missing[missingCount++] = available[i];
                }
            }

            // shuffle the missing numbers
            for (int i = missingCount - 1; i > 0; i--) {
                int j = rand() % (i + 1);
                int temp = missing[i];
                missing[i] = missing[j];
                missing[j] = temp;
            }

            // fill the block with missing numbers
            int missingIndex = 0;
            for (int i = 0; i < blockSize; i++) {
                for (int j = 0; j < blockSize; j++) {
                    int row = blockRow * blockSize + i;
                    int col = blockCol * blockSize + j;
                    if (grid[row][col] == 0 && !frozenGrid[row][col]) {
                        grid[row][col] = missing[missingIndex++];
                    }
                }
            }
        }
    }
}




void generateNeighbor(int **grid,int **frozenGrid, int blockSize)
{
    int blockRow = rand() % blockSize;
    int blockCol = rand() % blockSize;

    int row1, col1, row2, col2;
    do {
        row1 = blockRow * blockSize + rand() % blockSize;
        col1 = blockCol * blockSize + rand() % blockSize;
        row2 = blockRow * blockSize + rand() % blockSize;
        col2 = blockCol * blockSize + rand() % blockSize;
    } while ((row1 == row2 && col1 == col2) || frozenGrid[row1][col1] || frozenGrid[row2][col2]);

    int temp = grid[row1][col1];
    grid[row1][col1] = grid[row2][col2];
    grid[row2][col2] = temp;

}

int calculateCost(int **board, int blockSize, int boardN)
{
    int conflicts = 0;

    // Check row conflicts
    for (int row = 0; row < boardN; row++) {
        int count[boardN + 1];
        for (int i = 0; i <= boardN; i++) count[i] = 0;

        for (int col = 0; col < boardN; col++) {
            int num = board[row][col];
            if (num != 0) {
                count[num]++;
            }
        }

        for (int num = 1; num <= boardN; num++) {
            if (count[num] > 1) {
                conflicts += (count[num] - 1);
            }
        }
    }

    // Check column conflicts
    for (int col = 0; col < boardN; col++) {
        int count[boardN + 1];
        for (int i = 0; i <= boardN; i++) count[i] = 0;

        for (int row = 0; row < boardN; row++) {
            int num = board[row][col];
            if (num != 0) {
                count[num]++;
            }
        }

        for (int num = 1; num <= boardN; num++) {
            if (count[num] > 1) {
                conflicts += (count[num] - 1);
            }
        }
    }


    return conflicts;
}


void solve_knapsack_sa(int **board, int **solution, int blockSize, int boardN,// board is a frozenGrid thet we can't change
                        double T_start,double T_end,double alpha,int max_iterations)
{
    int **current_solution = NULL;
    int **best_solution = NULL;
    int **previous_solution = NULL;

    allocateBoard(&current_solution, boardN);
    allocateBoard(&best_solution, boardN);
    allocateBoard(&previous_solution, boardN);

    copyGrid(board, current_solution, boardN);

    insertRandomValues(current_solution,board, blockSize, boardN);
    copyGrid(current_solution, best_solution, boardN);
    copyGrid(current_solution, previous_solution, boardN);

    double T = T_start;
    int iterations = 0;
    int current_cost = calculateCost(current_solution, blockSize, boardN);
    int best_cost = current_cost;
    int previous_cost = current_cost;

    while (T > T_end && iterations < max_iterations && best_cost > 0) {

        copyGrid(current_solution, previous_solution, boardN);
        previous_cost = current_cost;

        generateNeighbor(current_solution, board, blockSize);
        current_cost = calculateCost(current_solution, blockSize, boardN);

        int deltaCost = current_cost - previous_cost;

        if (deltaCost <= 0) { // if the cost is lower, accept the new solution
            if (current_cost < best_cost) {
                copyGrid(current_solution, best_solution, boardN);
                best_cost = current_cost;
            }
        } else {
            double p = exp(-deltaCost / T);
            if ((double)rand() / RAND_MAX >= p) {
                copyGrid(previous_solution, current_solution, boardN);
                current_cost = previous_cost;
            }
        }

        //cooling
        T *= alpha;
        iterations++;
        printf("current_cost: %d, best_cost: %d, diff: %d, T: %f, iterations: %d\n", current_cost, best_cost, deltaCost, T, iterations);
    }
    printf("\n----- Solved -----\n");
    printf("cost: %d\n", best_cost);
    printBoard(best_solution, blockSize, boardN);

    freeBoard(current_solution, boardN);
    freeBoard(best_solution, boardN);
    freeBoard(previous_solution, boardN);
}


void FindSolutions(int* boardN, int* k, int* level)
{
    clear_screen();

    int blockSize = (int)sqrt(*boardN); // √n

    int **board = NULL;
    int **solution = NULL;
    allocateBoard(&board,*boardN);
    allocateBoard(&solution,*boardN);
    initializeGame(board,solution,*boardN, *k, blockSize);

    // SA parametrs
    double T_start = 1000;
    double T_end = 0.01;
    double alpha = 0.999;
    int max_iterations = 200000;

    solve_knapsack_sa(board, solution, blockSize,*boardN,T_start, T_end, alpha, max_iterations);

    printf("\nPress Enter to continue...");
    while (getchar() != '\n');
    getchar();
}
