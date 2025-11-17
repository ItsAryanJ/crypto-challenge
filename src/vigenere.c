#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "vigenere.h"

char* vigenere_encrypt(const char* text, const char* key) {
    int len = strlen(text);
    int klen = strlen(key);
    char* out = (char*)malloc(len + 1);

    int j = 0;
    for (int i = 0; i < len; i++) {
        char c = text[i];
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            char k = toupper(key[j % klen]) - 'A';
            out[i] = ( (c - base + k) % 26 ) + base;
            j++;
        } else {
            out[i] = c;
        }
    }
    out[len] = '\0';
    return out;
}

char* vigenere_decrypt(const char* text, const char* key) {
    int len = strlen(text);
    int klen = strlen(key);
    char* out = (char*)malloc(len + 1);

    int j = 0;
    for (int i = 0; i < len; i++) {
        char c = text[i];
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            char k = toupper(key[j % klen]) - 'A';
            out[i] = ( (c - base - k + 26) % 26 ) + base;
            j++;
        } else {
            out[i] = c;
        }
    }
    out[len] = '\0';
    return out;
}
