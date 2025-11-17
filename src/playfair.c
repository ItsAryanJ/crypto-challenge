#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "playfair.h"

static void generate_matrix(const char* key, char matrix[5][5]) {
    int used[26] = {0};
    char k[256];
    int idx = 0;

    // Prepare key
    for (int i = 0; key[i]; i++) {
        char c = toupper(key[i]);
        if (c == 'J') c = 'I';
        if (isalpha(c) && !used[c - 'A']) {
            used[c - 'A'] = 1;
            k[idx++] = c;
        }
    }

    // Fill remaining letters
    for (char c = 'A'; c <= 'Z'; c++) {
        if (c == 'J') continue;
        if (!used[c - 'A']) {
            k[idx++] = c;
        }
    }

    // Fill matrix
    idx = 0;
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 5; c++)
            matrix[r][c] = k[idx++];
}

static void find_pos(char matrix[5][5], char c, int* row, int* col) {
    if (c == 'J') c = 'I';
    for (int r = 0; r < 5; r++)
        for (int c2 = 0; c2 < 5; c2++)
            if (matrix[r][c2] == c) {
                *row = r;
                *col = c2;
                return;
            }
}

static void preprocess(const char* text, char* out) {
    int j = 0;
    for (int i = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            char c = toupper(text[i]);
            if (c == 'J') c = 'I';
            out[j++] = c;
        }
    }
    out[j] = '\0';

    // Insert 'X' for repeated letters in pairs
    char temp[512];
    int k = 0;
    for (int i = 0; out[i]; ) {
        char a = out[i];
        char b = out[i+1];

        if (!b) {
            temp[k++] = a;
            temp[k++] = 'X';
            break;
        }

        if (a == b) {
            temp[k++] = a;
            temp[k++] = 'X';
            i++;
        } else {
            temp[k++] = a;
            temp[k++] = b;
            i += 2;
        }
    }
    temp[k] = '\0';
    strcpy(out, temp);
}

char* playfair_encrypt(const char* plaintext, const char* key) {
    char matrix[5][5];
    generate_matrix(key, matrix);

    char prep[512];
    preprocess(plaintext, prep);

    int len = strlen(prep);
    char* out = malloc(len + 1);

    for (int i = 0; i < len; i += 2) {
        int r1, c1, r2, c2;
        find_pos(matrix, prep[i], &r1, &c1);
        find_pos(matrix, prep[i+1], &r2, &c2);

        if (r1 == r2) {
            out[i]   = matrix[r1][(c1+1)%5];
            out[i+1] = matrix[r2][(c2+1)%5];
        }
        else if (c1 == c2) {
            out[i]   = matrix[(r1+1)%5][c1];
            out[i+1] = matrix[(r2+1)%5][c2];
        }
        else {
            out[i]   = matrix[r1][c2];
            out[i+1] = matrix[r2][c1];
        }
    }

    out[len] = '\0';
    return out;
}

char* playfair_decrypt(const char* ciphertext, const char* key) {
    char matrix[5][5];
    generate_matrix(key, matrix);

    int len = strlen(ciphertext);
    char* out = malloc(len + 1);

    for (int i = 0; i < len; i += 2) {
        int r1, c1, r2, c2;
        find_pos(matrix, ciphertext[i], &r1, &c1);
        find_pos(matrix, ciphertext[i+1], &r2, &c2);

        if (r1 == r2) {
            out[i]   = matrix[r1][(c1+4)%5];
            out[i+1] = matrix[r2][(c2+4)%5];
        }
        else if (c1 == c2) {
            out[i]   = matrix[(r1+4)%5][c1];
            out[i+1] = matrix[(r2+4)%5][c2];
        }
        else {
            out[i]   = matrix[r1][c2];
            out[i+1] = matrix[r2][c1];
        }
    }

    out[len] = '\0';
    return out;
}
