#include <stdlib.h>
#include <math.h>
#include <time.h>

int** initializeGame(int **board,int **solution,int boardN, int k, int blockSize) {

    srand(time(0));
    sudokuGenerator(board,solution, k, blockSize, boardN);

    return board;
}


bool checkIfCorrect(int i, int j, int num,int **solution)
{
    return solution[i][j] == num;
}

bool isBoardComplete(int **board, int boardN) {
    for (int i = 0; i < boardN; i++) {
        for (int j = 0; j < boardN; j++) {
            if (board[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}


void timerStart(int *secounds) {
    time_t startTime = time(NULL);
    *secounds = (int)startTime;
}

int timerStop(int *secounds) {
    time_t endTime = time(NULL);
    int elapsed = (int)endTime - *secounds;
    return elapsed;
}


void printStatistics(int secounds, int mistakes,int hints)
{
    printf("\n ----- Statistics -----\n");
    printf("\nTime taken: %d hours %d minutes %d seconds\n", secounds / 3600, (secounds % 3600) / 60, secounds % 60);
    printf("\nTotal mistakes: %d\n", mistakes);
    printf("\nTotal hints taken: %d\n", hints);
    printf("------------------------\n");
}


void saveGame(int** board, int** solution, int boardN,int blockSize, int k, int level, int mistakes, int seconds) {
    FILE* file = fopen("sudoku.sav", "wb");
    if (!file) {
        printf("Error saving game!\n");
        return;
    }

    // Write metadata
    fwrite(&boardN, sizeof(int), 1, file);
    fwrite(&blockSize, sizeof(int), 1, file);
    fwrite(&k, sizeof(int), 1, file);
    fwrite(&level, sizeof(int), 1, file);
    fwrite(&mistakes, sizeof(int), 1, file);
    fwrite(&seconds, sizeof(int), 1, file);

    // Write board data
    for (int i = 0; i < boardN; i++) {
        fwrite(board[i], sizeof(int), boardN, file);
        fwrite(solution[i], sizeof(int), boardN, file);
    }

    fclose(file);
    printf("Game saved successfully!\n");
}

int loadGame(int*** board, int*** solution, int* boardN,int *blockSize, int* k, int* level, int* mistakes, int* seconds) {
    FILE* file = fopen("sudoku.sav", "rb");
    if (!file) {
        printf("No saved game found!\n");
        return 0;
    }

    // Read metadata
    fread(boardN, sizeof(int), 1, file);
    fread(blockSize, sizeof(int), 1, file);
    fread(k, sizeof(int), 1, file);
    fread(level, sizeof(int), 1, file);
    fread(mistakes, sizeof(int), 1, file);
    fread(seconds, sizeof(int), 1, file);

    // Allocate memory
    *board = malloc(sizeof(int*) * *boardN);
    *solution = malloc(sizeof(int*) * *boardN);
    for (int i = 0; i < *boardN; i++) {
        (*board)[i] = malloc(sizeof(int) * *boardN);
        (*solution)[i] = malloc(sizeof(int) * *boardN);
    }

    // Read board data
    for (int i = 0; i < *boardN; i++) {
        fread((*board)[i], sizeof(int), *boardN, file);
        fread((*solution)[i], sizeof(int), *boardN, file);
    }

    fclose(file);
    printf("Game loaded successfully!\n");
    return 1;
}