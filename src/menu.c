#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "menu.h"
#include "caesar.h"
#include "vigenere.h"
#include "playfair.h"
#include "hill.h"
#include "transposition.h"
#include "utils.h"

static void learn_caesar() {
    printf("\n=== Caesar Cipher ===\n");
    printf("Shift-based substitution cipher.\n");

    char text[200];
    int shift;

    printf("Enter plaintext: ");
    fgets(text, sizeof(text), stdin);
    trim_newline(text);

    printf("Enter shift (1-25): ");
    scanf("%d", &shift);
    clear_input_buffer();

    char* enc = caesar_encrypt(text, shift);
    char* dec = caesar_decrypt(enc, shift);

    printf("Encrypted: %s\n", enc);
    printf("Decrypted: %s\n", dec);

    free(enc);
    free(dec);

    press_enter();
}

static void learn_vigenere() {
    printf("\n=== Vigenere Cipher ===\n");

    char text[200], key[50];
    printf("Enter plaintext: ");
    fgets(text, sizeof(text), stdin); trim_newline(text);

    printf("Enter key: ");
    fgets(key, sizeof(key), stdin); trim_newline(key);

    char* enc = vigenere_encrypt(text, key);
    char* dec = vigenere_decrypt(enc, key);

    printf("Encrypted: %s\n", enc);
    printf("Decrypted: %s\n", dec);

    free(enc);
    free(dec);

    press_enter();
}

static void learn_playfair() {
    printf("\n=== Playfair Cipher ===\n");

    char text[200], key[50];
    printf("Enter plaintext: ");
    fgets(text, sizeof(text), stdin); trim_newline(text);

    printf("Enter key: ");
    fgets(key, sizeof(key), stdin); trim_newline(key);

    char* enc = playfair_encrypt(text, key);
    char* dec = playfair_decrypt(enc, key);

    printf("Encrypted: %s\n", enc);
    printf("Decrypted: %s\n", dec);

    free(enc);
    free(dec);

    press_enter();
}

static void learn_hill() {
    printf("\n=== Hill Cipher (2x2) ===\n");

    char text[200];
    printf("Enter plaintext: ");
    fgets(text, sizeof(text), stdin); trim_newline(text);

    int K[2][2] = {{9,4},{5,7}};
    printf("Using key matrix:\n[9 4]\n[5 7]\n");

    char* enc = hill_encrypt(text, K);
    char* dec = hill_decrypt(enc, K);

    printf("Encrypted: %s\n", enc);
    printf("Decrypted: %s\n", dec);

    free(enc);
    free(dec);
    press_enter();
}

static void learn_transposition() {
    printf("\n=== Columnar Transposition ===\n");

    char text[200], key[50];
    printf("Enter plaintext: ");
    fgets(text, sizeof(text), stdin); trim_newline(text);

    printf("Enter key: ");
    fgets(key, sizeof(key), stdin); trim_newline(key);

    char* enc = transposition_encrypt(text, key);
    char* dec = transposition_decrypt(enc, key);

    printf("Encrypted: %s\n", enc);
    printf("Decrypted: %s\n", dec);

    free(enc);
    free(dec);

    press_enter();
}

void learning_menu() {
    int ch;

    while (1) {
        printf("\n=== Learning Mode ===\n");
        printf("1. Caesar Cipher\n");
        printf("2. Vigenere Cipher\n");
        printf("3. Playfair Cipher\n");
        printf("4. Hill Cipher\n");
        printf("5. Transposition Cipher\n");
        printf("6. Back\n");
        printf("Enter choice: ");

        scanf("%d", &ch);
        clear_input_buffer();

        switch (ch) {
            case 1: learn_caesar(); break;
            case 2: learn_vigenere(); break;
            case 3: learn_playfair(); break;
            case 4: learn_hill(); break;
            case 5: learn_transposition(); break;
            case 6: return;
            default: printf("Invalid choice!\n");
        }
    }
}
