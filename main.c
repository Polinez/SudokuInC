#include "play.c"

int boardN = 9;
int level = 1;
int k = 17; //TODO change to 17

int main() {
    int choice;
    do {
        clear_screen();
        printf("\n--- MENU ---\n");
        printf("1. Play\n");
        printf("2. Choose board size\n");
        printf("3. Choose level\n");
        printf("4. Instruction\n");
        printf("0. Exit\n");
        printf("Place your option: ");

        if (scanf("%d", &choice) != 1) {
            printf("Error: place number from menu.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:
            playSudoku(&boardN, &k);
            break;
        case 2:
            chooseBoardN(&boardN, &k, &level);
            break;
        case 3:
            chooseLevel(&boardN, &k, &level);
            break;
        case 4:
            instruction();
            break;
        case 0:
            clear_screen();
            printf("bye!\n");
            break;
        default:
            printf("Error: place number from menu.\n");
        }
    } while (choice != 0);

    return 0;
}
