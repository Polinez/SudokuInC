#include <stdio.h>
#include <stdlib.h>
#include "hFiles/board.h"

void printSeparator(int blocksPerRow, int blockSize, int cell_width) {
    printf("   +");
    for (int b = 0; b < blocksPerRow; b++) {
        for (int j = 0; j < blockSize; j++) {
            for (int k = 0; k < cell_width + 1; k++) {
                printf("-");
            }
        }
        printf("+");
    }
    printf("\n");
}

void printBoard(int** board, int blockSize, int boardN) {
    int cell_width = (boardN > 9) ? 2 : 1; // wider cells for larger boards
    int blocksPerRow = boardN / blockSize;

    // 1- boardN
    printf("    "); // gap before column numbers
    for (int b = 0; b < blocksPerRow; b++) {
        for (int j = 0; j < blockSize; j++) {
            int col = b * blockSize + j;
            if (col >= boardN) break;

            if (boardN > 9) {
                printf("%3d", col + 1); // 3 chars (num + 2 space)
            } else {
                printf("%2d", col + 1); // 2 chars (num + 1 space)
            }
        }
        printf(" "); // gap between blocks
    }
    printf("\n");

    // Top separator
    printSeparator(blocksPerRow, blockSize, cell_width);

    for (int i = 0; i < boardN; i++) {
        // Horizontal separator between blocks
        if (i % blockSize == 0 && i != 0) {
            printSeparator(blocksPerRow, blockSize, cell_width);
        }

        // A-Z
        printf("%c  |", 'A' + i);

        for (int j = 0; j < boardN; j++) {
            // Vertical separator between blocks
            if (j % blockSize == 0 && j != 0) {
                printf("|");
            }

            // value of cell
            if (board[i][j] == 0) {
                printf(" %*s", cell_width, ".");
            } else {
                printf(" %*d", cell_width, board[i][j]);
            }
        }

        // Right separator
        printf(" |\n");
    }

    // Bottom separator
    printSeparator(blocksPerRow, blockSize, cell_width);
}


void freeBoard(int **board, int boardN)
{
    for (int i = 0; i < boardN; i++) {
        free(board[i]);
    }
    free(board);
}

void allocateBoard(int ***board, int boardN) {
    *board = malloc(sizeof(int *) * boardN);
    if (*board == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < boardN; i++) {
        (*board)[i] = malloc(sizeof(int) * boardN);
    }

}
