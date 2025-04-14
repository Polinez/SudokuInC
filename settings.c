#include <stdio.h>
#include <stdlib.h>
#include "settings.h"

void clear_screen() {
    system("clear||cls");
}

//set up K based on level
void setUpK(int boardN, int *k, int level) {
    int M_min;
    switch (boardN) {
    case 4:  M_min = 4; break;
    case 9:  M_min = 17; break;
    case 16: M_min = 55; break;
    default: printf("Error: Invalid board size.\n"); return;
    }

    double percentage;
    switch (level) {
    case 1: percentage = 0.45; break; // Easy
    case 2: percentage = 0.30; break; // Medium
    case 3: percentage = 0.20; break; // Hard
    default:
        printf("Error: Invalid level.\n");
        return;
    }

    *k = (int)(percentage * boardN * boardN);
    *k = (*k < M_min) ? M_min : *k;  // max of M_min and calculated k
}


void chooseLevel(int *boardN, int *k, int *level) {
    int choice;
    do {
        clear_screen();
        printf("\n--- Current level -> %d ---", *level);
        printf("\nChoose level:\n");
        printf("1. Easy\n");
        printf("2. Medium\n");
        printf("3. Hard\n");
        printf("0. Back\n");
        printf("Place youre option: ");

        if (scanf("%d", &choice) != 1) {
            printf("Error: Please enter a number from the menu.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
        case 1: // Easy
            *level = 1;
            setUpK(*boardN, k, *level);
            printf("Easy level: %d prefilled cells\n", (*k));
            break;
        case 2: // Medium
            *level = 2;
            setUpK(*boardN, k, *level);
            printf("Medium level: %d prefilled cells\n", (*k));
            break;
        case 3: // Hard
            *level = 3;
            setUpK(*boardN, k, *level);
            printf("Hard level: %d prefilled cells\n", (*k));
            break;
        case 0:
            printf("Returning to main menu...\n");
            break;
        default:
            printf("Error: Please enter a number from the menu.\n");
        }
    } while (choice != 0);
}


void chooseBoardN(int *boardN, int *k, int *level) {
    int choice;
    do {
        clear_screen();
        printf("\n--- Current board size -> %d ---", *boardN);
        printf("\nChoose size:\n");
        printf("1. 4x4\n");
        printf("2. 9x9\n");
        printf("3. 16x16\n");
        printf("0. Back\n");
        printf("Place youre option: ");

        if (scanf("%d", &choice) != 1) {
            printf("Error: place number from menu.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');


        *level = 1; // default level
        switch (choice) {
        case 1:
            *boardN = 4;
            setUpK(*boardN, k, *level);
            printf("Youre board will be 4x4:\n");
            break;
        case 2:
            *boardN = 9;
            setUpK(*boardN, k, *level);
            printf("Youre board will be 9x9:\n");
            break;
        case 3:
            *boardN = 16;
            setUpK(*boardN, k, *level);
            printf("Youre board will be 16x16:\n");
            break;
        default:
            printf("Error: place number from menu.\n");
        }
    } while (choice != 0);
}


void instruction() {
    int choice;
    do {
        clear_screen();
        printf("=== Instructions ===\n\n");
        printf("Welcome to Sudoku!\n");
        printf("The goal of the game is to fill the grid so that:\n");
        printf("- Each row contains numbers from 1 to n (grid size).\n");
        printf("- Each column contains numbers from 1 to n.\n");
        printf("- Each smaller sub-grid contains numbers from 1 to n.\n\n");
        printf("Game controls:\n");
        printf("1. Enter row, column, and number to place a value. Format: A1 5\n");
        printf("2. Enter \"exit\" to quit the game.\n");
        printf("3. Enter \"hint\" to get a hint\n");
        printf("3. Enter \"save\" to save the game from file\n");
        printf("3. Enter \"load\" to load the game from file\n");
        printf("=======================\n");
        printf("Press 0 to return to the main menu: ");

        if (scanf("%d", &choice) != 1) {
            printf("\nError: Invalid input. Please enter 0 to return.\n");
            while (getchar() != '\n');
        }

    } while (choice != 0);
}
