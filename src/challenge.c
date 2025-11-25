#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "challenge.h"
#include "utils.h"
#include "caesar.h"
#include "vigenere.h"
#include "playfair.h"
#include "hill.h"
#include "transposition.h"
#include "leaderboard.h"

#define CONFIG_FILE "data/config.txt"

static int enabledCiphers[10];
static int cipherCount = 0;
static int numLevels = 10;
static int timeLimit = 60;
static int minPass = 7;

static time_t start;

// Sample challenge texts
static const char* sampleTexts[] = {
    "HELLO WORLD",
    "SECURE THE DATA",
    "MEET AT DAWN",
    "ENCRYPT THIS",
    "CRYPTOGRAPHY IS FUN",
    "DECODE THE SECRET",
    "ATTACK AT ONCE",
    "FIND THE PASSWORD",
    "KEEP THE KEY SAFE"
};

static int sampleCount = 9;

// -------------------------------
// CONFIG SAVE/LOAD
// -------------------------------

static void save_config_internal() {
    FILE* f = fopen(CONFIG_FILE, "w");
    if (!f) return;

    fprintf(f, "%d\n%d\n%d\n%d ", numLevels, timeLimit, minPass, cipherCount);
    for (int i = 0; i < cipherCount; i++)
        fprintf(f, "%d ", enabledCiphers[i]);

    fclose(f);
}

static void load_config_internal() {
    FILE* f = fopen(CONFIG_FILE, "r");
    if (!f) {
        enabledCiphers[0] = 1;
        enabledCiphers[1] = 2;
        enabledCiphers[2] = 3;
        enabledCiphers[3] = 4;
        enabledCiphers[4] = 5;
        cipherCount = 5;
        save_config_internal();
        return;
    }

    fscanf(f, "%d %d %d %d", &numLevels, &timeLimit, &minPass, &cipherCount);

    for (int i = 0; i < cipherCount; i++)
        fscanf(f, "%d", &enabledCiphers[i]);

    fclose(f);
}

// -------------------------------
// PICK RANDOM CIPHER
// -------------------------------

static int pick_random_cipher() {
    if (cipherCount <= 0) return 1;
    int idx = random_int(0, cipherCount - 1);
    return enabledCiphers[idx];
}

// -------------------------------
// ROUND GENERATION
// -------------------------------

static void generate_round(Challenge* r) {
    r->cipherId = pick_random_cipher();
    r->timeLimit = timeLimit;

    strcpy(r->plaintext, sampleTexts[random_int(0, sampleCount - 1)]);
    r->isEncrypt = random_int(0, 1);

    switch (r->cipherId) {
        case 1: sprintf(r->key, "%d", random_int(1, 25)); break;
        case 2: strcpy(r->key, "KEY"); break;
        case 3: strcpy(r->key, "SECRET"); break;
        case 4: strcpy(r->key, "9 4 5 7"); break;
        case 5: strcpy(r->key, "CRYPTO"); break;
        default: strcpy(r->key, "KEY"); break;
    }

    if (r->isEncrypt) {
        strcpy(r->challengeText, r->plaintext);

        if (r->cipherId == 1) {
            char *tmp = caesar_encrypt(r->plaintext, atoi(r->key));
            strcpy(r->expectedAnswer, tmp);
            free(tmp);
        }
        else if (r->cipherId == 2) {
            char *tmp = vigenere_encrypt(r->plaintext, r->key);
            strcpy(r->expectedAnswer, tmp);
            free(tmp);
        }
        else if (r->cipherId == 3) {
            char *tmp = playfair_encrypt(r->plaintext, r->key);
            strcpy(r->expectedAnswer, tmp);
            free(tmp);
        }
        else if (r->cipherId == 4) {
            int K[2][2] = {{9,4},{5,7}};
            char *tmp = hill_encrypt(r->plaintext, K);
            strcpy(r->expectedAnswer, tmp);
            free(tmp);
        }
        else if (r->cipherId == 5) {
            char *tmp = transposition_encrypt(r->plaintext, r->key);
            strcpy(r->expectedAnswer, tmp);
            free(tmp);
        } else {
            strcpy(r->expectedAnswer, r->plaintext);
        }
    } else {
        /* Provide ciphertext as challengeText, expectedAnswer = plaintext */
        if (r->cipherId == 1) {
            char *ct = caesar_encrypt(r->plaintext, atoi(r->key));
            strcpy(r->challengeText, ct);
            free(ct);
            strcpy(r->expectedAnswer, r->plaintext);
        }
        else if (r->cipherId == 2) {
            char *ct = vigenere_encrypt(r->plaintext, r->key);
            strcpy(r->challengeText, ct);
            free(ct);
            strcpy(r->expectedAnswer, r->plaintext);
        }
        else if (r->cipherId == 3) {
            char *ct = playfair_encrypt(r->plaintext, r->key);
            strcpy(r->challengeText, ct);
            free(ct);
            strcpy(r->expectedAnswer, r->plaintext);
        }
        else if (r->cipherId == 4) {
            int K[2][2] = {{9,4},{5,7}};
            char *ct = hill_encrypt(r->plaintext, K);
            strcpy(r->challengeText, ct);
            free(ct);
            strcpy(r->expectedAnswer, r->plaintext);
        }
        else if (r->cipherId == 5) {
            char *ct = transposition_encrypt(r->plaintext, r->key);
            strcpy(r->challengeText, ct);
            free(ct);
            strcpy(r->expectedAnswer, r->plaintext);
        } else {
            strcpy(r->challengeText, r->plaintext);
            strcpy(r->expectedAnswer, r->plaintext);
        }
    }
}

// -------------------------------
// PLAY ONE ROUND
// -------------------------------

