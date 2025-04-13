#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "sudokuGenerator.c"
#include "settings.c"
#include "board.c"
#include "game.c"



void clear_and_print(const char* message) {
    clear_screen();
    if (message != NULL) {
        printf("%s\n", message);
    }
}


int convertInput(char* str, int boardN, int* row, int* col, int* num) {
    for (char* p = str; *p; ++p) {
        *p = tolower(*p); // all input lowercase
    }

    if (strcmp(str, "exit") == 0) return 0; // quit
    if (strcmp(str, "hint") == 0) return 2; // hint
    if (strcmp(str, "save") == 0) return 3; // save
    if (strcmp(str, "load") == 0) return 4; // load

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

void playSudoku(int *boardN, int *k,int *level)
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
        printf("Other commands: \"exit\", \"hint\", \"save\", \"load\" \n");
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
            case 3://save
                saveGame(board, solution, *boardN, *k, *level, mistakes, secounds);
                quit = true;
                break;
            case 4:
                loadGame(&board, &solution, boardN, k,level, &mistakes, &secounds);
                blockSize = (int)sqrt(*boardN);
                clear_screen();
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
