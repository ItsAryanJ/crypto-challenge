#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "caesar.h"

char* caesar_encrypt(const char* text, int shift) {
    int len = strlen(text);
    char* out = (char*)malloc(len + 1);

    for (int i = 0; i < len; i++) {
        char c = text[i];
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            out[i] = ( (c - base + shift) % 26 ) + base;
        } else {
            out[i] = c;
        }
    }
    out[len] = '\0';
    return out;
}

char* caesar_decrypt(const char* text, int shift) {
    return caesar_encrypt(text, 26 - (shift % 26));
}