static void play_single_round(int roundNumber, Challenge* r, int* score, int* passCount) {
    printf("\n========================\n");
    printf(" ROUND %d\n", roundNumber);
    printf("========================\n");

    printf("\nCipher Used: ");
    switch (r->cipherId) {
        case 1: printf("Caesar\n"); break;
        case 2: printf("Vigenere\n"); break;
        case 3: printf("Playfair\n"); break;
        case 4: printf("Hill\n"); break;
        case 5: printf("Columnar Transposition\n"); break;
        default: printf("Unknown\n"); break;
    }

    printf("Task: %s\n", r->isEncrypt ? "Encrypt" : "Decrypt");

    if (r->isEncrypt) {
        // Only show plaintext for encryption task
        printf("Plaintext:  %s\n", r->challengeText);
    }
     else {
        // Only show ciphertext for decryption task
        printf("Ciphertext: %s\n", r->challengeText);
    }
    printf("Key:        %s\n", r->key);
    printf("Time Limit: %d seconds\n\n", r->timeLimit);


    printf("Your Answer: ");
    char answer[MAX_TEXT];
    if (!fgets(answer, sizeof(answer), stdin)) {
        trim_newline(answer);
        str_upper(answer);
    } else {
        trim_newline(answer);
        str_upper(answer);
    }

    time_t end = time(NULL);
    int taken = (int)(end - start);

    if (taken > r->timeLimit) {
        printf("\nTime's up! You took %d seconds.\n", taken);
        printf("Correct Answer: %s\n", r->expectedAnswer);
        *score -= 10;
        return;
    }

    if (strcmp(answer, r->expectedAnswer) == 0) {
        printf("\nCorrect! (%d sec)\n", taken);
        *score += 100;
        if (taken < r->timeLimit / 2)
            *score += 20; // speed bonus
        (*passCount)++;
    } else {
        printf("\nWrong.\n");
        printf("Correct: %s\n", r->expectedAnswer);
        *score -= 10;
    }
}

// -------------------------------
// PLAY FULL GAME
// -------------------------------

void play_challenge_mode() {
    load_config_internal();

    printf("\n=== STORY MODE: DEFEND THE ORGANIZATION ===\n");
    printf("A hacker is trying to steal confidential data.\n");
    printf("Solve the encryption tasks before time runs out!\n\n");

    printf("Enter your name: ");
    char playerName[100];
    /* eat any leftover newline from previous scanf */
    if (!fgets(playerName, sizeof(playerName), stdin)) {
        playerName[0] = '\0';
    }
    trim_newline(playerName);

    if (strlen(playerName) == 0) {
        strcpy(playerName, "PLAYER");
    }

    srand((unsigned)time(NULL));

    int score = 0;
    int passCount = 0;

    for (int level = 1; level <= numLevels; level++) {
        Challenge r;
        generate_round(&r);

        start = time(NULL);
        play_single_round(level, &r, &score, &passCount);

        printf("\nCurrent Score: %d\n", score);
        press_enter();
    }

    printf("\n========================\n");
    printf(" FINAL RESULTS\n");
    printf("========================\n");

    printf("Score: %d\n", score);
    printf("Passed: %d/%d\n", passCount, numLevels);

    if (passCount >= minPass)
        printf("\n🎉 Result: SUCCESS — You saved the organization!\n");
    else
        printf("\n💀 Result: FAILED — The hacker won.\n");

    add_score(playerName, score);

    press_enter();
}

// -------------------------------
// CONFIGURATION VIEW + EDIT
// -------------------------------

void configure_challenge() {
    load_config_internal();

    while (1) {
        printf("\n=== CHALLENGE CONFIGURATION ===\n");
        printf("1. Number of Levels (Current: %d)\n", numLevels);
        printf("2. Time per Round in seconds (Current: %d)\n", timeLimit);
        printf("3. Minimum Pass Count (Current: %d)\n", minPass);
        printf("4. Toggle Ciphers (enabled currently:");
        for (int i=0;i<cipherCount;i++) printf(" %d", enabledCiphers[i]);
        printf(" )\n");
        printf("5. Save & Back\n");
        printf("Enter choice: ");

        int ch = 0;
        if (scanf("%d", &ch) != 1) {
            printf("Invalid input.\n");
            continue;
        }

        if (ch == 1) {
            printf("Enter new number of levels: ");
            if (scanf("%d", &numLevels) != 1) { ; printf("Invalid\n"); continue; }
        } else if (ch == 2) {
            printf("Enter new time limit (sec): ");
            if (scanf("%d", &timeLimit) != 1) { ; printf("Invalid\n"); continue; }
            ;
        } else if (ch == 3) {
            printf("Enter new minimum pass count: ");
            if (scanf("%d", &minPass) != 1) { ; printf("Invalid\n"); continue; }
            ;
        } else if (ch == 4) {
            printf("Enter cipher ID to toggle (1=Caesar,2=Vigenere,3=Playfair,4=Hill,5=Transposition): ");
            int id; if (scanf("%d", &id) != 1) { ; printf("Invalid\n"); continue; }
            ;
            int found = -1;
            for (int i = 0; i < cipherCount; i++)
                if (enabledCiphers[i] == id) { found = i; break; }
            if (found >= 0) {
                for (int i = found; i < cipherCount - 1; i++) enabledCiphers[i] = enabledCiphers[i+1];
                cipherCount--;
                if (cipherCount < 0) cipherCount = 0;
            } else {
                if (cipherCount < 10) enabledCiphers[cipherCount++] = id;
            }
        } else if (ch == 5) {
            save_config_internal();
            printf("Configuration saved.\n");
            press_enter();
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }
}
