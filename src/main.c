#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "challenge.h"
#include "leaderboard.h"
#include "utils.h"

void main_menu() {
    int choice;

    while (1) {
        printf("\n========================================\n");
        printf("   CRYPTO CHALLENGE: DECODE THE SECRETS\n");
        printf("========================================\n");
        printf("1. Learning Mode\n");
        printf("2. Configure Challenge Settings\n");
        printf("3. Story Mode (Play Challenges)\n");
        printf("4. View Leaderboard\n");
        printf("5. Exit\n");
        printf("Enter choice: ");

        scanf("%d", &choice);

        clear_input_buffer();

        switch (choice) {
            case 1:
                learning_menu();
                break;

            case 2:
                configure_challenge();
                break;

            case 3:
                play_challenge_mode();
                break;

            case 4:
                load_leaderboard();
                view_leaderboard();
                break;

            case 5:
                printf("\nExiting... Goodbye!\n");
                return;

            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

int main() {
    system("cls");  // For Windows. On Linux use system("clear")
    main_menu();
    return 0;
}
