#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "utils.h"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void press_enter() {
    printf("\nPress Enter to continue...");
    clear_input_buffer();
}

void trim_newline(char* s) {
    int len = strlen(s);
    if (len > 0 && s[len - 1] == '\n')
        s[len - 1] = '\0';
}

void str_upper(char* s) {
    for (int i = 0; s[i]; i++)
        s[i] = toupper(s[i]);
}

void remove_nonalpha(char* dest, const char* src) {
    int j = 0;
    for (int i = 0; src[i]; i++) {
        if (isalpha(src[i]))
            dest[j++] = src[i];
    }
    dest[j] = '\0';
}

int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}
