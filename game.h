#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

int** initializeGame(int **board, int **solution, int boardN, int k, int blockSize);
bool checkIfCorrect(int i, int j, int num, int **solution);
bool isBoardComplete(int **board, int boardN);
void timerStart(int *seconds);
int timerStop(int *seconds);
//void saveGame(int** board, int** solution, int boardN, int blockSize, int k, int level, int mistakes, int seconds);
//int loadGame(int*** board, int*** solution, int* boardN, int* blockSize, int* k, int* level, int* mistakes, int* seconds);
void printStatistics(int seconds, int mistakes, int hints);
void saveGame(int** board, int** solution, int boardN, int blockSize, int k, int level, int mistakes, int hints, int seconds);
int loadGame(int*** board, int*** solution, int* boardN, int* blockSize, int* k, int* level, int* mistakes, int* hints, int* seconds);

#endif
