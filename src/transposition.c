#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "transposition.h"

static int charcmp(const void* a, const void* b) {
    return (*(char*)a - *(char*)b);
}

char* transposition_encrypt(const char* plaintext, const char* key) {
    int len = strlen(plaintext);
    int klen = strlen(key);

    char clean[512];
    int j = 0;
    for (int i = 0; i < len; i++)
        if (isalpha(plaintext[i]))
            clean[j++] = toupper(plaintext[i]);
    clean[j] = '\0';

    int rows = (j + klen - 1) / klen;
    char table[rows][klen];

    int idx = 0;
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < klen; c++)
            table[r][c] = (idx < j ? clean[idx++] : 'X');

    char sorted[64];
    strcpy(sorted, key);
    for (int i = 0; i < klen; i++) sorted[i] = toupper(sorted[i]);
    qsort(sorted, klen, sizeof(char), charcmp);

    char* out = malloc(rows * klen + 1);
    int pos = 0;

    for (int s = 0; s < klen; s++) {
        for (int c = 0; c < klen; c++) {
            if (toupper(key[c]) == sorted[s]) {
                for (int r = 0; r < rows; r++)
                    out[pos++] = table[r][c];
                break;
            }
        }
    }
    out[pos] = '\0';
    return out;
}

char* transposition_decrypt(const char* ciphertext, const char* key) {
    int len = strlen(ciphertext);
    int klen = strlen(key);
    int rows = len / klen;

    char sorted[64];
    strcpy(sorted, key);
    for (int i = 0; i < klen; i++) sorted[i] = toupper(sorted[i]);
    qsort(sorted, klen, sizeof(char), charcmp);

    char table[rows][klen];

    int idx = 0;
    for (int s = 0; s < klen; s++) {
        for (int c = 0; c < klen; c++) {
            if (toupper(key[c]) == sorted[s]) {
                for (int r = 0; r < rows; r++)
                    table[r][c] = ciphertext[idx++];
                break;
            }
        }
    }

    char* out = malloc(len + 1);
    int pos = 0;

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < klen; c++)
            out[pos++] = table[r][c];

    out[pos] = '\0';
    return out;
}
