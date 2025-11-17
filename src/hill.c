#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "hill.h"

static int mod26(int x) {
    x %= 26;
    if (x < 0) x += 26;
    return x;
}

int is_invertible_2x2(int K[2][2]) {
    int det = mod26(K[0][0]*K[1][1] - K[0][1]*K[1][0]);
    for (int i = 1; i < 26; i++)
        if ((det * i) % 26 == 1)
            return 1;
    return 0;
}

static void invert_matrix(int K[2][2], int inv[2][2]) {
    int det = mod26(K[0][0]*K[1][1] - K[0][1]*K[1][0]);

    int det_inv = 0;
    for (int i = 1; i < 26; i++)
        if ((det * i) % 26 == 1)
            det_inv = i;

    inv[0][0] = mod26( K[1][1] * det_inv );
    inv[0][1] = mod26(-K[0][1] * det_inv );
    inv[1][0] = mod26(-K[1][0] * det_inv );
    inv[1][1] = mod26( K[0][0] * det_inv );
}

char* hill_encrypt(const char* plaintext, int K[2][2]) {
    char clean[512];
    int j = 0;

    for (int i = 0; plaintext[i]; i++)
        if (isalpha(plaintext[i]))
            clean[j++] = toupper(plaintext[i]);

    if (j % 2 != 0) clean[j++] = 'X';
    clean[j] = '\0';

    char* out = malloc(j + 1);

    for (int i = 0; i < j; i += 2) {
        int a = clean[i] - 'A';
        int b = clean[i+1] - 'A';

        int c0 = mod26(K[0][0]*a + K[0][1]*b);
        int c1 = mod26(K[1][0]*a + K[1][1]*b);

        out[i] = 'A' + c0;
        out[i+1] = 'A' + c1;
    }
    out[j] = '\0';
    return out;
}

char* hill_decrypt(const char* ciphertext, int K[2][2]) {
    int len = strlen(ciphertext);
    char* out = malloc(len + 1);

    int inv[2][2];
    invert_matrix(K, inv);

    for (int i = 0; i < len; i += 2) {
        int a = toupper(ciphertext[i]) - 'A';
        int b = toupper(ciphertext[i+1]) - 'A';

        int p0 = mod26(inv[0][0]*a + inv[0][1]*b);
        int p1 = mod26(inv[1][0]*a + inv[1][1]*b);

        out[i] = 'A' + p0;
        out[i+1] = 'A' + p1;
    }

    out[len] = '\0';
    return out;
}
