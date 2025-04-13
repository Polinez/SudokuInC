#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>
#include <time.h>
#include "sudokuGenerator.c"
#include "settings.c"

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





void clear_and_print(const char* message) {
    clear_screen();
    if (message != NULL) {
        printf("%s\n", message);
    }
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


void printStatistics(int secounds, int mistakes)
{
    printf("\n ----- Statistics -----\n");
    printf("\nTime taken: %d hours %d minutes %d seconds\n", secounds / 3600, (secounds % 3600) / 60, secounds % 60);
    printf("\nTotal mistakes: %d\n", mistakes);
    printf("------------------------\n");
}

int convertInput(char* str, int boardN, int* row, int* col, int* num) {
    for (char* p = str; *p; ++p) {
        *p = tolower(*p); // all input lowercase
    }

    if (strcmp(str, "quit") == 0) {
        return 0; // quit
    }
    if (strcmp(str, "hint") == 0) {
        return 2; // hint
    }

    char rowChar;
    if (sscanf(str, "%c%d %d", &rowChar, col, num) == 3) {
        rowChar = toupper(rowChar);
        *row = rowChar - 'A'; // 'A' → 0, 'B' → 1 etc.

        // Validate input
        if (*row < 0 || *row >= boardN || *col < 1 || *col > boardN || *num < 1 || *num > boardN) {
            return -1;
        }

        *col -= 1;
        return 1; // Valid input
    }

    return -1; // Wrong format
}

void playSudoku(int *boardN, int *k)
{
    clear_screen();

    int blockSize = (int)sqrt(*boardN); // √n

    int **board = NULL;
    int **solution = NULL;
    allocateBoard(&board,*boardN);
    allocateBoard(&solution,*boardN);
    initializeGame(board,solution,*boardN, *k, blockSize);


    char input[10];
    int row, col, num;
    bool gameWon = false;
    bool quit = false;
    int hintRow = 0, hintCol = 0;

    // statistics
    int mistakes = 0;
    int secounds = 0;
    timerStart(&secounds);

    while (!quit && !gameWon) {
        printBoard(board, blockSize, *boardN);
        printf("Enter row (A-%c) column (1-%d), and number (1-%d):\n", 'A' + *boardN - 1, *boardN, *boardN);
        printf("Example: A1 5\n");
        printf("Write \"quit\" to exit\n");
        printf("Write \"hint\" to get hint\n");
        printf("Your input: ");

        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        int answer = convertInput(input, *boardN, &row, &col, &num);

        switch (answer) {
            case -1: // Invalid input
                clear_and_print("Invalid input! Please try again.");
                continue;

            case 0: // Quit
                clear_and_print("Quitting game...");
                quit = true;
                break;

            case 2: // Hint
                do {
                    hintRow = rand() % *boardN;
                    hintCol = rand() % *boardN;
                } while (board[hintRow][hintCol] != 0);

                board[hintRow][hintCol] = solution[hintRow][hintCol];
                clear_screen();
                printf("Hint provided in %c%d: %d\n", 'A' + hintRow, hintCol + 1, board[hintRow][hintCol]);
                break;

            case 1: // current input
                if (board[row][col] != 0) {
                    clear_and_print("This cell is already filled!");
                    continue;
                }

                if (checkIfCorrect(row, col, num, solution)) {
                    board[row][col] = num;
                    clear_and_print("Correct!");
                } else {
                    clear_and_print("Incorrect! Try again.");
                    mistakes++;
                }
                break;
    }


    if (isBoardComplete(board, *boardN)) {
        gameWon = true;
        secounds = timerStop(&secounds);

        clear_and_print("\nCongratulations! You've solved the Sudoku!");
        printBoard(board, blockSize, *boardN);

        printStatistics(secounds, mistakes);
        printf("\nPress any key to exit...");
        scanf("%*s"); // Wait for user input
    }
}


    freeBoard(board,*boardN);
    freeBoard(solution,*boardN);

}
