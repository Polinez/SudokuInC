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

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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
    char command[13];
    int tempCol;

    // Check for remove command format: "A1 -r"
    if (sscanf(str, "%c%d %s", &rowChar, &tempCol, command) == 3) {
        if (strcmp(command, "-r") == 0) {
            rowChar = toupper(rowChar);
            *row = rowChar - 'A'; // 'A' → 0, 'B' → 1 etc.
            *col = tempCol - 1;

            if (*row < 0 || *row >= boardN || *col < 0 || *col >= boardN) {
                return -2; // Invalid row/column
            }

            return 5; // Remove command
        }
    }

    // Check for number input format: "A1 5"
    if (sscanf(str, "%c%d %d", &rowChar, col, num) == 3) {
        rowChar = toupper(rowChar);
        *row = rowChar - 'A'; // 'A' → 0, 'B' → 1 etc.
        *col -= 1;

        // Validate input
        if (*row < 0 || *row >= boardN || *col < 0 || *col >= boardN || *num < 1 || *num > boardN) {
            return -2; // Invalid row/column/number
        }

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
    int countNum = 0;

    // statistics
    int mistakes = 0;
    int secounds = 0;
    int hints= 0;
    timerStart(&secounds);

    while (!quit && !gameWon) {
        printBoard(board, blockSize, *boardN);
        printf("\n");
        printBoard(solution, blockSize, *boardN);
        printf("Enter row (A-%c) column (1-%d), and number (1-%d):\n", 'A' + *boardN - 1, *boardN, *boardN);
        printf("Example: A1 5\n");
        printf("To remove value: A1 -r\n");
        printf("Other commands: \"exit\", \"hint\", \"save\", \"load\" \n");
        printf("Your input: ");

        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        int answer = convertInput(input, *boardN, &row, &col, &num);

        switch (answer) {
            case -2: // invalid row/column/number
                clear_and_print("Invalid row/column/number.");
                continue;
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
                hints++;
                clear_screen();
                printf("Hint provided in %c%d: %d\n", 'A' + hintRow, hintCol + 1, board[hintRow][hintCol]);
                break;
            case 3://save game
                saveGame(board, solution, *boardN, blockSize, *k, *level, mistakes, secounds);
                quit = true;
                break;
            case 4: //load game
                loadGame(&board, &solution, boardN, &blockSize, k,level, &mistakes, &secounds);
                clear_screen();
                break;
            case 5: // Remove/delete value
                countNum = 0;
                if (solution[row][col] != 0 && board[row][col] != 0) {
                    //count numbers on the board
                    for (int i = 0; i < *boardN; i++) {
                        for (int j = 0; j < *boardN; j++) {
                            if (board[i][j] != 0 ) {
                                countNum+=1;
                            }
                        }
                    }

                    if (countNum > *k) {
                        board[row][col] = 0;
                        clear_and_print("Value removed!");
                    } else {
                        clear_and_print("Cannot remove! To low number of numbers on the board.");
                    }
                } else {
                    clear_and_print("Cell is already empty!");
                }
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
                    clear_and_print("Incorrect guess! Try again.");
                    mistakes++;
                }
                break;
    }


    if (isBoardComplete(board, *boardN)) {
        gameWon = true;
        secounds = timerStop(&secounds);

        clear_and_print("\nCongratulations! You've solved the Sudoku!");
        printBoard(board, blockSize, *boardN);

        printStatistics(secounds, mistakes,hints);
        printf("\nPress any key to exit...");
        clear_input_buffer();
        getchar();
    }
}


    freeBoard(board,*boardN);
    freeBoard(solution,*boardN);

}
